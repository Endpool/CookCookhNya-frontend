#include "view.hpp"

#include "backend/api/storages.hpp"
#include "backend/models/storage.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "states.hpp"
#include "utils/utils.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::select_storages {

using api::models::storage::StorageSummary;
using states::StoragesSelection;
using namespace tg_types;
using namespace std::views;

void renderStorageSelection(
    const StoragesSelection& state, UserId userId, ChatId chatId, BotRef bot, api::StorageApiRef storageApi) {
    const auto& selectedStorages = state.selectedStorages;
    auto allStorages = storageApi.getStoragesList(userId);

    const std::size_t buttonRows = ((allStorages.size() + 1) / 2) + 1; // ceil(storagesCount / 2) and back
    InlineKeyboardBuilder keyboard{buttonRows};

    for (auto chunk : allStorages | chunk(2)) {
        keyboard.reserveInRow(2);
        for (auto& storage : chunk) {
            const bool isSelected = std::ranges::contains(selectedStorages, storage.id, &StorageSummary::id);
            std::string emoji = utils::utf8str(isSelected ? u8"[ + ]" : u8"[ᅠ]");
            const char* actionPrefix = isSelected ? "+" : "-";
            const std::string text = std::format("{} {}", emoji, storage.name);
            const std::string data = actionPrefix + utils::to_string(storage.id);
            keyboard << makeCallbackButton(text, data);
        }
        keyboard << NewRow{};
    }

    keyboard.reserveInRow(2);
    keyboard << makeCallbackButton(u8"↩️ Назад", "cancel");
    if (!selectedStorages.empty())
        keyboard << makeCallbackButton(u8"▶️ Подтвердить", "confirm");

    auto text = utils::utf8str(u8"🍱 Откуда брать продукты?");
    auto messageId = message::getMessageId(userId);
    if (messageId)
        bot.editMessageText(text, chatId, *messageId, std::move(keyboard).build());
    else {
        auto message = bot.sendMessage(chatId, text, std::move(keyboard).build());
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::select_storages
