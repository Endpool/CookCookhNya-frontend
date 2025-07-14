#include "search_ingredients.hpp"

#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "patched_bot.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <tgbot/types/InlineKeyboardButton.h>

#include <cstddef>
#include <memory>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::recipe::ingredients {

using namespace api::models::ingredient;
using namespace tg_types;

namespace {

auto makeKeyboard(const std::vector<IngredientSearchForRecipeItem>& ingredients,
                  std::size_t /*pageNo*/,
                  std::size_t /*totalPages*/) {
    using namespace std::views;
    InlineKeyboard keyboard{2 + ingredients.size()};

    auto searchButton = std::make_shared<TgBot::InlineKeyboardButton>();
    searchButton->text = utils::utf8str(u8"Поиск");
    searchButton->switchInlineQueryCurrentChat = "";
    keyboard[0].push_back(std::move(searchButton));

    for (auto [row, ing] : zip(std::ranges::views::drop(keyboard, 1), ingredients))
        row.push_back(makeCallbackButton((ing.available ? "[+] " : "[-] ") + ing.name, utils::to_string(ing.id)));

    keyboard[1 + ingredients.size()].push_back(makeCallbackButton(u8"↩️ Назад", "back"));

    return makeKeyboardMarkup(std::move(keyboard));
}

} // namespace

void renderRecipeIngredientsSearchEdit(const std::vector<IngredientSearchForRecipeItem>& ingredients,
                                       std::size_t pageNo,
                                       std::size_t totalPages,
                                       MessageId message,
                                       ChatId chatId,
                                       BotRef bot) {
    const PatchedBot& patchedBot{bot};
    patchedBot.editMessageReplyMarkup(chatId, message, makeKeyboard(ingredients, pageNo, totalPages));
}

void renderStorageIngredientsSearch(ChatId chatId, UserId userId, BotRef bot) {
    const PatchedBot& patchedBot{bot};
    if (auto mMessageId = message::getMessageId(userId)) {
        patchedBot.editMessageText(utils::utf8str(u8"Используй кнопку ниже как поисковик чтобы найти ингредиент"),
                                   chatId,
                                   *mMessageId,
                                   makeKeyboard({}, 0, 0));
    }
}

} // namespace cookcookhnya::render::recipe::ingredients
