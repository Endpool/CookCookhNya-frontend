#include "view.hpp"

#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <format>
#include <iterator>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::storage::members {

void renderMemberList(bool toBeEdited,
                      const api::StorageId& storageId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    const bool isOwner = storage.ownerId == userId;
    const int buttonRows = isOwner ? 2 : 1;

    InlineKeyboard keyboard(buttonRows);

    if (isOwner) {
        keyboard[0].push_back(makeCallbackButton(u8"🔐 Добавить", "add"));
        if (storageApi.getStorageMembers(userId, storageId).size() > 1) {
            keyboard[0].push_back(makeCallbackButton(u8"🔒 Удалить", "delete"));
        }
        keyboard[1].push_back(makeCallbackButton(u8"↩️Назад", "back"));
    } else {
        keyboard[0].push_back(makeCallbackButton(u8"↩️Назад", "back"));
    }

    std::string list;
    std::vector<std::string> memberNames;
    for (const auto& memberDetails : storageApi.getStorageMembers(userId, storageId)) {
        memberNames.push_back(memberDetails.fullName);
    }
    for (auto [i, name] : std::views::enumerate(memberNames))
        std::format_to(std::back_inserter(list), "  {}. {}\n", i + 1, name);
    auto text = utils::utf8str(u8"👥 Список участников\n") + list;
    if (toBeEdited) {
        auto messageId = message::getMessageId(userId);
        if (messageId)
            bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
    } else {
        auto messageId = bot.sendMessage(chatId, text, makeKeyboardMarkup(std::move(keyboard)));
        message::addMessageId(userId, messageId->messageId);
    }
};

} // namespace cookcookhnya::render::storage::members
