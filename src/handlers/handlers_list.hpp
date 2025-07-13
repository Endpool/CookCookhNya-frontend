#pragma once

// Handler callbacks
#include "initial/start.hpp"

#include "main_menu/view.hpp"

#include "personal_account/ingredients_list/create.hpp"
#include "personal_account/ingredients_list/publish.hpp"
#include "personal_account/ingredients_list/view.hpp"

#include "personal_account/recipes_list/create.hpp"
#include "personal_account/recipes_list/recipe/search_ingredients.hpp"
#include "personal_account/recipes_list/recipe/view.hpp"
#include "personal_account/recipes_list/view.hpp"

#include "personal_account/view.hpp"

#include "recipe/add_storage.hpp"
#include "recipe/view.hpp"

#include "recipes_suggestions/view.hpp"

#include "shopping_list/create.hpp"
#include "shopping_list/view.hpp"

#include "storage/ingredients/view.hpp"

#include "storage/members/add.hpp"
#include "storage/members/delete.hpp"
#include "storage/members/view.hpp"

#include "storage/view.hpp"

#include "storages_list/create.hpp"
#include "storages_list/delete.hpp"
#include "storages_list/view.hpp"

#include "storages_selection/view.hpp"

#include "handlers/common.hpp"

#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>
#include <tg_stater/handler/type.hpp>

namespace cookcookhnya::handlers {

using namespace initial;
using namespace main_menu;
using namespace personal_account;
using namespace personal_account::ingredients;
using namespace personal_account::recipes;
using namespace recipe;
using namespace recipes_suggestions;
using namespace shopping_list;
using namespace storage;
using namespace storage::ingredients;
using namespace storage::members;
using namespace storages_list;
using namespace storages_selection;

using namespace tg_stater;

namespace bot_handlers {

// Init
constexpr char startCmd[] = "start";                                        // NOLINT(*c-arrays)
using startHandler = Handler<Events::Command{startCmd}, start, AnyState{}>; // NOLINT(*decay)
using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, NoState{}>;

// MainMenu
using mainMenuButtonHandler = Handler<Events::CallbackQuery{}, mainMenuHandler>;

// // CustomIngredient
using customIngredientsListButtonHandler = Handler<Events::CallbackQuery{}, customIngredientsList>;
using customIngredientCreateHandler = Handler<Events::Message{}, customIngredientEnterName>;
using customIngredientCancelCreationHandler = Handler<Events::CallbackQuery{}, customIngredientCancelCreation>;
using customIngredientConfirmHandler = Handler<Events::CallbackQuery{}, customIngredientConfirmation>;
using customIngredientPublishHandler = Handler<Events::CallbackQuery{}, customIngredientPublish>;

// StorageListCreate
using storageCreateHandler = Handler<Events::Message{}, createStorage>;
using storageCreateButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageCreation>;

// StorageListDelete
using storageDeleteButtonHandler = Handler<Events::CallbackQuery{}, deleteStorage>;

// StorageListView
using storageListButtonHandler = Handler<Events::CallbackQuery{}, storageListButtonCallback>;

// StorageView
using storageViewButtonHandler = Handler<Events::CallbackQuery{}, storageViewButtonCallback>;

// StorageViewMembers
using storageMemberViewButtonHandler = Handler<Events::CallbackQuery{}, storageMemberViewButtonCallback>;

// StorageAddMembers
using memberAdditionMessageHandler = Handler<Events::Message{}, addMember>;
using cancelStorageMemberAdditionButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageMemberAddition>;

// StorageDeleteMembers
using memberDeletionButtonHandler = Handler<Events::CallbackQuery{}, deleteMember>;

// StorageSelection
using storagesSelectionHandler = Handler<Events::CallbackQuery{}, selectStorages>;

// SuggestedRecipeList
using recipesSuggestionListHandler = Handler<Events::CallbackQuery{}, changePageAndBack>;

// StorageIngredientsList
using storageIngredientsSearchButtonHandler = Handler<Events::CallbackQuery{}, storageIngredientsSearchButtonCallback>;
using storageIngredientsSearchHandler = Handler<Events::InlineQuery{}, storageIngredientsSearchInlineQueryCallback>;

// RecipeView
using recipeViewHandler = Handler<Events::CallbackQuery{}, handleRecipeView>;

// Recipe Add Storage
using recipeAddStorageHandler = Handler<Events::CallbackQuery{}, handleRecipeAddStorage>;

// ShoppingListCreation
using shoppingListCreationHandler = Handler<Events::CallbackQuery{}, handleProductListSubmission>;

// Shopping list
using shoppingListButtonHandler = Handler<Events::CallbackQuery{}, shoppingListButtonCallback>;

// Personal account
using personalAccountButtonHandler = Handler<Events::CallbackQuery{}, personalAccount>;

// Custom Recipes List
using customRecipesListHandler = Handler<Events::CallbackQuery{}, customRecipeList>;

// Custom Recipe Create
using customRecipeCreateHandler = Handler<Events::Message{}, createRecipe>;
using customRecipeCreateButtonHandler = Handler<Events::CallbackQuery{}, cancelRecipeCreation>;

// Custom Recipe View
using customRecipeViewHandler = Handler<Events::CallbackQuery{}, handleCustomRecipeView>;

// Custom Recipe Ingredients Search
using customRecipeIngredientsSearchButtonHandler =
    Handler<Events::CallbackQuery{}, customRecipeIngredientsSearchButtonCallback>;
using customRecipeIngredientsSearchHandler =
    Handler<Events::InlineQuery{}, customRecipeIngredientsSearchInlineQueryCallback>;
} // namespace bot_handlers

} // namespace cookcookhnya::handlers
