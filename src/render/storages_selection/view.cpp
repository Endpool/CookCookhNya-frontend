#include "view.hpp"

#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::select_storages {

using namespace states;
using namespace tg_types;

void renderStorageSelection(
    const StoragesSelection& state, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    const auto& selectedStorages = state.storageIds;
    auto allStorages = storageApi.getStoragesList(userId);

    const std::size_t buttonRows = ((allStorages.size() + 1) / 2) + 1; // ceil(storagesCount / 2) and back
    InlineKeyboardBuilder keyboard{buttonRows};

    for (auto [i, storage] : std::views::enumerate(allStorages)) {
        const bool isSelected = std::ranges::contains(selectedStorages, storage.id);
        std::string emoji = utils::utf8str(isSelected ? u8"[ + ]" : u8"[ᅠ]");
        const char* actionPrefix = isSelected ? "in__" : "out_";
        std::string text = std::format("{} {}", emoji, storage.name);
        std::string data = actionPrefix + utils::to_string(storage.id);

        if (i % 2 == 0)
            keyboard.reserveInRow(2);
        keyboard << makeCallbackButton(text, data);
        if (i % 2 == 1)
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
}

} // namespace cookcookhnya::render::select_storages
