#include "view.hpp"

#include "backend/api/publicity_filter.hpp"
#include "backend/api/recipes.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "render/pagination.hpp"
#include "render/personal_account/recipes_list/view.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <memory>
#include <string>

namespace TgBot {
class InlineKeyboardMarkup;
} // namespace TgBot

namespace cookcookhnya::render::personal_account::recipes_list {

using namespace api::models::recipe;

namespace {

std::shared_ptr<TgBot::InlineKeyboardMarkup>
constructKeyboard(std::size_t pageNo, std::size_t pageSize, RecipesList& recipesList) {
    InlineKeyboardBuilder keyboard;
    keyboard << makeCallbackButton(u8"🆕 Создать", "custom_recipe_create") << NewRow{};
    auto makeRecipeButton = [i = (pageNo * pageSize) + 1](RecipeSummary& r) mutable {
        return makeCallbackButton(std::format("{}. {}", i++, r.name), "recipe_" + utils::to_string(r.id));
    };
    keyboard << constructPagination(pageNo, pageSize, recipesList.found, recipesList.page, makeRecipeButton)
             << makeCallbackButton(u8"↩️ Назад", "back");
    return keyboard;
}

} // namespace

void renderCustomRecipesList(
    std::size_t pageNo, UserId userId, ChatId chatId, BotRef bot, api::RecipesApiRef recipesApi) {
    const std::size_t numOfRecipesOnPage = 5;
    auto recipesList =
        recipesApi.getList(userId, PublicityFilterType::Custom, numOfRecipesOnPage, pageNo * numOfRecipesOnPage);

    const std::string pageInfo = utils::utf8str(
        recipesList.found > 0 ? u8"🔪 Рецепты созданные вами:"
                              : u8"🔪 Вы находитесь в Мои рецепты. Создавайте и делитесь новыми рецептами.\n\n");

    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(pageInfo, chatId, *messageId, constructKeyboard(pageNo, numOfRecipesOnPage, recipesList));
    }
}

} // namespace cookcookhnya::render::personal_account::recipes_list
