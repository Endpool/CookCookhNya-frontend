#pragma once

// Handler callbacks
#include "commands/my_storages.hpp"
#include "commands/personal_account.hpp"
#include "commands/shopping_list.hpp"
#include "commands/start.hpp"
#include "commands/wanna_eat.hpp"

#include "main_menu/view.hpp"

#include "personal_account/ingredients_list/create.hpp"
#include "personal_account/ingredients_list/publish.hpp"
#include "personal_account/ingredients_list/view.hpp"

#include "personal_account/recipe/moderation_history.hpp"
#include "personal_account/recipe/search_ingredients.hpp"
#include "personal_account/recipe/view.hpp"

#include "personal_account/recipes_list/create.hpp"
#include "personal_account/recipes_list/view.hpp"

#include "personal_account/publication_history.hpp"
#include "personal_account/view.hpp"

#include "recipe/add_storage.hpp"
#include "recipe/view.hpp"

#include "recipes_suggestions/view.hpp"

#include "shopping_list/create.hpp"
#include "shopping_list/search.hpp"
#include "shopping_list/storage_selection_to_buy.hpp"
#include "shopping_list/view.hpp"

#include "storage/ingredients/view.hpp"

#include "storage/members/add.hpp"
#include "storage/members/delete.hpp"
#include "storage/members/view.hpp"

#include "storage/delete.hpp"
#include "storage/view.hpp"

#include "storages_list/create.hpp"
#include "storages_list/view.hpp"

#include "storages_selection/view.hpp"

#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>

namespace cookcookhnya::handlers::bot_handlers {

using namespace handlers::commands;
using namespace handlers::main_menu;
using namespace handlers::personal_account;
using namespace handlers::personal_account::ingredients;
using namespace handlers::personal_account::recipe;
using namespace handlers::personal_account::recipes_list;
using namespace handlers::shopping_list;
using namespace handlers::recipe;
using namespace handlers::storage;
using namespace handlers::storage::ingredients;
using namespace handlers::storage::members;
using namespace handlers::storages_list;
using namespace handlers::storages_selection;
using namespace handlers::recipes_suggestions;

using namespace tg_stater;

using NoState = tg_stater::HandlerTypes::NoState;
using AnyState = tg_stater::HandlerTypes::AnyState;

using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, NoState{}>;

// Commands
constexpr char startCmd[] = "start";                                                    // NOLINT(*c-arrays)
using startCmdHandler = Handler<Events::Command{startCmd}, handleStartCmd, AnyState{}>; // NOLINT(*decay)

constexpr char myStoragesCmd[] = "my_storages"; // NOLINT(*c-arrays)
using myStoragesCmdHandler = Handler<Events::Command{myStoragesCmd}, handleMyStoragesCmd, AnyState{}>; // NOLINT(*decay)

constexpr char shoppingListCmd[] = "shopping_list"; // NOLINT(*c-arrays)
using shoppingListCmdHandler =
    Handler<Events::Command{shoppingListCmd}, handleShoppingListCmd, AnyState{}>; // NOLINT(*decay)

constexpr char personalAccountCmd[] = "personal_account"; // NOLINT(*c-arrays)
using personalAccountCmdHandler =
    Handler<Events::Command{personalAccountCmd}, handlePersonalAccountCmd, AnyState{}>; // NOLINT(*decay)

constexpr char wannaEatCmd[] = "wanna_eat";                                                      // NOLINT(*c-arrays)
using wannaEatCmdHandler = Handler<Events::Command{wannaEatCmd}, handleWannaEatCmd, AnyState{}>; // NOLINT(*decay)

// MainMenu
using mainMenuCQHandler = Handler<Events::CallbackQuery{}, handleMainMenuCQ>;

// CustomIngredient
using customIngredientsListCQHandler = Handler<Events::CallbackQuery{}, handleCustomIngredientsListCQ>;
using customIngredientCreationEnterNameMsgHandler =
    Handler<Events::Message{}, handleCustomIngredientCreationEnterNameMsg>;
using customIngredientCreationEnterNameCQHandler =
    Handler<Events::CallbackQuery{}, handleCustomIngredientCreationEnterNameCQ>;
using customIngredientConfirmationCQHandler = Handler<Events::CallbackQuery{}, handleCustomIngredientConfirmationCQ>;
using customIngredientPublishCQHandler = Handler<Events::CallbackQuery{}, handleCustomIngredientPublishCQ>;

// StorageListCreate
using storageListCQHandler = Handler<Events::CallbackQuery{}, handleStorageListCQ>;
using storageCreationEnterNameMsgHandler = Handler<Events::Message{}, handleStorageCreationEnterNameMsg>;
using storageCreationEnterNameCQHandler = Handler<Events::CallbackQuery{}, handleStorageCreationEnterNameCQ>;

// StorageView
using storageViewCQHandler = Handler<Events::CallbackQuery{}, handleStorageViewCQ>;
using storageDeletionCQHandler = Handler<Events::CallbackQuery{}, handleStorageDeletionCQ>;

// StorageViewMembers
using storageMemberViewCQHandler = Handler<Events::CallbackQuery{}, handleStorageMemberViewCQ>;
using storageMemberAdditionMsgHandler = Handler<Events::Message{}, handleStorageMemberAdditionMsg>;
using storageMemberAdditionCQHandler = Handler<Events::CallbackQuery{}, handleStorageMemberAdditionCQ>;
using storageMemberDeletionCQHandler = Handler<Events::CallbackQuery{}, handleStorageMemberDeletionCQ>;

// StoragesSelection
using storageSelectionCQHandler = Handler<Events::CallbackQuery{}, handleStoragesSelectionCQ>;

// SuggestedRecipesList
using suggestedRecipeListCQHandler = Handler<Events::CallbackQuery{}, handleSuggestedRecipesListCQ>;

// StorageIngredientsList
using storageIngredientsListCQHandler = Handler<Events::CallbackQuery{}, handleStorageIngredientsListCQ>;
using storageIngredientsListIQHandler = Handler<Events::InlineQuery{}, handleStorageIngredientsListIQ>;

// RecipeView
using recipeViewCQHandler = Handler<Events::CallbackQuery{}, handleRecipeViewCQ>;
using recipeStorageAdditionCQHandler = Handler<Events::CallbackQuery{}, handleRecipeStorageAdditionCQ>;
using shoppingListCreationCQHandler = Handler<Events::CallbackQuery{}, handleShoppingListCreationCQ>;

// Shopping list
using shoppingListViewCQHandler = Handler<Events::CallbackQuery{}, handleShoppingListViewCQ>;
using shoppingListStorageSelectionToBuyCQHandler =
    Handler<Events::CallbackQuery{}, handleShoppingListStorageSelectionToBuyCQ>;
using shoppingListIngredientSearchCQHandler = Handler<Events::CallbackQuery{}, handleShoppingListIngredientSearchCQ>;
using shoppingListIngredientSearchIQHandler = Handler<Events::InlineQuery{}, handleShoppingListIngredientSearchIQ>;

// Personal account
using personalAccountMenuCQHandler = Handler<Events::CallbackQuery{}, handlePersonalAccountMenuCQ>;
using totalPublicationHistoryCQHandler = Handler<Events::CallbackQuery{}, handleTotalPublicationHistoryCQ>;

// Custom Recipes List
using customRecipesListCQHandler = Handler<Events::CallbackQuery{}, handleCustomRecipesListCQ>;
using createCustomRecipeMsgHandler = Handler<Events::Message{}, handleCreateCustomRecipeMsg>;
using createCustomRecipeCQHandler = Handler<Events::CallbackQuery{}, handleCreateCustomRecipeCQ>;

// Custom Recipe View
using recipeCustomViewCQHandler = Handler<Events::CallbackQuery{}, handleRecipeCustomViewCQ>;
using customRecipeIngredientsSearchCQHandler = Handler<Events::CallbackQuery{}, handleCustomRecipeIngredientsSearchCQ>;
using customRecipeIngredientsSearchIQHandler = Handler<Events::InlineQuery{}, handleCustomRecipeIngredientsSearchIQ>;
using customRecipePublicationHistoryCQHandler =
    Handler<Events::CallbackQuery{}, handleCustomRecipePublicationHistoryCQ>;

} // namespace cookcookhnya::handlers::bot_handlers
