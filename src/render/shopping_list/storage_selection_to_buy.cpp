#include "storage_selection_to_buy.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "states.hpp"
#include "utils/utils.hpp"

#include <ranges>
#include <utility>

namespace cookcookhnya::render::shopping_list {

using namespace std::views;

void renderShoppingListStorageSelectionToBuy(const states::ShoppingListStorageSelectionToBuy& state,
                                             UserId userId,
                                             ChatId chatId,
                                             BotRef bot) {
    InlineKeyboardBuilder keyboard{((state.storages.size() + 1) / 2) + 1}; // ceil(storagesCount / 2) and back

    // in two columns
    for (auto chunk : state.storages | chunk(2)) {
        keyboard.reserveInRow(2);
        for (const auto& s : chunk)
            keyboard << makeCallbackButton(utils::utf8str(u8"🍱 ") + s.name, "storage_" + utils::to_string(s.id));
        keyboard << NewRow{};
    }

    keyboard << makeCallbackButton(u8"↩️ Назад", "back");

    if (auto mMessageId = message::getMessageId(userId)) {
        bot.editMessageText(
            utils::utf8str(u8"Выберите хранилище, куда положить продукты"), chatId, *mMessageId, std::move(keyboard));
    }
}

} // namespace cookcookhnya::render::shopping_list
