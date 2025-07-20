#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "states.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::recipe {

using namespace api::models::recipe;
using IngredientAvailability = states::RecipeView::IngredientAvailability;
using AvailabilityType = states::RecipeView::AvailabilityType;

textGenInfo recipeView(const std::vector<IngredientAvailability>& inStoragesAvailability,
                       api::RecipeId recipeId,
                       UserId userId,
                       ApiClient api) {
    auto recipeIngredients = api.getRecipesApi().get(userId, recipeId);

    bool isIngredientNotAvailable = false;
    bool isIngredientIsOtherStorages = false;
    std::string& recipeName = recipeIngredients.name;
    auto text = std::format("{} Ингредиенты для *{}* \n\n", utils::utf8str(u8"📖"), recipeName);

    for (const auto& availability : inStoragesAvailability) {
        if (availability.available == AvailabilityType::AVAILABLE) {
            text += "`[+]` " + availability.ingredient.name + "\n";
        } else if (availability.available == AvailabilityType::OTHER_STORAGES) {
            text += "`[?]` " + availability.ingredient.name + "\n";
            isIngredientIsOtherStorages = true;
        } else {
            text += "`[ ]` " + availability.ingredient.name + "\n";
            isIngredientNotAvailable = true;
        }
    }
    if (recipeIngredients.link)
        text += utils::utf8str(u8"\n🌐 Источник: ") + *recipeIngredients.link;

    return {.text = text,
            .isIngredientNotAvailable = isIngredientNotAvailable,
            .isIngredientIsOtherStorages = isIngredientIsOtherStorages};
}

void renderRecipeView(std::vector<IngredientAvailability>& inStoragesAvailability,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      ApiClient api) {
    auto textGen = recipeView(inStoragesAvailability, recipeId, userId, api);
    const std::size_t buttonRows = textGen.isIngredientNotAvailable ? 3 : 2;
    InlineKeyboard keyboard(buttonRows);

    keyboard[0].push_back(makeCallbackButton(u8"🧑‍🍳 Готовить", "start_cooking"));

    if (textGen.isIngredientIsOtherStorages) {
        keyboard[0].push_back(makeCallbackButton(u8"?", "add_storages"));
    }
    if (textGen.isIngredientNotAvailable) {
        keyboard[1].push_back(makeCallbackButton(u8"📝 Составить список продуктов", "shopping_list"));
    }

    keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"↩️ Назад", "back_from_recipe_view"));

    auto messageId = message::getMessageId(userId);
    if (messageId) {
        // Only on difference between function above
        bot.editMessageText(textGen.text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)), "Markdown");
    }
}

} // namespace cookcookhnya::render::recipe
