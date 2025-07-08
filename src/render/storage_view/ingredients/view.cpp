#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "search_bot_patch.hpp"
#include "utils.hpp"

#include <tgbot/types/InlineKeyboardButton.h>

#include <cstddef>
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

auto makeKeyboard(const std::vector<IngredientSearchItem>& ingredients,
                  std::size_t /*pageNo*/,
                  std::size_t /*totalPages*/) {
    using namespace std::views;
    InlineKeyboard keyboard{2 + ingredients.size()};

    auto searchButton = std::make_shared<TgBot::InlineKeyboardButton>();
    searchButton->text = utils::utf8str(u8"✏️ Редактировать");
    searchButton->switchInlineQueryCurrentChat = "";
    keyboard[0].push_back(std::move(searchButton));

    for (auto [row, ing] : zip(drop(keyboard, 1), ingredients))
        row.push_back(detail::makeCallbackButton((ing.available ? "[+] " : "[-] ") + ing.name, std::to_string(ing.id)));

    keyboard[1 + ingredients.size()].push_back(detail::makeCallbackButton(u8"↩️ Назад", "back"));

    return detail::makeKeyboardMarkup(std::move(keyboard));
}

} // namespace

void renderIngredientsListSearch(
    api::StorageId storage, UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api) {
    using namespace std::views;
    using std::ranges::to;

    const PatchedBot patchedBot{bot};

    std::vector<Ingredient> ingredients = api.getStorageIngredients(userId, storage);
    std::string list =
        ingredients | transform([](auto& i) { return std::format("- {}\n", i.name); }) | join | to<std::string>();

    auto text = utils::utf8str(u8"🍗 Ваши ингредиенты:\n\n") + std::move(list);
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        patchedBot.editMessageText(text, chatId, *messageId, makeKeyboard({}, 0, 0));
    }
}

void renderStorageIngredientsSearchEdit(const std::vector<IngredientSearchItem>& ingredients,
                                        std::size_t pageNo,
                                        std::size_t totalPages,
                                        MessageId message,
                                        ChatId chatId,
                                        BotRef bot) {
    const PatchedBot patchedBot{bot};
    patchedBot.editMessageReplyMarkup(chatId, message, makeKeyboard(ingredients, pageNo, totalPages));
}

} // namespace cookcookhnya::render::storage::ingredients
