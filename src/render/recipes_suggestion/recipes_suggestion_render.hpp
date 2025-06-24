#pragma once

#include "render/common.hpp"
#include "tg_types.hpp"

namespace cookcookhnya::render::recipes_suggestion {

/*
@brief shared function between render and edit of suggestion message
*/
InlineKeyboard
constructMarkup(std::vector<StorageId> const& storages, int pageNo, UserId userId, RecipesApiRef recipesApi);

void renderRecipesSuggestion(std::vector<StorageId> const& storages,
                             int pageNo,
                             UserId userId,
                             ChatId chatId,
                             BotRef bot,
                             RecipesApiRef recipesApi);

void editSuggestionMessage(std::vector<StorageId> const& storages,
                           int pageNo,
                           UserId userId,
                           ChatId chatId,
                           tg_types::MessageId messageId,
                           BotRef bot,
                           RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::recipes_suggestion
