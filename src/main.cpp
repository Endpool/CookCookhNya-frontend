#include "backend/api/api.hpp"
#include "handlers/handlers_list.hpp"
#include "states.hpp"
#include "utils/utils.hpp"

#include <tg_stater/bot.hpp>
#include <tg_stater/dependencies.hpp>

int main() {
    using namespace tg_stater;
    using namespace cookcookhnya;
    using namespace cookcookhnya::handlers::bot_handlers;
    using namespace cookcookhnya::states;
    using namespace cookcookhnya::api;

    Setup<State, Dependencies<ApiClient>>::Stater<startCmdHandler,
                                                  noStateHandler,
                                                  mainMenuCQHandler,
                                                  customIngredientsListCQHandler,
                                                  customIngredientCreationEnterNameMsgHandler,
                                                  customIngredientCreationEnterNameCQHandler,
                                                  customIngredientConfirmationCQHandler,
                                                  customIngredientPublishCQHandler,
                                                  storageListCQHandler,
                                                  storageCreationEnterNameMsgHandler,
                                                  storageCreationEnterNameCQHandler,
                                                  storageDeletionCQHandler,
                                                  storageViewCQHandler,
                                                  storageMemberViewCQHandler,
                                                  storageMemberAdditionMsgHandler,
                                                  storageMemberAdditionCQHandler,
                                                  storageMemberDeletionCQHandler,
                                                  storageSelectionCQHandler,
                                                  storageIngredientsListCQHandler,
                                                  storageIngredientsListIQHandler,
                                                  suggestedRecipeListCQHandler,
                                                  recipeViewCQHandler,
                                                  recipeStorageAdditionCQHandler,
                                                  shoppingListCreationCQHandler,
                                                  shoppingListViewCQHandler,
                                                  personalAccountMenuCQHandler,
                                                  customRecipesListCQHandler,
                                                  createCustomRecipeMsgHandler,
                                                  createCustomRecipeCQHandler,
                                                  recipeCustomViewCQHandler,
                                                  customRecipeIngredientsSearchCQHandler,
                                                  customRecipeIngredientsSearchIQHandler>
        bot{{}, {ApiClient{utils::getenvWithError("API_URL")}}};

    bot.start(TgBot::Bot{utils::getenvWithError("BOT_TOKEN")});
}
