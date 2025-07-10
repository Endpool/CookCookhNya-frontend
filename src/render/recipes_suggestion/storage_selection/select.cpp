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
        if (isSelected) {
            keyboard[i / 2].push_back(
                detail::makeCallbackButton(std::format("{} {}", utils::utf8str(u8"[ + ]"), storages[i].name),
                                           "in__" + std::to_string(storages[i].id)));
        } else {
            keyboard[i / 2].push_back(
                detail::makeCallbackButton(std::format("{} {}", utils::utf8str(u8"[ᅠ]"), storages[i].name),
                                           "out_" + std::to_string(storages[i].id)));
        }
    }
    keyboard[buttonRows - 1].reserve(2);
    keyboard[buttonRows - 1].push_back(detail::makeCallbackButton(u8"↩️ Назад", "cancel"));
    if (!selected_storages.empty()) {
        keyboard[buttonRows - 1].push_back(detail::makeCallbackButton(u8"▶️ Подтвердить", "confirm"));
    }
    auto text = utils::utf8str(u8"🍱 Откуда брать продукты?");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    }
}

} // namespace cookcookhnya::render::select_storages
