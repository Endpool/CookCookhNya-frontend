#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "backend/models/shopping_list.hpp"
#include "utils/fast_sorted_db.hpp"

#include <tg_stater/state_storage/common.hpp>
#include <tg_stater/state_storage/memory.hpp>

#include <concepts>
#include <cstddef>
#include <ranges>
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
    std::string inlineQuery;

    template <std::ranges::range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, IngredientsDb::mapped_type>
    StorageIngredientsList(api::StorageId storageId, R&& ingredients, std::string iq)
        : StorageIdMixin{storageId}, storageIngredients{std::forward<R>(ingredients)}, inlineQuery(std::move(iq)) {}
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
    std::size_t pageNo;
};

struct RecipeStorageAddition {
    std::vector<api::StorageId> storageIds;
    api::RecipeId recipeId;
    bool fromStorage;
    std::size_t pageNo;
};

struct CustomRecipesList {
    std::size_t pageNo;
};

struct CustomRecipeIngredientsSearch {
    using IngredientsDb = utils::FastSortedDb<api::models::ingredient::Ingredient>;

    api::RecipeId recipeId;
    IngredientsDb recipeIngredients;
    std::string query;
    std::size_t totalFound = 0;
    std::size_t pageNo = 0;
    std::vector<api::models::ingredient::IngredientSearchForRecipeItem> searchItems;

    template <std::ranges::range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, IngredientsDb::mapped_type>
    CustomRecipeIngredientsSearch(api::RecipeId recipeId, R&& ingredients, std::string inlineQuery)
        : recipeId(recipeId), recipeIngredients{std::forward<R>(ingredients)}, query(std::move(inlineQuery)) {}
};

struct RecipeCustomView {
    api::RecipeId recipeId;
    std::size_t pageNo;
    std::vector<api::models::ingredient::Ingredient> ingredients;
};

struct CreateCustomRecipe {
    api::RecipeId recipeId;
    std::size_t pageNo;
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
    std::size_t pageNo;
};

struct ShoppingListView { // NOLINT(*member-init) // Strange. Flags only this struct due to ItemsDb
    struct SelectableItem : api::models::shopping_list::ShoppingListItem {
        bool selected = false;
    };
    using ItemsDb = utils::FastSortedDb<SelectableItem, &api::models::shopping_list::ShoppingListItem::ingredientId>;

    ItemsDb items;
    bool canBuy;
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
