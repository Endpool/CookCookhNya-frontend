#pragma once

#include "backend/models/storage.hpp"
#include "render.hpp"
#include "states.hpp"
#include "types.hpp"
#include "utils.hpp"

#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>
#include <tg_stater/handler/type.hpp>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/Message.h>
#include <backend/api/storages.hpp>
#include <iostream>
#include <sstream>
#include <string>

namespace cookcookhnya::handlers {

using namespace TgBot;
using namespace tg_stater;
using NoState = HandlerTypes::NoState;
using AnyState = HandlerTypes::AnyState;
using namespace states;
using namespace render;
using namespace cookcookhnya::api;

using BackendApiRef = const StoragesApi&;
using MessageRef = const Message&;
using CallbackQueryRef = const CallbackQuery&;
using SMRef = const StateManager&;

inline bool filterPublicMessage(MessageRef m, BotRef bot) {
    if (!m.from || m.chat->id != m.from->id) {
        bot.sendMessage(m.chat->id, "Setup stickers in private messages");
        return true;
    }
    return false;
}

constexpr char startCmd[] = "start";
inline void start(MessageRef m, BotRef bot, SMRef stateManager, BackendApiRef api) {
    stateManager.put(StorageList{});
    renderStorageList(m.from->id, m.chat->id, bot, api);
    std::cerr << "hello.";
};
using startHandler = Handler<Events::Command{startCmd}, start, AnyState{}>;

inline void handleNoState(MessageRef m, BotRef bot) {
    if (m.text.starts_with("/start"))
        return;
    bot.sendMessage(m.chat->id, "Use /start please");
};
using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, HandlerTypes::NoState{}>;

inline void storageListButtonCallback(StorageList&,
                                      CallbackQueryRef cq,
                                      const Api& bot,
                                      SMRef stateManager, BackendApiRef api) { // BackendProvider bkn
    bot.answerCallbackQuery(cq.id);

    std::stringstream temp; // Convert string to int
    temp << cq.data;
    int id = 0;
    temp >> id;

    auto chatId = cq.message->chat->id;
    if (cq.data == "StorageViewCreate") {
        stateManager.put(StorageCreationEnterName{}); // Go to function create storage, while cancel button is handled
                                                      // on cancel storage creation
        renderStorageCreate(chatId, bot, cq.from->id);             // Bot here prints menu of storage creation ADD USER ID
        return;
    }

    if (cq.data == "StorageViewDelete") {
        stateManager.put(StorageDeletionEnterName{});
        renderStorageDelete(chatId, bot, cq.from->id, api); // Need for api, so it could put the list of storages to delete
        return;
    }

    stateManager.put(StorageView{id});
    renderStorageView(id, cq.from->id, chatId, bot); // If nor buttons were pressed then user pressed on their storages
}
using StorageListButtonHandler = Handler<Events::CallbackQuery{}, storageListButtonCallback>;

inline void storageViewButtonCallback(StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "explore") {
        //stateManager.put(IngredientsView{state.storageId}); temporarily not available
        renderIngredientsList(state.storageId, userId, chatId, bot);
    } else if (cq.data == "members") {
        stateManager.put(StorageMemberView{state.storageId});
        renderMemberList(state.storageId, userId, chatId, bot);
    } else if (cq.data == "back") {
        stateManager.put(StorageList{});
        renderStorageList(userId, chatId, bot, api);
    }
}
using storageViewButtonHandler = Handler<Events::CallbackQuery{}, storageViewButtonCallback>;

inline void
storageMemberViewButtonCallback(StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager,  BackendApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "add_delete_member") {
        stateManager.put(MembersAdditionDeletion{state.storageId});
        renderMemberAdditionDeletionPrompt(state.storageId, chatId, bot);
    } else if (cq.data == "back") {
        stateManager.put(StorageView{state.storageId});
        renderStorageList(userId, chatId, bot, api);
    }
}
using storageMemberViewButtonHandler = Handler<Events::CallbackQuery{}, storageMemberViewButtonCallback>;

inline void addDeleteMember(MembersAdditionDeletion& state, MessageRef m, BotRef bot, SMRef stateManager, BackendApiRef api) {
    auto chatId = m.chat->id;
    auto userId = m.from->id;
    auto memberId = utils::parseSafe<UserId>(m.text.data());
    auto storage = api.get(userId, state.storageId);
    if (!memberId) {
        bot.sendMessage(chatId, "Invalid user ID");
        renderMemberAdditionDeletionPrompt(state.storageId, chatId, bot);
        return;
    }
    if (api.memberOf(userId, state.storageId, *memberId)){
        api.deleteMember(userId, state.storageId, *memberId);
        bot.sendMessage(chatId, "Member deleted successfully");
    } else {
        api.addMember(userId, state.storageId, *memberId);
        bot.sendMessage(chatId, "Member added successfully");
    }
    stateManager.put(StorageMemberView{state.storageId});
    renderMemberList(state.storageId, userId, chatId, bot);
}
using memberAdditionDeletionMessageHandler = Handler<Events::Message{}, addDeleteMember>;

inline void cancelAddDeleteMember(MembersAdditionDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "member_add_delete_cancel") {
        stateManager.put(StorageMemberView{state.storageId});
        renderMemberList(state.storageId, userId, chatId, bot);
    }
}
using cancelAddDeleteMemberHandler = Handler<Events::CallbackQuery{}, cancelAddDeleteMember>;

inline void createStorage(StorageCreationEnterName&, MessageRef m, BotRef bot, SMRef stateManager, BackendApiRef api) { // BackendProvider bkn
    api.create(m.from->id, models::storage::StorageCreateBody{m.text}); // Create storage bpdy with new name
    stateManager.put(StorageList{});
    renderStorageList(m.from->id, m.chat->id, bot, api);
};


using StorageCreateHandler = Handler<Events::Message{}, createStorage>;

inline void cancelStorageCreation(StorageCreationEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "StorageCreateCancel") { // Here compare with data in button which was pressed (data was put in
                                            // renderStorageCreate)
        stateManager.put(StorageList{});
        renderStorageList(cq.from->id, cq.message->chat->id, bot, api);
    }
};
using StorageCreateButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageCreation>;

inline void deleteStorage(StorageDeletionEnterName&,
                          CallbackQueryRef cq,
                          BotRef bot,
                          SMRef stateManager, BackendApiRef api) { 

    std::stringstream temp;
    temp << cq.data;
    StorageId id = 0;
    temp >> id; // Probably dangerous (However can be okay as storageId is long int)
    api.delete_(cq.from->id, id); // delete by a userid and id of their storage
};
using storgeDeleteHandler = Handler<Events::CallbackQuery{}, deleteStorage>;

inline void cancelStorageDeletion(StorageDeletionEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "cancel") {
        stateManager.put(StorageList{});
        renderStorageList(cq.from->id, cq.message->chat->id, bot, api);
    }
};
using StorageDeleteButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageDeletion>;

} // namespace cookcookhnya::handlers
