#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "backend/models/shopping_list.hpp"
#include "backend/models/storage.hpp"
#include "utils/fast_sorted_db.hpp"
#include "utils/ingredients_availability.hpp"

#include <optional>
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

    // All optionals are for "back" from this menu, so this state won't erase all info
    std::optional<api::RecipeId> recipeFrom;
    std::optional<size_t> pageNo;
    std::optional<std::vector<api::models::ingredient::Ingredient>> ingredients;

    std::optional<api::StorageId> storageFrom;

    explicit CustomIngredientConfirmation(
        std::string name,
        std::optional<api::RecipeId> recipeId = std::nullopt,
        std::optional<size_t> pageNo = std::nullopt,
        std::optional<std::vector<api::models::ingredient::Ingredient>> ingredients = std::nullopt,
        std::optional<api::StorageId> storageId = std::nullopt)
        : name(std::move(name)), recipeFrom(recipeId), pageNo(pageNo), ingredients(std::move(ingredients)),
          storageFrom(storageId) {};
};
struct CustomIngredientPublish {};

struct StorageList {};
struct StorageCreationEnterName {};

struct StorageView : detail::StorageIdMixin {};
struct StorageDeletion : detail::StorageIdMixin {};
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
    std::vector<api::models::storage::StorageSummary> selectedStorages;
};
struct SuggestedRecipesList {
    std::size_t pageNo;
    std::vector<api::models::storage::StorageSummary> selectedStorages;
    bool fromStorage;
};
struct RecipeView {
    std::vector<api::models::storage::StorageSummary> selectedStorages;
    std::vector<api::models::storage::StorageSummary> addedStorages;
    std::vector<std::pair<cookcookhnya::api::models::recipe::IngredientInRecipe, utils::IngredientAvailability>>
        availability;
    api::RecipeId recipeId;
    bool fromStorage;
    std::size_t pageNo;
};

struct RecipeStorageAddition {
    std::vector<api::models::storage::StorageSummary> selectedStorages;
    std::vector<api::models::storage::StorageSummary> addedStorages;
    std::vector<std::pair<cookcookhnya::api::models::recipe::IngredientInRecipe, utils::IngredientAvailability>>
        availability;
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
    std::string recipeName;
};

struct CreateCustomRecipe {
    api::RecipeId recipeId;
    std::size_t pageNo;
};

struct ShoppingListCreation {
    std::vector<api::models::storage::StorageSummary> selectedStorages;
    std::vector<api::models::storage::StorageSummary> addedStorages;
    std::vector<std::pair<cookcookhnya::api::models::recipe::IngredientInRecipe, utils::IngredientAvailability>>
        availability;
    api::RecipeId recipeId;
    std::vector<api::models::ingredient::Ingredient> selectedIngredients;
    std::vector<api::models::ingredient::Ingredient> allIngredients;
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
struct ShoppingListStorageSelectionToBuy {
    ShoppingListView prevState;
    std::vector<api::IngredientId> selectedIngredients;
    std::vector<api::models::storage::StorageSummary> storages;
};

struct CustomRecipePublicationHistory {
    api::RecipeId recipeId;
    std::size_t pageNo;
};

struct AllPublicationHistory {
    std::size_t pageNo;
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
                           SuggestedRecipesList,
                           RecipeView,
                           ShoppingListCreation,
                           ShoppingListView,
                           ShoppingListStorageSelectionToBuy,
                           CreateCustomRecipe,
                           RecipeCustomView,
                           CustomRecipeIngredientsSearch,
                           CustomRecipesList,
                           RecipeStorageAddition,
                           RecipeIngredientsSearch,
                           CustomRecipePublicationHistory,
                           AllPublicationHistory>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;

}; // namespace cookcookhnya::states
