#include "list.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "render/common.hpp"
#include "tg_types.hpp"
#include "utils.hpp"

#include <format>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::storage::ingredients {

using namespace api::models::ingredient;

void renderIngredientsList(api::StorageId storage,
                           UserId userId,
                           ChatId chatId,
                           tg_types::MessageId messageId,
                           BotRef bot,
                           IngredientsApiRef api) {
    using namespace std::views;
    using std::ranges::to;

    std::vector<Ingredient> ingredients = api.getStorageIngredients(userId, storage);
    std::string list =
        ingredients | transform([](auto& i) { return std::format("- {}\n", i.name); }) | join | to<std::string>();

    InlineKeyboard keyboard{2};
    keyboard[0].push_back(detail::makeCallbackButton(u8"Добавить/Удалить", "search"));
    keyboard[1].push_back(detail::makeCallbackButton(u8"Назад", "back"));

    auto text = utils::utf8str(u8"🍗 Ваши ингредиенты:\n\n") + std::move(list);
    // auto messageId = message::getMessageId(userId);
    bot.editMessageText(text, chatId, messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::storage::ingredients
