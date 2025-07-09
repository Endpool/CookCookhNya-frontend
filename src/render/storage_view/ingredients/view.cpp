#include "view.hpp"

#include "backend/models/ingredient.hpp"
#include "bot_patches.hpp"
#include "message_tracker.hpp"
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
        row.push_back(detail::makeCallbackButton((ing.available ? "[+] " : "[-] ") + ing.name, std::to_string(ing.id)));

    keyboard[1 + state.searchItems.size()].push_back(detail::makeCallbackButton(u8"↩️ Назад", "back"));

    return detail::makeKeyboardMarkup(std::move(keyboard));
}

} // namespace

void renderIngredientsListSearch(const states::StorageIngredientsList& state,
                                 UserId userId,
                                 ChatId chatId,
                                 BotRef bot) {
    using namespace std::views;
    using std::ranges::to;

    const PatchedBot patchedBot{bot};

    std::string list = state.getStorageIngredients() |
                       transform([](auto& i) { return std::format("• {}\n", i.name); }) | join | to<std::string>();

    auto text = utils::utf8str(u8"🍗 Ваши ингредиенты:\n\n") + std::move(list);
    if (auto messageId = message::getMessageId(userId))
        patchedBot.editMessageText(text, chatId, *messageId, makeKeyboard(state));
}

} // namespace cookcookhnya::render::storage::ingredients
