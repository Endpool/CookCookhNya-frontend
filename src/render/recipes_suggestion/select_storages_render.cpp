#include "select_storages_render.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

namespace cookcookhnya::render::select_storages {

using namespace tg_types;

void renderStoragesSelect(
    std::vector<StorageId> selected_storages, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storages = storageApi.getStoragesList(userId);

    unsigned long buttonRows = ((storages.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);

    for (uint32_t i = 0; i < storages.size(); ++i) {
        if (i % 2 == 0)
            keyboard[(i / 2)].reserve(2);
        bool isSelected = std::ranges::find(selected_storages, storages[i].id) != selected_storages.end();
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
    auto text = utils::utf8str(u8"Брать ингредиенты из:");
    auto messageId = cookcookhnya::message::getMessageId(userId);
    bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

void editStorageSelectMessage(
    std::vector<StorageId> selected_storages, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storages = storageApi.getStoragesList(userId); // 🟢 🔴

    unsigned long buttonRows = ((storages.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);
    for (uint32_t i = 0; i < storages.size(); ++i) {
        if (i % 2 == 0)
            keyboard[(i / 2)].reserve(2);
        bool isSelected = std::ranges::find(selected_storages, storages[i].id) != selected_storages.end();
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
    auto text = utils::utf8str(u8"Брать ингредиенты из:");
    auto messageId = cookcookhnya::message::getMessageId(userId);
    bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}
} // namespace cookcookhnya::render::select_storages
