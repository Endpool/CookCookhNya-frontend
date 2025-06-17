#pragma once

#include "render.hpp"
#include "states.hpp"
#include "types.hpp"
#include "utils.hpp"

#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>
#include <tg_stater/handler/type.hpp>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/Message.h>

#include <memory>
#include <string>
#include <vector>

namespace handlers {
using namespace TgBot;
using namespace tg_stater;
using NoState = HandlerTypes::NoState;
using AnyState = HandlerTypes::AnyState;
using namespace states;
using namespace render;

using MessageRef = const Message&;
using CallbackQueryRef = const CallbackQuery&;
using SMRef = const StateManager&;


constexpr char startCmd[] = "start";
inline void start(MessageRef m, BotRef bot, SMRef stateManager) {
    stateManager.put(StorageView{0});
    renderStorageList(m.from->id, m.chat->id, bot);
    std::cerr<< "hello.";
};
using startHandler = Handler<Events::Command{startCmd}, start, AnyState{}>;

inline void storageViewButtonCallback(StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "explore") {
        stateManager.put(IngredientsView{state.storageId});
        renderIngredientsList(state.storageId, userId, chatId, bot);
    } else if (cq.data == "members") {
        stateManager.put(StorageMemberView{state.storageId});
        renderMemberList(state.storageId, userId, chatId, bot);
    } else if (cq.data == "back") {
        stateManager.put(StorageList{});
        renderStorageList(userId, chatId, bot);
    }
}
using storageViewButtonHandler = Handler<Events::CallbackQuery{}, storageViewButtonCallback>;

inline void storageMemberViewButtonCallback(StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "add_delete_member") {
        stateManager.put(MembersAdditionDeletion{state.storageId});
        renderMemberAdditionDeletionPrompt(state.storageId, chatId, bot);
    } else if (cq.data == "back") {
        stateManager.put(StorageView{state.storageId});
        renderStorageList(userId, chatId, bot);
    }
}
using storageMemberViewButtonHandler = Handler<Events::CallbackQuery{}, storageMemberViewButtonCallback>;

inline void addDeleteMember(MembersAdditionDeletion& state, MessageRef m, BotRef bot, SMRef stateManager) { 
    auto chatId = m.chat->id;
    auto userId = m.from->id;
    auto memberId = utils::parseSafe<UserId>(m.text.data());
    auto storage = StorageRepositoryClass::Storage::get(state.storageId);
    if (!memberId) {
        bot.sendMessage(chatId, "Invalid user ID");
        renderMemberAdditionDeletionPrompt(state.storageId, chatId, bot);
        return;
    }
    if (storage.addMember(state.storageId, *memberId)) {
        bot.sendMessage(chatId, "Member added successfully");
    } else {
        bot.sendMessage(chatId, "Member already added");
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

} // namespace handlers