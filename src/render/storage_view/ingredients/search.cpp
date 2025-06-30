#include "search.hpp"
#include "search_bot_patch.hpp"

#include "backend/models/ingredient.hpp"
#include "render/common.hpp"
#include "tg_types.hpp"
#include "utils.hpp"

#include <tgbot/types/InlineKeyboardButton.h>

#include <memory>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::storage::ingredients {

using namespace api::models::ingredient;
using namespace tg_types;

namespace {

auto makeKeyboard(const std::vector<IngredientSearchResult>& ingredients) {
    using namespace std::views;
    InlineKeyboard keyboard{2 + ingredients.size()};

    auto searchButton = std::make_shared<TgBot::InlineKeyboardButton>();
    searchButton->text = utils::utf8str(u8"Поиск");
    searchButton->switchInlineQueryCurrentChat = "";
    keyboard[0].push_back(std::move(searchButton));

    for (auto [row, ing] : zip(drop(keyboard, 1), ingredients))
        row.push_back(detail::makeCallbackButton((ing.available ? "[+] " : "[-] ") + ing.name, std::to_string(ing.id)));

    keyboard[1 + ingredients.size()].push_back(detail::makeCallbackButton(u8"Назад", "back"));
  
    return detail::makeKeyboardMarkup(std::move(keyboard));
}

} // namespace

MessageId renderStorageIngredientsSearchSend(ChatId chat, BotRef bot) {
    PatchedBot patchedBot{bot};
    return patchedBot
        .sendMessage(
            chat, utils::utf8str(u8"Используй кнопку ниже как поисковик чтобы найти ингредиент"), makeKeyboard({}))
        ->messageId;
}

void renderStorageIngredientsSearchEdit(const std::vector<IngredientSearchResult>& ingredients,
                                        MessageId message,
                                        ChatId chat,
                                        BotRef bot) {
    PatchedBot patchedBot{bot};
    patchedBot.editMessageReplyMarkup(chat, message, makeKeyboard(ingredients));
}

} // namespace cookcookhnya::render::storage::ingredients
