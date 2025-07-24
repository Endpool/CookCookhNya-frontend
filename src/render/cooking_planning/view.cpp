#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "states.hpp"
#include "utils/u8format.hpp"
#include "utils/utils.hpp"

#include <boost/url/url.hpp>

#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::cooking_planning {

using namespace api::models::recipe;
using IngredientAvailability = states::CookingPlanning::IngredientAvailability;
using AvailabilityType = states::CookingPlanning::AvailabilityType;

namespace {

struct CookingInfo {
    std::string renderText;
    std::string recipeName;
    bool isIngredientNotAvailable;
    bool isIngredientInOtherStorages;
};

CookingInfo recipeView(const std::vector<IngredientAvailability>& inStoragesAvailability,
                       api::RecipeId recipeId,
                       UserId userId,
                       api::ApiClientRef api) {
    auto recipeIngredients = api.getRecipesApi().getSuggested(userId, recipeId);

    bool isIngredientNotAvailable = false;
    bool isIngredientIsOtherStorages = false;
    std::string& recipeName = recipeIngredients.name;
    auto text = std::format("{} *{}* \n\n", utils::utf8str(u8"📖 Ингредиенты для"), recipeName);

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

    return {.renderText = text,
            .recipeName = recipeName,
            .isIngredientNotAvailable = isIngredientNotAvailable,
            .isIngredientInOtherStorages = isIngredientIsOtherStorages};
}

} // namespace

void renderCookingPlanning(std::vector<IngredientAvailability>& inStoragesAvailability,
                           api::RecipeId recipeId,
                           UserId userId,
                           ChatId chatId,
                           BotRef bot,
                           api::ApiClientRef api) {
    auto cookingInfo = recipeView(inStoragesAvailability, recipeId, userId, api);
    InlineKeyboardBuilder keyboard{4}; // Cook + add storages, shopping list, share, back

    keyboard << makeCallbackButton(u8"🧑‍🍳 Готовить", "start_cooking");
    if (cookingInfo.isIngredientInOtherStorages)
        keyboard << makeCallbackButton(u8"?", "add_storages");
    keyboard << NewRow{};

    if (cookingInfo.isIngredientNotAvailable)
        keyboard << makeCallbackButton(u8"📝 Составить список продуктов", "shopping_list") << NewRow{};

    auto shareButton = std::make_shared<TgBot::InlineKeyboardButton>();
    shareButton->text = utils::utf8str(u8"📤 Поделиться");
    const std::string botAlias = bot.getUnderlying().getMe()->username;
    const std::string recipeUrl = std::format("https://t.me/{}?start=recipe_{}", botAlias, recipeId);
    const std::string shareText =
        utils::u8format("{} **{}**", u8"Хочу поделиться с тобой рецептом", cookingInfo.recipeName);

    boost::urls::url url{"https://t.me/share/url"};
    url.params().append({"url", recipeUrl});
    url.params().append({"text", shareText});
    shareButton->url = url.buffer();

    keyboard << std::move(shareButton) << NewRow{} << makeCallbackButton(u8"↩️ Назад", "back");

    auto messageId = message::getMessageId(userId);
    if (messageId) {
        // Only on difference between function above
        bot.editMessageText(cookingInfo.renderText, chatId, *messageId, std::move(keyboard), "Markdown");
    }
}

} // namespace cookcookhnya::render::cooking_planning
