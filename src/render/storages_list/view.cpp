#include "view.hpp"

#include "backend/api/storages.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <ranges>
#include <utility>

namespace cookcookhnya::render::storages_list {

using namespace tg_types;
using namespace std::views;

void renderStorageList(bool toBeEdited, UserId userId, ChatId chatId, BotRef bot, api::StorageApiRef storageApi) {
    const auto text = utils::utf8str(u8"🍱 Ваши хранилища");

    auto storages = storageApi.getStoragesList(userId);

    const std::size_t buttonRows = ((storages.size() + 1) / 2) + 1; // ceil(storagesCount / 2) and back
    InlineKeyboardBuilder keyboard{buttonRows};

    for (auto chunk : storages | chunk(2)) {
        keyboard.reserveInRow(2);
        for (auto& s : chunk)
            keyboard << makeCallbackButton(utils::utf8str(u8"🍱 ") + s.name, utils::to_string(s.id));
        keyboard << NewRow{};
    }

    keyboard << makeCallbackButton(u8"↩️ Назад", "back") << makeCallbackButton(u8"🆕 Создать", "create");

    if (toBeEdited) {
        auto messageId = message::getMessageId(userId);
        if (messageId) {
            bot.editMessageText(text, chatId, *messageId, std::move(keyboard));
        }
    } else {
        auto message = bot.sendMessage(chatId, text, std::move(keyboard));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::storages_list
