#pragma once

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
        int id;
    };
}

struct StorageList {};
struct StorageMemberView : detail::UserStoragesIdMixin {};
struct StorageCreationEnterName {};
struct StorageWrongNameToDelete {};
struct StorageDeletionEnterName:detail::UserStoragesIdMixin {};

struct PackMemberView : detail::UserStoragesIdMixin {};
struct MemberAddition : detail::UserStoragesIdMixin {};
struct MemberDeletion : detail::UserStoragesIdMixin {};
struct IngredientsView : detail::UserStoragesIdMixin {};
struct IngredientsAddition : detail::UserStoragesIdMixin{};
struct IngredientsDeletion : detail::UserStoragesIdMixin{};


using State = std::variant<StorageList,
                            StorageMemberView,
                            StorageCreationEnterName,
                            StorageDeletionEnterName,
                            PackMemberView,
                            MemberAddition,
                            MemberDeletion,
                            IngredientsView,
                            IngredientsAddition,
                            IngredientsDeletion>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;
}