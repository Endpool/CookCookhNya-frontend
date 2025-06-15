#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>
namespace detail {
    struct UserStoragesIdMixin {
        int id;
    };
}

struct StorageList {};
struct StorageView : detail::UserStoragesIdMixin {};
struct StorageCreation {};

struct StorageDeletion:detail::UserStoragesIdMixin {};

struct PackMemberView : detail::UserStoragesIdMixin {};
struct MemberAddition : detail::UserStoragesIdMixin {};
struct MemberDeletion : detail::UserStoragesIdMixin {};
struct IngredientsView : detail::UserStoragesIdMixin {};
struct IngredientsAddition : detail::UserStoragesIdMixin{};
struct IngredientsDeletion : detail::UserStoragesIdMixin{};


using State = std::variant<StorageList,
                            StorageView,
                            StorageCreation,
                            StorageDeletion,
                            PackMemberView,
                            MemberAddition,
                            MemberDeletion,
                            IngredientsView,
                            IngredientsAddition,
                            IngredientsDeletion>;