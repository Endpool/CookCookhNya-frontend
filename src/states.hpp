#pragma once

#include <backend/id_types.hpp>
#include <tg_stater/state_storage/common.hpp>
#include <tg_stater/state_storage/memory.hpp>
#include <variant>
#include <vector>

namespace cookcookhnya::states {
using namespace api;

namespace detail {
struct StorageIdMixin {
    StorageId storageId;
    StorageIdMixin(int storageId) : storageId{storageId} {} // NOLINT(*-explicit-*)
};

} // namespace detail

struct StorageList {};

struct StorageCreation {};
struct StorageDeletion : detail::StorageIdMixin {};
struct StorageCreationEnterName {};
struct StorageWrongNameToDelete {};
struct StorageDeletionEnterName {};
struct StorageView : detail::StorageIdMixin {};

struct StorageSelection {
    std::vector<StorageId> storageIds;
};

struct SuggestedRecipeList {
    int pageNo; // DONT FORGET TO INIT FROM AMIRKHAN
    std::vector<StorageId> storageIds;
};

struct StorageMemberView : detail::StorageIdMixin {};
struct MembersAdditionDeletion : detail::StorageIdMixin {};
struct PackMemberView : detail::StorageIdMixin {};
struct MemberAddition : detail::StorageIdMixin {};
struct MemberDeletion : detail::StorageIdMixin {};

struct IngredientsView : detail::StorageIdMixin {};
struct IngredientsAddition : detail::StorageIdMixin {};
struct IngredientsDeletion : detail::StorageIdMixin {};

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
                           IngredientsView,
                           IngredientsAddition,
                           IngredientsDeletion,
                           StorageSelection,
                           SuggestedRecipeList>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;

} // namespace cookcookhnya::states
