#include "select_storages_render.hpp"

#include "backend/id_types.hpp"
#include "render/common.hpp"
#include "tg_types.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::select_storages {

using namespace tg_types;

MessageId renderStoragesSelect(const std::vector<api::StorageId>& selected_storages,
                               UserId userId,
                               ChatId chatId,
                               BotRef bot,
                               StorageApiRef storageApi) {
    auto storages = storageApi.getStoragesList(userId);

    const std::size_t buttonRows = ((storages.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);

    for (std::size_t i = 0; i < storages.size(); ++i) {
        if (i % 2 == 0)
            keyboard[(i / 2)].reserve(2);
        const bool isSelected = std::ranges::find(selected_storages, storages[i].id) != selected_storages.end();
        if (isSelected) {
            keyboard[(i / 2)].push_back(detail::makeCallbackButton(utils::utf8str(u8"✓ ") + storages[i].name,
                                                                   "in__" + std::to_string(storages[i].id)));
        } else {
            keyboard[(i / 2)].push_back(
                detail::makeCallbackButton(storages[i].name, "out_" + std::to_string(storages[i].id)));
        }
    }
    keyboard[buttonRows - 1].reserve(2);
    keyboard[buttonRows - 1].push_back(
        detail::makeCallbackButton(utils::utf8str(u8"Назад"), "cancel_storages_selection"));
    keyboard[buttonRows - 1].push_back(
        detail::makeCallbackButton(utils::utf8str(u8"Подтвердить"), "confirm_storages_selection"));

    auto message = bot.sendMessage(chatId,
                                   utils::utf8str(u8"Брать ингредиенты из:"),
                                   nullptr,
                                   nullptr,
                                   detail::makeKeyboardMarkup(std::move(keyboard)));
    return message->messageId;
}

void updateStorageSelect(const std::vector<api::StorageId>& selected_storages,
                         MessageId messageId,
                         UserId userId,
                         ChatId chatId,
                         BotRef bot,
                         StorageApiRef storageApi) {
    const auto storages = storageApi.getStoragesList(userId); // 🟢 🔴

    const std::size_t buttonRows = ((storages.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);
    for (std::size_t i = 0; i < storages.size(); ++i) {
        if (i % 2 == 0)
            keyboard[(i / 2)].reserve(2);
        const bool isSelected = std::ranges::find(selected_storages, storages[i].id) != selected_storages.end();
        if (isSelected) {
            keyboard[(i / 2)].push_back(detail::makeCallbackButton(utils::utf8str(u8"✓ ") + storages[i].name,
                                                                   "in__" + std::to_string(storages[i].id)));
        } else {
            keyboard[(i / 2)].push_back(
                detail::makeCallbackButton(storages[i].name, "out_" + std::to_string(storages[i].id)));
        }
    }
    keyboard[buttonRows - 1].reserve(2);
    keyboard[buttonRows - 1].push_back(
        detail::makeCallbackButton(utils::utf8str(u8"Назад"), "cancel_storages_selection"));
    keyboard[buttonRows - 1].push_back(
        detail::makeCallbackButton(utils::utf8str(u8"Подтвердить"), "confirm_storages_selection"));
    bot.editMessageReplyMarkup(chatId, messageId, "", detail::makeKeyboardMarkup(std::move(keyboard)));
}
} // namespace cookcookhnya::render::select_storages
