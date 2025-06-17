#pragma once

#include "render.hpp"
#include "settings.hpp"
#include "states.hpp"
#include "types.hpp"
#include "utils.hpp"

#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>
#include <tg_stater/handler/type.hpp>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/Message.h>
#include <uuid.h>

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

inline void storageViewButtonCallback(StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "explore") {
        stateManager.put(IngredientsView{state.storageId});
        renderIngredientsList(userId, chatId, bot);
    } else if (cq.data == "members") {
        stateManager.put(PackMemberView{state.storageId});
        bool isCreator = StorageRepository::isCreator(state.storageId, userId);
        renderMemberList(state.storageId, userId, chatId, bot, isCreator);
    } else if (cq.data == "back") {
        stateManager.put(StorageList{});
        renderStorageList(userId, chatId, bot);
    }
}
using storageViewButtonHandler = Handler<Events::CallbackQuery{}, storageViewButtonCallback, StorageView{};

inline void packMemberViewButtonCallback(PackMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager) { // or add another state for creater and non-creater
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    bool isCreator = StorageRepository::isCreator(state.storageId, userId);
    if (!isCreator) {
        bot.sendMessage(chatId, "Only the creator can modify members");
        return;
    }
    if (cq.data == "add_member") {
        stateManager.put(MemberAddition{state.storageId});
        renderMemberAdditionPrompt(chatId, bot);
    } else if (cq.data == "delete_member") {
        stateManager.put(MemberDeletion{state.storageId});
        renderMemberDeletionPrompt(chatId, bot);
    } else if (cq.data == "back") {
        stateManager.put(StorageView{state.storageId});
        renderStorageView(StorageId{state.storageId}, userId, chatId, bot);
    }
}
using packMemberViewButtonHandler = Handler<Events::CallbackQuery{}, packMemberViewButtonCallback, PackMemberView{}>;

inline void addMember(MemberAddition& state, MessageRef m, BotRef bot, SMRef stateManager) {
    auto chatId = m.chat->id;
    auto userId = m.from->id;
    auto memberId = utils::parseSafe<UserId>(m.text.data());
    if (!memberId) {
        bot.sendMessage(chatId, "Invalid user ID");
        renderMemberAdditionPrompt(chatId, bot);
        return;
    }
    if (MemberRepository::addMember(state.storageId, *memberId)) {
        bot.sendMessage(chatId, "Member added successfully");
    } else {
        bot.sendMessage(chatId, "Member already added");
    }
    stateManager.put(PackMemberView{state.storageId});
    bool isCreator = StorageRepository::isCreator(state.storageId, userId);
    renderMemberList(state.storageId, userId, chatId, bot, isCreator);
}
using memberAdditionHandler = Handler<Events::Message{}, addMember, MemberAddition{}>;

inline void deleteMember(MemberDeletion& state, MessageRef m, BotRef bot, SMRef stateManager) {
    auto chatId = m.chat->id;
    auto userId = m.from->id;
    auto memberId = utils::parseSafe<UserId>(m.text.data());
    if (!memberId) {
        bot.sendMessage(chatId, "Invalid user ID");
        renderMemberDeletionPrompt(chatId, bot);
        return;
    }
    if (MemberRepository::deleteMember(state.storageId, *memberId)) {
        bot.sendMessage(chatId, "Member removed successfully");
    } else {
        bot.sendMessage(chatId, "Failed to remove member or not found");
    }
    stateManager.put(PackMemberView{state.storageId});
    bool isCreator = StorageRepository::isCreator(state.storageId, userId);
    renderMemberList(state.storageId, userId, chatId, bot, isCreator);
}
using memberDeletionHandler = Handler<Events::Message{}, deleteMember, MemberDeletion{}>;

} // namespace handlers