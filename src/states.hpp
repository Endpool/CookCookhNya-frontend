#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "backend/models/shopping_list.hpp"
#include "utils/fast_sorted_db.hpp"

#include <tg_stater/state_storage/common.hpp>
#include <tg_stater/state_storage/memory.hpp>

#include <cstddef>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace cookcookhnya::states {

namespace detail {

struct StorageIdMixin {
    api::StorageId storageId;
    StorageIdMixin(api::StorageId storageId) : storageId{storageId} {} // NOLINT(*-explicit-*)
};

} // namespace detail

struct MainMenu {};

struct PersonalAccountMenu {};

struct CustomIngredientsList {};
struct CustomIngredientCreationEnterName {};
struct CustomIngredientConfirmation {
    std::string name;
};
struct CustomIngredientPublish {};

struct StorageList {};
struct StorageDeletion {};
struct StorageCreationEnterName {};
struct StorageView : detail::StorageIdMixin {};

struct StorageMemberView : detail::StorageIdMixin {};
struct StorageMemberAddition : detail::StorageIdMixin {};
struct StorageMemberDeletion : detail::StorageIdMixin {};

struct StorageIngredientsList : detail::StorageIdMixin {
    using IngredientsDb = utils::FastSortedDb<api::models::ingredient::Ingredient>;

    IngredientsDb storageIngredients;
    std::size_t totalFound = 0;
    std::size_t pageNo = 0;
    std::vector<api::models::ingredient::IngredientSearchForStorageItem> searchItems;

    StorageIngredientsList(api::StorageId storageId, IngredientsDb::Set ingredients)
        : StorageIdMixin{storageId}, storageIngredients{std::move(ingredients)} {}
};

struct StoragesSelection {
    std::vector<api::StorageId> storageIds;
};
struct SuggestedRecipeList {
    std::size_t pageNo;
    std::vector<api::StorageId> storageIds;
    bool fromStorage;
};
struct RecipeView {
    std::vector<api::StorageId> storageIds;
    api::RecipeId recipeId;
    bool fromStorage;
    size_t pageNo;
};

struct RecipeStorageAddition {
    std::vector<api::StorageId> storageIds;
    api::RecipeId recipeId;
    bool fromStorage;
    size_t pageNo;
};

struct CustomRecipesList {
    size_t pageNo;
};

struct CustomRecipeIngredientsSearch {
    api::RecipeId recipeId;
    std::vector<api::models::ingredient::IngredientSearchForRecipeItem> shownIngredients;
    std::size_t totalFound;
    size_t pageNo;
};

struct RecipeCustomView {
    api::RecipeId recipeId;
    size_t pageNo;
};

struct CreateCustomRecipe {
    api::RecipeId recipeId;
    size_t pageNo;
};

struct ShoppingListCreation {
    std::vector<api::StorageId> storageIdsFrom;
    api::RecipeId recipeIdFrom;
    std::vector<api::IngredientId> ingredientIdsInList;
    bool fromStorage;
    std::size_t pageNo;
};

struct RecipeIngredientsSearch {
    api::RecipeId recipeId;
    size_t pageNo;
};

struct ShoppingListView {
    struct SelectableItem : api::models::shopping_list::ShoppingListItem {
        bool selected = false;
    };
    using ItemsDb = utils::FastSortedDb<SelectableItem, &api::models::shopping_list::ShoppingListItem::ingredientId>;

    ItemsDb items;
};

using State = std::variant<MainMenu,
                           PersonalAccountMenu,
                           CustomIngredientsList,
                           CustomIngredientCreationEnterName,
                           CustomIngredientConfirmation,
                           CustomIngredientPublish,
                           StorageList,
                           StorageDeletion,
                           StorageCreationEnterName,
                           StorageView,
                           StorageMemberView,
                           StorageMemberAddition,
                           StorageMemberDeletion,
                           StorageIngredientsList,
                           StoragesSelection,
                           SuggestedRecipeList,
                           RecipeView,
                           ShoppingListCreation,
                           ShoppingListView,
                           CreateCustomRecipe,
                           RecipeCustomView,
                           CustomRecipeIngredientsSearch,
                           CustomRecipesList,
                           RecipeStorageAddition,
                           RecipeIngredientsSearch>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;

}; // namespace cookcookhnya::states
