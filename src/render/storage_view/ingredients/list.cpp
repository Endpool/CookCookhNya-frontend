#include "list.hpp"

#include "render/common.hpp"

#include <format>
#include <ranges>
#include <string>
#include <utility>

namespace cookcookhnya::render::storage::ingredients {

using namespace api::models::ingredient;

void renderIngredientsList(StorageId storage, UserId user, ChatId chat, BotRef bot, IngredientsApiRef api) {
    using namespace std::views;
    using std::ranges::to;

    std::vector<Ingredient> ingredients = api.getStorageIngredients(user, storage);
    std::string list =
        ingredients | transform([](auto& i) { return std::format("- {}\n", i.name); }) | join | to<std::string>();

    InlineKeyboard keyboard{2};
    keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"Добавить/Удалить"), "search"));
    keyboard[1].push_back(detail::makeCallbackButton(utils::utf8str(u8"Назад"), "back"));

    bot.sendMessage(chat,
                    utils::utf8str(u8"Ваши ингредиенты:\n\n") + std::move(list),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::storage::ingredients
