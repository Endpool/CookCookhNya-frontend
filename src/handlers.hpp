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
#include <format>
#include <stdexcept>
#include <utility>
#include <sstream>
#include <iostream>

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

inline bool filterPublicMessage(MessageRef m, BotRef bot) {
    if (!m.from || m.chat->id != m.from->id) {
        bot.sendMessage(m.chat->id, "Setup stickers in private messages");
        return true;
    }
    return false;
}

constexpr char startCmd[] = "start";
inline void start(MessageRef m, BotRef bot, SMRef stateManager) {
    stateManager.put(StorageView{0});
    renderStorageList(m.from->id, m.chat->id, bot);
    std::cerr<< "hello.";
};
using startHandler = Handler<Events::Command{startCmd}, start, AnyState{}>;
  
inline void handleNoState(MessageRef m, BotRef bot) {
    if (m.text.starts_with("/start"))
        return;
    bot.sendMessage(m.chat->id, "Use /start please");
};
using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, HandlerTypes::NoState{}>;

inline void storageListButtonCallback (StorageList& ,CallbackQueryRef cq, const Api& bot,  SMRef stateManager, const HelloWorldProvider& hwp){ //BackendProvider bkn
    bot.answerCallbackQuery(cq.id);
    //std::string temp = (cq.data.substr(7)); // 7 Because string created on render of button is "storage *idNumber*"
    std::stringstream temp;
    temp<<cq.data;

    int id = 0;
    temp >> id;
    auto chatId = cq.message->chat->id;
    if (cq.data == "StorageViewCreate"){
        stateManager.put(StorageCreationEnterName{}); // Go to function create storage, while cancel button is handled on cancel storage creation 
        renderStorageCreate(chatId, bot); // Bot here prints menu of storage creation 
        return;
    }
    
    if (cq.data == "StorageViewDelete"){
        stateManager.put(StorageDeletionEnterName{});
        renderStorageDelete(chatId, bot);
        return;
    }
    
    stateManager.put(StorageView{id});
    renderStorageView(id, cq.from->id, chatId, bot); // If nor buttons were pressed then user pressed on their storages
}
using StorageListButtonHandler = Han

  
void helloWorld(MessageRef m, BotRef bot, const HelloWorldProvider& hwp) {
    bot.sendMessage(m.chat->id, hwp.getHelloWorld());
}
using helloWorldHandler = Handler<Events::Message{}, helloWorld, HandlerTypes::AnyState{}>;

inline void handleNoState(MessageRef m, BotRef bot) {
    if (m.text.starts_with("/start"))
        return;
    bot.sendMessage(m.chat->id, "Use /start please");
};
using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, HandlerTypes::NoState{}>;

constexpr char startCmd[] = "start";
inline void start(MessageRef m, BotRef bot, SMRef stateManager) {
    stateManager.put(StorageList{});
    renderStoragesView(m.from->id, m.chat->id, bot);
};
using startHandler = Handler<Events::Command{startCmd}, start, HandlerTypes::AnyState{}>;dler<Events::CallbackQuery{}, storageListButtonCallback>;

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

inline void createStorage(StorageCreationEnterName&, MessageRef m, BotRef bot, SMRef stateManager) { //BackendProvider bkn
    backendEx.createStorage(m.from->id, m.text);
    stateManager.put(StorageList{});
    renderStoragesView(m.from->id, m.chat->id, bot);
};
using StorageCreateHandler = Handler<Events::Message{}, createStorage>;

inline void cancelStorageCreation(StorageCreationEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "StorageCreateCancel") { // Here compare with data in button which was pressed (data was put in renderStorageCreate)
        stateManager.put(StorageList{});
        renderStoragesView(cq.from->id, cq.message->chat->id, bot);
    }
};
using StorageCreateButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageCreation>;

inline bool deleteStorage(StorageDeletionEnterName&, MessageRef m, BotRef bot, SMRef stateManager, const HelloWorldProvider& hwp) { //BackendProvider bkn
    if (backendEx.deleteStorage(m.from->id, m.text)){
        stateManager.put(StorageList{});
        renderStoragesView(m.from->id, m.chat->id, bot);
        return true;
    } else {
        bot.sendMessage(m.chat->id,"You entered wrong name");
        return false;
    }
};
using storgeDeleteHandler = Handler<Events::Message{}, deleteStorage>;

inline void cancelStorageDeletion(StorageDeletionEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "cancel") {
        stateManager.put(StorageList{});
        renderStoragesView(cq.from->id, cq.message->chat->id, bot);
    }
};
using StorageDeleteButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageDeletion>;


} // namespace