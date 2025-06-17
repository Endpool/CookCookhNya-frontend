#pragma once

#include "types.hpp"

#include <tg_stater/state_storage/common.hpp>
#include <tg_stater/state_storage/memory.hpp>

#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace states {

namespace detail {
    struct UserStoragesIdMixin {
        int storageId;
        UserStoragesIdMixin(int storageId) : storageId{storageId} {} // NOLINT(*-explicit-*)
    };
}

struct StorageList {};
struct StorageView : detail::UserStoragesIdMixin {};
struct StorageCreation {};

struct StorageDeletion:detail::UserStoragesIdMixin {};

struct PackMemberView : detail::UserStoragesIdMixin {};
struct MembersAdditionDeletion : detail::UserStoragesIdMixin {};
struct IngredientsView : detail::UserStoragesIdMixin {};
struct IngredientsAddition : detail::UserStoragesIdMixin{};
struct IngredientsDeletion : detail::UserStoragesIdMixin{};


using State = std::variant<StorageList,
                            StorageView,
                            StorageCreation,
                            StorageDeletion,
                            PackMemberView,
                            MembersAdditionDeletion,
                            IngredientsView,
                            IngredientsAddition,
                            IngredientsDeletion>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;

} // namespace states

