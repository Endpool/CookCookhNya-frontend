#include "view.hpp"

#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "patched_bot.hpp"
#include "render/common.hpp"
#include "states.hpp"
#include "utils.hpp"

#include <tgbot/types/InlineKeyboardButton.h>

#include <format>
#include <memory>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::storage::ingredients {

using namespace api::models::ingredient;
using namespace tg_types;

namespace {

auto makeKeyboard(const states::StorageIngredientsList& state) {
    using namespace std::views;
    InlineKeyboard keyboard{2 + state.searchItems.size()};

    auto searchButton = std::make_shared<TgBot::InlineKeyboardButton>();
    searchButton->text = utils::utf8str(u8"✏️ Редактировать");
    searchButton->switchInlineQueryCurrentChat = "";
    keyboard[0].push_back(std::move(searchButton));

    for (auto [row, ing] : zip(drop(keyboard, 1), state.searchItems))
        row.push_back(makeCallbackButton((ing.available ? "[ + ] " : "[ㅤ] ") + ing.name, std::to_string(ing.id)));
    keyboard[1 + state.searchItems.size()].push_back(makeCallbackButton(u8"↩️ Назад", "back"));

    return makeKeyboardMarkup(std::move(keyboard));
}

} // namespace

void renderIngredientsListSearch(const states::StorageIngredientsList& state,
                                 UserId userId,
                                 ChatId chatId,
                                 BotRef bot) {
    using namespace std::views;
    using std::ranges::to;

    std::string list = state.storageIngredients.getAll() |
                       transform([](auto& i) { return std::format("• {}\n", i.name); }) | join | to<std::string>();

    auto text = utils::utf8str(u8"🍗 Ваши ингредиенты:\n\n") + std::move(list);
    if (auto messageId = message::getMessageId(userId))
        bot.editMessageText(text, chatId, *messageId, makeKeyboard(state));
}

} // namespace cookcookhnya::render::storage::ingredients
