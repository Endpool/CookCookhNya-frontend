#pragma once

#include <tg_stater/state_storage/common.hpp>
#include <tg_stater/state_storage/memory.hpp>

#include <variant>

namespace cookcookhnya::states {

namespace detail {

struct UserStoragesIdMixin {
    int storageId;
    UserStoragesIdMixin(int storageId) : storageId{storageId} {} // NOLINT(*-explicit-*)
};

} // namespace detail

struct StorageList {};

struct StorageCreation {};
struct StorageDeletion : detail::UserStoragesIdMixin {};
struct StorageCreationEnterName {};
struct StorageWrongNameToDelete {};
struct StorageDeletionEnterName {};

struct StorageView : detail::UserStoragesIdMixin {};

struct StorageMemberView : detail::UserStoragesIdMixin {};
struct MembersAdditionDeletion : detail::UserStoragesIdMixin {};
struct PackMemberView : detail::UserStoragesIdMixin {};
struct MemberAddition : detail::UserStoragesIdMixin {};
struct MemberDeletion : detail::UserStoragesIdMixin {};

struct IngredientsView : detail::UserStoragesIdMixin {};
struct IngredientsAddition : detail::UserStoragesIdMixin {};
struct IngredientsDeletion : detail::UserStoragesIdMixin {};

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
                           IngredientsDeletion>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;

} // namespace cookcookhnya::states
