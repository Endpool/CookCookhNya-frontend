#include "select.hpp"

#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::select_storages {

using namespace tg_types;

void renderStorageSelect(const std::vector<api::StorageId>& selected_storages,
                         UserId userId,
                         ChatId chatId,
                         BotRef bot,
                         StorageApiRef storageApi) {
    auto storages = storageApi.getStoragesList(userId);
    const std::size_t buttonRows = ((storages.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);

    for (std::size_t i = 0; i < storages.size(); ++i) {
        if (i % 2 == 0)
            keyboard[i / 2].reserve(2);
        const bool isSelected = std::ranges::find(selected_storages, storages[i].id) != selected_storages.end();

        std::string emoji = utils::utf8str(isSelected ? u8"🔘" : u8"⚪️");
        const char* actionPrefix = isSelected ? "in__" : "out_";
        const std::string text = std::format("{} {}", emoji, storages[i].name);
        const std::string data = actionPrefix + std::to_string(storages[i].id);
        keyboard[i / 2].push_back(makeCallbackButton(text, data));
    }
    keyboard[buttonRows - 1].reserve(2);
    keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"↩️ Назад", "cancel"));
    if (!selected_storages.empty()) {
        keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"▶️ Подтвердить", "confirm"));
    }
    auto text = utils::utf8str(u8"🍱 Откуда брать продукты?");
    auto messageId = message::getMessageId(userId);
    if (messageId)
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::select_storages
