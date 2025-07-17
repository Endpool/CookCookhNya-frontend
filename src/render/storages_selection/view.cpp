#include "view.hpp"

#include "backend/models/storage.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::select_storages {

using namespace tg_types;

void renderStorageSelection(const std::vector<api::models::storage::StorageSummary>& selectedStorages,
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
        const bool isSelected =
            std::ranges::find(selectedStorages, storages[i].id, &api::models::storage::StorageSummary::id) !=
            selectedStorages.end();

        std::string emoji = utils::utf8str(isSelected ? u8"[ + ]" : u8"[ᅠ]");
        const char* actionPrefix = isSelected ? "+" : "-";
        const std::string text = std::format("{} {}", emoji, storages[i].name);
        const std::string data = actionPrefix + utils::to_string(storages[i].id);
        keyboard[i / 2].push_back(makeCallbackButton(text, data));
    }
    keyboard[buttonRows - 1].reserve(2);
    keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"↩️ Назад", "cancel"));
    if (!selectedStorages.empty()) {
        keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"▶️ Подтвердить", "confirm"));
    }
    auto text = utils::utf8str(u8"🍱 Откуда брать продукты?");
    auto messageId = message::getMessageId(userId);
    if (messageId)
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::select_storages
