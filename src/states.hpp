#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "backend/models/shopping_list.hpp"
#include "backend/models/storage.hpp"
#include "utils/fast_sorted_db.hpp"
#include "utils/ingredients_availability.hpp"

#include <tg_stater/state_storage/common.hpp>
#include <tg_stater/state_storage/memory.hpp>

#include <cstddef>
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
struct StorageCreationEnterName {};

struct StorageView : detail::StorageIdMixin {};
struct StorageDeletion : detail::StorageIdMixin{};
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
    StorageIngredientsList(api::StorageId storageId, IngredientsDb::Set ingredients, std::string iq)
        : StorageIdMixin{storageId}, storageIngredients{std::move(ingredients)}, inlineQuery(std::move(iq)) {}
};

struct StoragesSelection {
    std::vector<api::models::storage::StorageSummary> selectedStorages;
};
struct SuggestedRecipeList {
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
    size_t pageNo;
};

struct RecipeStorageAddition {
    std::vector<api::models::storage::StorageSummary> selectedStorages;
    std::vector<api::models::storage::StorageSummary> addedStorages;
    std::vector<std::pair<cookcookhnya::api::models::recipe::IngredientInRecipe, utils::IngredientAvailability>>
        availability;
    api::RecipeId recipeId;
    bool fromStorage;
    size_t pageNo;
};

struct CustomRecipesList {
    size_t pageNo;
};

struct CustomRecipeIngredientsSearch {
    using IngredientsDb = utils::FastSortedDb<api::models::ingredient::Ingredient>;
    api::RecipeId recipeId;
    IngredientsDb recipeIngredients;
    std::size_t totalFound = 0;
    std::size_t pageNo = 0;
    std::vector<api::models::ingredient::IngredientSearchForRecipeItem> searchItems;
    std::string inlineQuery;

    CustomRecipeIngredientsSearch(api::RecipeId recipeId, IngredientsDb::Set ingredients, std::string iq)
        : recipeId(recipeId), recipeIngredients{std::move(ingredients)}, inlineQuery(std::move(iq)) {}
};

struct RecipeCustomView {
    api::RecipeId recipeId;
    size_t pageNo;
    std::vector<api::models::ingredient::Ingredient> ingredients;
};

struct CreateCustomRecipe {
    api::RecipeId recipeId;
    size_t pageNo;
};

struct ShoppingListCreation {
    std::vector<api::models::storage::StorageSummary> selectedStorages;
    std::vector<api::models::storage::StorageSummary> addedStorages;
    std::vector<std::pair<cookcookhnya::api::models::recipe::IngredientInRecipe, utils::IngredientAvailability>>
        availability;
    api::RecipeId recipeId;
    std::vector<api::models::ingredient::Ingredient> selectedIngredients;
    bool fromStorage;
    std::size_t pageNo;
};

struct RecipeIngredientsSearch {
    api::RecipeId recipeId;
    size_t pageNo;
};

struct ShoppingListView {
    using ItemsDb = utils::FastSortedDb<api::models::shopping_list::ShoppingListItem,
                                        &api::models::shopping_list::ShoppingListItem::ingredientId>;

    ItemsDb items;
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
