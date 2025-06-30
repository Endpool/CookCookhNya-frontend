#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "tg_types.hpp"

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

struct StorageCreation {};
struct StorageDeletion : detail::StorageIdMixin {};
struct StorageCreationEnterName {};
struct StorageWrongNameToDelete {};
struct StorageDeletionEnterName {};
struct StorageView : detail::StorageIdMixin {};

struct StorageMemberView : detail::StorageIdMixin {};
struct MembersAdditionDeletion : detail::StorageIdMixin {};
struct PackMemberView : detail::StorageIdMixin {};
struct MemberAddition : detail::StorageIdMixin {};
struct MemberDeletion : detail::StorageIdMixin {};

struct StorageIngredientsList : detail::StorageIdMixin {};
struct StorageIngredientsSearch : detail::StorageIdMixin {
    tg_types::MessageId message;
    std::vector<api::models::ingredient::IngredientSearchResult> shownIngredients;
};

struct StorageSelection {
    std::vector<api::StorageId> storageIds;
    tg_types::MessageId messageId;
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

using State = std::variant<StorageList,
                           StorageCreation,
                           StorageDeletion,
                           StorageCreationEnterName,
                           StorageWrongNameToDelete,
                           StorageDeletionEnterName,
                           StorageView,
                           StorageMemberView,
                           MembersAdditionDeletion,
                           PackMemberView,
                           MemberAddition,
                           MemberDeletion,
                           StorageIngredientsList,
                           StorageIngredientsSearch,
                           StorageSelection,
                           SuggestedRecipeList,
                           RecipeView>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;

} // namespace cookcookhnya::states
