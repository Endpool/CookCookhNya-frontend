#include "view.hpp"

#include "backend/models/recipe.hpp"
#include "backend/models/storage.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "render/pagination.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <memory>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace TgBot {
class InlineKeyboardMarkup;
} // namespace TgBot

namespace cookcookhnya::render::recipes_suggestions {

using namespace api::models::storage;
using namespace api::models::recipe;
using namespace std::views;
using namespace std::ranges;

namespace {

std::shared_ptr<TgBot::InlineKeyboardMarkup>
constructKeyboard(std::size_t pageNo, std::size_t pageSize, RecipesListWithIngredientsCount& recipesList) {
    InlineKeyboardBuilder keyboard;
    auto makeRecipeButton = [i = (pageNo * pageSize) + 1](RecipeSummaryWithIngredients& r) mutable {
        return makeCallbackButton(std::format("{}. {} [{} из {}]", i++, r.name, r.available, r.total),
                                  std::format("recipe_{}", r.id));
    };
    keyboard << constructPagination(pageNo, pageSize, recipesList.found, recipesList.page, makeRecipeButton)
             << makeCallbackButton(u8"↩️ Назад", "back");
    return std::move(keyboard);
}

} // namespace

void renderRecipesSuggestion(std::vector<StorageSummary>& storages,
                             std::size_t pageNo,
                             UserId userId,
                             ChatId chatId,
                             BotRef bot,
                             RecipesApiRef recipesApi) {
    const std::size_t numOfRecipesOnPage = 5;
    const std::size_t numOfRecipes = 500;

    auto storagesIds = storages | views::transform(&StorageSummary::id) | to<std::vector>();
    auto recipesList = recipesApi.getSuggestedRecipes(userId, storagesIds, numOfRecipes, pageNo * numOfRecipesOnPage);

    std::string text =
        utils::utf8str(recipesList.found > 0 ? u8"🔪 Рецепты подобранные специально для вас"
                                             : u8"😔 К сожалению, нам не удалось найти подходящие рецепты для вас...");
    auto keyboardMarkup = constructKeyboard(pageNo, numOfRecipesOnPage, recipesList);

    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(text, chatId, *messageId, keyboardMarkup);
    } else {
        auto message = bot.sendMessage(chatId, text, keyboardMarkup);
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::recipes_suggestions
