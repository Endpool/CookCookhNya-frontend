#include "select_storages_render.hpp"

#include "render/common.hpp"
#include "tg_types.hpp"

namespace cookcookhnya::render::select_storages {

MessageId renderStoragesSelect(UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    std::cerr << "in render/select_storages_render.cpp";
    auto storages = storageApi.getStoragesList(userId); 

    unsigned long buttonRows = ((storages.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);

    for (uint32_t i = 0; i < storages.size(); ++i) {
        if (i % 2 == 0)
            keyboard[(i / 2)].reserve(2);
        keyboard[(i / 2)].push_back(
            detail::makeCallbackButton("❎" + storages[i].name, "out" + std::to_string(storages[i].id)));
    }
    keyboard[buttonRows - 1].reserve(2);
    keyboard[buttonRows - 1].push_back(detail::makeCallbackButton("Confirm", "confirm_storages_selection")); 
    keyboard[buttonRows - 1].push_back(detail::makeCallbackButton("Back", "cancel_storages_selection")); 

    auto message = bot.sendMessage(chatId, "Consider ingredients from:", nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    std::cerr << "  done rendering\n";
    return message->messageId;
}

void updateStorageSelect(std::vector<StorageId> selected_storages, MessageId messageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi){
    auto storages = storageApi.getStoragesList(userId); //🟢 🔴

    unsigned long buttonRows = ((storages.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);

    for (uint32_t i = 0; i < storages.size(); ++i) {
        if (i % 2 == 0)
            keyboard[(i / 2)].reserve(2);
        bool isSelected = std::ranges::find(selected_storages, storages[i].id) != selected_storages.end();
        if (isSelected){
            keyboard[(i / 2)].push_back(
            detail::makeCallbackButton("🟢"+storages[i].name, "in_" + std::to_string(storages[i].id)));
        } else {
            keyboard[(i / 2)].push_back(
            detail::makeCallbackButton("🔴" + storages[i].name, "out_" + std::to_string(storages[i].id)));
        }
    }
    keyboard[buttonRows - 1].reserve(2);
    keyboard[buttonRows - 1].push_back(detail::makeCallbackButton("Confirm", "confirm_storages_selection"));
    keyboard[buttonRows - 1].push_back(detail::makeCallbackButton("Back", "cancel_storages_selection"));

    bot.editMessageText("Consider ingredients from:", chatId, messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    std::cerr << "  done rendering\n";
}
} // namespace cookcookhnya::render::select_storages
