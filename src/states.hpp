#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"

#include <tg_stater/state_storage/common.hpp>
#include <tg_stater/state_storage/memory.hpp>

#include <cstddef>
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

struct StorageList {};

struct StorageDeletion {};
struct StorageCreationEnterName {};
struct StorageView : detail::StorageIdMixin {};

struct StorageMemberView : detail::StorageIdMixin {};
struct PackMemberView : detail::StorageIdMixin {};
struct MemberAddition : detail::StorageIdMixin {};
struct MemberDeletion : detail::StorageIdMixin {};

struct StorageIngredientsList : detail::StorageIdMixin {};
struct StorageIngredientsSearch : detail::StorageIdMixin {
    std::vector<api::models::ingredient::IngredientSearchItem> shownIngredients;
    std::size_t totalFound;
    size_t pageNo;
};

struct StorageSelection {
    std::vector<api::StorageId> storageIds;
};
struct SuggestedRecipeList {
    size_t pageNo;
    std::vector<api::StorageId> storageIds;
    bool fromStorage;
};
struct RecipeView {
    std::vector<api::StorageId> storageIds;
    api::RecipeId recipeId;
    bool fromStorage;
    size_t pageNo;
};

struct RecipeAddStoarege {
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
    std::vector<api::models::ingredient::IngredientSearchItem> shownIngredients;
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
    size_t pageNo;
};

struct ShoppingListView {};

struct PersonalAccountMenu {};

using State = std::variant<MainMenu,
                           StorageList,
                           StorageDeletion,
                           StorageCreationEnterName,
                           StorageView,
                           StorageMemberView,
                           PackMemberView,
                           MemberAddition,
                           MemberDeletion,
                           StorageIngredientsList,
                           StorageIngredientsSearch,
                           StorageSelection,
                           SuggestedRecipeList,
                           RecipeView,
                           ShoppingListCreation,
                           ShoppingListView,
                           PersonalAccountMenu,
                           CustomRecipesList,
                           RecipeCustomView,
                           CreateCustomRecipe,
                           CustomRecipeIngredientsSearch,
                           RecipeAddStoarege>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;

} // namespace cookcookhnya::states
