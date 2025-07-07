#pragma once

// Handler callbacks
#include "initial/initial_start.hpp"
#include "main_menu/main_menu_view.hpp"
#include "recipes_suggestion/recipe_view.hpp"
#include "recipes_suggestion/recipes_suggestion_list.hpp"
#include "recipes_suggestion/shopping_list_creation.hpp"
#include "recipes_suggestion/storages_select.hpp"
#include "shopping_list/list.hpp"
#include "storage_list/storage_list_create.hpp"
#include "storage_list/storage_list_delete.hpp"
#include "storage_list/storage_list_view.hpp"
#include "storage_view/ingredients/list.hpp"
#include "storage_view/ingredients/search.hpp"
#include "storage_view/storage_add_member.hpp"
#include "storage_view/storage_delete_member.hpp"
#include "storage_view/storage_view.hpp"
#include "storage_view/storage_view_members.hpp"

#include "handlers/common.hpp"

#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>
#include <tg_stater/handler/type.hpp>

namespace cookcookhnya::handlers {

using namespace init;
using namespace main_menu_view;
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
using namespace shopping_list_creation;
using namespace shopping_list;

using namespace tg_stater;

namespace bot_handlers {

// Init
constexpr char startCmd[] = "start";                                        // NOLINT(*c-arrays)
using startHandler = Handler<Events::Command{startCmd}, start, AnyState{}>; // NOLINT(*decay)
using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, NoState{}>;

// MainMenu
using mainMenuButtonHandler = Handler<Events::CallbackQuery{}, mainMenuHandler>;

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
