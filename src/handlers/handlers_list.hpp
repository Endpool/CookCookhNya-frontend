#pragma once

// Handler callbacks
#include "initial/start.hpp"
#include "main_menu/view.hpp"
#include "personal_account/ingredients/create.hpp"
#include "personal_account/ingredients/publish.hpp"
#include "personal_account/ingredients/view.hpp"
#include "personal_account/view.hpp"
#include "recipes_suggestion/recipe/view.hpp"
#include "recipes_suggestion/storage_selection/select.hpp"
#include "recipes_suggestion/suggest.hpp"
#include "shopping_list/create.hpp"
#include "shopping_list/view.hpp"
#include "storage_list/create.hpp"
#include "storage_list/delete.hpp"
#include "storage_list/view.hpp"
#include "storage_view/ingredients/search.hpp"

#include "storage_view/ingredients/view.hpp"

#include "storage_view/members/add.hpp"
#include "storage_view/members/delete.hpp"
#include "storage_view/members/view.hpp"
#include "storage_view/view.hpp"

#include "handlers/common.hpp"

#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>
#include <tg_stater/handler/type.hpp>

namespace cookcookhnya::handlers {

using namespace init;
using namespace main_menu_view;
using namespace personal_account_view;
using namespace create_custom_ingredients;
using namespace publish_custom_ingredients;
using namespace custom_ingredients_view;
using namespace storage_create;
using namespace storage_delete;
using namespace storage_list_view;
using namespace storage_view;
using namespace storage_view_members;
using namespace storage_add_member;
using namespace storage_delete_member;
using namespace storage::ingredients;
using namespace storages_select;
using namespace recipes_suggestion;
using namespace recipe_view;
using namespace shopping_list_create;
using namespace shopping_list;

using namespace tg_stater;

namespace bot_handlers {

// Init
constexpr char startCmd[] = "start";                                        // NOLINT(*c-arrays)
using startHandler = Handler<Events::Command{startCmd}, start, AnyState{}>; // NOLINT(*decay)
using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, NoState{}>;

// MainMenu
using mainMenuButtonHandler = Handler<Events::CallbackQuery{}, mainMenuHandler>;

// PersonalAccount
using personalAccountButtonHandler = Handler<Events::CallbackQuery{}, personalAccountButtonHandler>;

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
using cancelMemberAdditionButtonHandler = Handler<Events::CallbackQuery{}, cancelMemberAddition>;

// StorageDeleteMembers
using memberDeletionButtonHandler = Handler<Events::CallbackQuery{}, deleteMember>;

// StorageSelection
using storagesSelectionHandler = Handler<Events::CallbackQuery{}, selectStorages>;

// SuggestedRecipeList
using recipesSuggestionListHandler = Handler<Events::CallbackQuery{}, changePageAndBack>;

// StorageIngredientsList
using storageIngredientsListButtonHandler = Handler<Events::CallbackQuery{}, storageIngredientsListButtonCallback>;
using storageIngredientsSearchButtonHandler = Handler<Events::CallbackQuery{}, storageIngredientsSearchButtonCallback>;
using storageIngredientsSearchHandler = Handler<Events::InlineQuery{}, storageIngredientsSearchInlineQueryCallback>;

// RecipeView
using recipeViewHandler = Handler<Events::CallbackQuery{}, handleRecipeView>;

// ShoppingListCreation
using shoppingListCreationHandler = Handler<Events::CallbackQuery{}, handleProductListSubmission>;

// Shopping list
using shoppingListButtonHandler = Handler<Events::CallbackQuery{}, shoppingListButtonCallback>;

} // namespace bot_handlers

} // namespace cookcookhnya::handlers
