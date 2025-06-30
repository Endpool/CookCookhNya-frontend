#pragma once

// Handler callbacks
#include "initial/initial_start.hpp"
#include "recipes_suggestion/recipe_view.hpp"
#include "recipes_suggestion/recipes_suggestion_list.hpp"
#include "recipes_suggestion/storages_select.hpp"
#include "storage_list/storage_list_create.hpp"
#include "storage_list/storage_list_delete.hpp"
#include "storage_list/storage_list_view.hpp"
#include "storage_view/ingredients/list.hpp"
#include "storage_view/ingredients/search.hpp"
#include "storage_view/storage_add_delete_members.hpp"
#include "storage_view/storage_view.hpp"
#include "storage_view/storage_view_members.hpp"

#include "handlers/common.hpp"

#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>

namespace cookcookhnya::handlers {

using namespace init;
using namespace storage_create;
using namespace storage_delete;
using namespace storage_list_view;
using namespace storage_view;
using namespace storage_view_members;
using namespace storage_add_delete_members;
using namespace storage::ingredients;
using namespace storages_select;
using namespace recipes_suggestion;
using namespace recipe_view;

using namespace tg_stater;

namespace bot_handlers {

// Init
constexpr char startCmd[] = "start";                                        // NOLINT(*c-arrays)
using startHandler = Handler<Events::Command{startCmd}, start, AnyState{}>; // NOLINT(*decay)
using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, NoState{}>;

// StorageListCreate
using storageCreateHandler = Handler<Events::Message{}, createStorage>;
using storageCreateButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageCreation>;

// StorageListDelete
using storgeDeleteHandler = Handler<Events::CallbackQuery{}, deleteStorage>;
using storageDeleteButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageDeletion>;

// StorageListView
using storageListButtonHandler = Handler<Events::CallbackQuery{}, storageListButtonCallback>;

// StorageView
using storageViewButtonHandler = Handler<Events::CallbackQuery{}, storageViewButtonCallback>;

// StorageViewMembers
using storageMemberViewButtonHandler = Handler<Events::CallbackQuery{}, storageMemberViewButtonCallback>;

// StorageAddDeleteMembers
using memberAdditionDeletionMessageHandler = Handler<Events::Message{}, addDeleteMember>;
using cancelAddDeleteMemberHandler = Handler<Events::CallbackQuery{}, cancelAddDeleteMember>;

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
} // namespace bot_handlers

} // namespace cookcookhnya::handlers
