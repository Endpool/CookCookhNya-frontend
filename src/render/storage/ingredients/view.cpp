#include "view.hpp"

#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "render/pagination.hpp"
#include "states.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <memory>
#include <ranges>
#include <string>
#include <utility>

namespace TgBot {
class InlineKeyboardMarkup;
} // namespace TgBot

namespace cookcookhnya::render::storage::ingredients {

using namespace api::models::ingredient;
using namespace tg_types;
using namespace std::views;
using std::ranges::to;

namespace {

std::shared_ptr<TgBot::InlineKeyboardMarkup>
constructKeyboard(std::size_t pageNo, std::size_t pageSize, const states::StorageIngredientsList& state) {
    InlineKeyboardBuilder keyboard;

    auto searchButton = std::make_shared<TgBot::InlineKeyboardButton>();
    searchButton->text = utils::utf8str(u8"🛒 Добавить");
    searchButton->switchInlineQueryCurrentChat = "";
    keyboard << std::move(searchButton) << NewRow{};

    auto makeIngredientButton = [](const IngredientSearchForStorageItem& ing) {
        return makeCallbackButton((ing.isInStorage ? "[ + ] " : "[ㅤ] ") + ing.name, utils::to_string(ing.id));
    };
    keyboard << constructPagination(pageNo, pageSize, state.totalFound, state.searchItems, makeIngredientButton);

    if (!state.storageIngredients.getValues().empty())
        keyboard << makeCallbackButton(u8"🗑 Удалить", "delete") << NewRow{};
    keyboard << makeCallbackButton(u8"↩️ Назад", "back");

    return keyboard;
}

} // namespace

void renderIngredientsListSearch(const states::StorageIngredientsList& state,
                                 UserId userId,
                                 ChatId chatId,
                                 BotRef bot) {
    const std::size_t numOfIngredientsOnPage = 5;
    const std::string list = state.storageIngredients.getValues() |
                             transform([](auto& i) { return std::format("• {}\n", i.name); }) | join |
                             to<std::string>();

    auto text =
        state.storageIngredients.getValues().empty()
            ? utils::utf8str(u8"🍗 Кажется, в вашем хранилище пока нет никаких продуктов. Чтобы добавить новый, "
                             u8"нажмите на кнопку 🛒 Добавить и начните вводить название продукта...\n\n")
            : utils::utf8str(u8"🍗 Ваши продукты:\n\n");
    text += list;
    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(text, chatId, *messageId, constructKeyboard(state.pageNo, numOfIngredientsOnPage, state));
    }
}

} // namespace cookcookhnya::render::storage::ingredients
