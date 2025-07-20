#include "add_storage.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "backend/models/storage.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "states.hpp"
#include "utils/utils.hpp"
#include "view.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::recipe {

using namespace api::models::recipe;
using namespace api::models::storage;
using IngredientAvailability = states::RecipeView::IngredientAvailability;
using AvailabilityType = states::RecipeView::AvailabilityType;

textGenInfo storageAdditionView(const std::vector<IngredientAvailability>& inStoragesAvailability,
                                const std::vector<StorageSummary>& selectedStorages,
                                api::RecipeId recipeId,
                                UserId userId,
                                ApiClient api) {
    auto recipe = api.getRecipesApi().get(userId, recipeId);

    bool isIngredientNotAvailable = false;
    bool isIngredientIsOtherStorages = false;
    const std::string recipeName = recipe.name;
    auto text = std::format("{} Выбранные хранилища: ", utils::utf8str(u8"🍱"));
    for (std::size_t i = 0; i != selectedStorages.size(); ++i) {
        text += selectedStorages[i].name;
        text += i != selectedStorages.size() - 1 ? ", " : "\n";
    }
    text += std::format("{} Ингредиенты для *{}* \n\n", utils::utf8str(u8"📖"), recipeName);

    for (const auto& infoPair : inStoragesAvailability) {
        if (infoPair.available == AvailabilityType::AVAILABLE) {
            text += "`[+]` " + infoPair.ingredient.name + "\n";
        } else if (infoPair.available == AvailabilityType::OTHER_STORAGES) {
            text += "`[?]` " + infoPair.ingredient.name + "\n";
            isIngredientIsOtherStorages = true;
            text += "Доступно в: ";
            auto storages = infoPair.storages;
            for (std::size_t i = 0; i != storages.size(); ++i) {
                text += storages[i].name;
                text += i != storages.size() - 1 ? ", " : "\n";
            }
        } else {
            text += "`[ ]` " + infoPair.ingredient.name + "\n";
            isIngredientNotAvailable = true;
        }
    }
    if (recipe.link)
        text += utils::utf8str(u8"\n🌐 Источник: ") + *recipe.link;

    return {.text = text,
            .isIngredientNotAvailable = isIngredientNotAvailable,
            .isIngredientIsOtherStorages = isIngredientIsOtherStorages};
}

void renderStoragesSuggestion(const std::vector<IngredientAvailability>& inStoragesAvailability,
                              const std::vector<StorageSummary>& selectedStorages,
                              const std::vector<StorageSummary>& addedStorages,
                              api::RecipeId recipeId,
                              UserId userId,
                              ChatId chatId,
                              BotRef bot,
                              ApiClient api) {
    auto textGen = storageAdditionView(inStoragesAvailability, selectedStorages, recipeId, userId, api);
    std::vector<StorageSummary> storages;
    for (const auto& infoPair : inStoragesAvailability) {
        if (infoPair.available == AvailabilityType::OTHER_STORAGES) {
            for (const auto& storage : infoPair.storages) {
                if (std::ranges::find(storages, storage.id, &StorageSummary::id) == storages.end()) {
                    storages.push_back(storage);
                }
            }
        }
    }
    storages.insert(storages.end(), addedStorages.begin(), addedStorages.end());
    const size_t buttonRows = ((storages.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);

    for (std::size_t i = 0; i < storages.size(); ++i) {
        if (i % 2 == 0)
            keyboard[i / 2].reserve(2);
        const bool isSelected =
            std::ranges::find(addedStorages, storages[i].id, &StorageSummary::id) != addedStorages.end();

        std::string emoji = utils::utf8str(isSelected ? u8"[ + ]" : u8"[ᅠ]");
        const char* actionPrefix = isSelected ? "+" : "-";
        const std::string text = std::format("{} {}", emoji, storages[i].name);
        const std::string data = actionPrefix + utils::to_string(storages[i].id);
        keyboard[i / 2].push_back(makeCallbackButton(text, data));
    }
    if (!storages.empty()) {
        keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    }

    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(textGen.text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)), "Markdown");
    }
}
} // namespace cookcookhnya::render::recipe
