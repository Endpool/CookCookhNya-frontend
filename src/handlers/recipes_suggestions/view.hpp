#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::recipes_suggestions {

/*
 * @brief  Back: check the number of storages passed to state.
 * If more then one then return to storage list choose if one then go to the storage view.
 * Even if one storage was chosen in storage list choose it will return to view of these one storage.
 *
 * ChangePage: move to next of prev page checking if page is valid
 */
void handleSuggestedRecipeListCQ(
    SuggestedRecipeList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::recipes_suggestions
