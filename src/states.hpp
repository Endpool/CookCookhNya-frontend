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
    std::size_t pageNo;
};

struct StorageSelection {
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
};

struct ShoppingListCreation {
    std::vector<api::StorageId> storageIdsFrom;
    api::RecipeId recipeIdFrom;
    std::vector<api::IngredientId> ingredientIdsInList;
};

struct ShoppingListView {};

using State = std::variant<StorageList,
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
                           ShoppingListView>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;

} // namespace cookcookhnya::states
