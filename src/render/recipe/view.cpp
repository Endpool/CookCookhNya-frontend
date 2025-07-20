#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/ingredients_availability.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::recipe {

using namespace api::models::recipe;

textGenInfo
recipeView(const std::vector<std::pair<IngredientInRecipe, utils::IngredientAvailability>>& inStoragesAvailability,
           api::RecipeId recipeId,
           UserId userId,
           ApiClient api) {
    auto recipeIngredients = api.getRecipesApi().get(userId, recipeId);

    bool isIngredientNotAvailable = false;
    bool isIngredientIsOtherStorages = false;
    std::string& recipeName = recipeIngredients.name;
    auto text = std::format("{} Ингредиенты для *{}* \n\n", utils::utf8str(u8"📖"), recipeName);

    for (const auto& infoPair : inStoragesAvailability) {
        if (infoPair.second.available == utils::AvailabiltiyType::AVAILABLE) {
            text += "`[+]` " + infoPair.first.name + "\n";
        } else if (infoPair.second.available == utils::AvailabiltiyType::OTHER_STORAGES) {
            text += "`[?]` " + infoPair.first.name + "\n";
            isIngredientIsOtherStorages = true;
        } else {
            text += "`[ ]` " + infoPair.first.name + "\n";
            isIngredientNotAvailable = true;
        }
    }
    if (recipeIngredients.link)
        text += utils::utf8str(u8"\n🌐 Источник: ") + *recipeIngredients.link;

    return {.text = text,
            .isIngredientNotAvailable = isIngredientNotAvailable,
            .isIngredientIsOtherStorages = isIngredientIsOtherStorages};
}

void renderRecipeView(std::vector<std::pair<IngredientInRecipe, utils::IngredientAvailability>>& inStoragesAvailability,
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
        bot.editMessageText(textGen.text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)), "MarkdownV2");
    }
}

} // namespace cookcookhnya::render::recipe
