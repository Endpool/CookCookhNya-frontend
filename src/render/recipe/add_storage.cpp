#include "add_storage.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "backend/models/storage.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"
#include "view.hpp"

#include <cmath>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::recipe {

using namespace handlers::recipes_suggestions;

textGenInfo storageAdditionView(const std::vector<std::pair<api::models::recipe::IngredientInRecipe, handlers::recipes_suggestions::IngredientAvailability>>& inStoragesAvailability,
                    const std::vector<api::models::storage::StorageSummary>& selectedStorages,
                    api::RecipeId recipeId,
                    UserId userId,
                    ApiClient api){
    auto recipeIngredients = api.getRecipesApi().getIngredientsInRecipe(userId, recipeId);

    bool isIngredientNotAvailable = false;
    bool isIngredientIsOtherStorages = false;
    const std::string recipeName = recipeIngredients.name;
    auto text = std::format("{} Выбранные хранилища: ", utils::utf8str(u8"🍱"));
            for (std::size_t i = 0; i != selectedStorages.size(); ++i){
                text += selectedStorages[i].name;
                text += i != selectedStorages.size() - 1 ? ", " : "\n";
            }
    text += std::format("{} Ингредиенты для *{}* \n\n", utils::utf8str(u8"📖"), recipeName);

    for (const auto& infoPair : inStoragesAvailability){
        if (infoPair.second.available == AvailabiltiyType::available){
            text += "`[+]` " + infoPair.first.name + "\n";
        } else if (infoPair.second.available == AvailabiltiyType::other_storages){
            text += "`[?]` " + infoPair.first.name + "\n";
            isIngredientIsOtherStorages = true;
            text += "__Доступно в: ";
            auto storages = infoPair.second.storages;
            for (std::size_t i = 0; i != storages.size(); ++i){
                text += storages[i].name;
                text += i != storages.size() - 1 ? ", " : "\n";
            }
        } else {
            text += "`[ ]` " + infoPair.first.name + "\n";
            isIngredientNotAvailable = true;
        }
    }
    text += "\n🌐 Источник: " + recipeIngredients.link;

    return {.text = text, .isIngredientNotAvailable = isIngredientNotAvailable, .isIngredientIsOtherStorages = isIngredientIsOtherStorages};
}

void renderStoragesSuggestion(const std::vector<std::pair<api::models::recipe::IngredientInRecipe, handlers::recipes_suggestions::IngredientAvailability>>& inStoragesAvailability,
                      const std::vector<api::models::storage::StorageSummary>& selectedStorages,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      ApiClient api) {
    auto textGen = storageAdditionView(inStoragesAvailability, selectedStorages, recipeId, userId, api);
    auto storages = api.getStoragesApi().getStoragesList(userId);
    const size_t buttonRows = ((storages.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);
    
    for (std::size_t i = 0; i < storages.size(); ++i) {
        if (i % 2 == 0)
            keyboard[i / 2].reserve(2);
        const bool isSelected = std::ranges::find(selectedStorages, storages[i].id, &api::models::storage::StorageSummary::id) != selectedStorages.end();

        std::string emoji = utils::utf8str(isSelected ? u8"[ + ]" : u8"[ᅠ]");
        const char* actionPrefix = isSelected ? "in__" : "out_";
        const std::string text = std::format("{} {}", emoji, storages[i].name);
        const std::string data = actionPrefix + utils::to_string(storages[i].id);
        keyboard[i / 2].push_back(makeCallbackButton(text, data));
    }
    keyboard[buttonRows - 1].reserve(2);
    keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"↩️ Назад", "cancel"));
    if (!selectedStorages.empty()) {
        keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"▶️ Подтвердить", "confirm"));
    }
    
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        // Only on difference between function above
        bot.editMessageText(textGen.text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)), "MarkdownV2");
    }
}
} // namespace cookcookhnya::render::recipe
