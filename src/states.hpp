#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "backend/models/recipe.hpp"
#include "backend/models/shopping_list.hpp"
#include "backend/models/storage.hpp"
#include "utils/fast_sorted_db.hpp"
#include "utils/utils.hpp"

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
    StorageIdMixin(api::StorageId storageId) : storageId{storageId} {} // NOLINT(*explicit*)
};

struct Pagination {
    std::size_t pageNo;
    std::size_t totalItems;
};

} // namespace detail

struct MainMenu {};

struct PersonalAccountMenu {};

struct CustomIngredientsList {
    std::size_t pageNo;
};
struct CustomIngredientCreationEnterName {
    std::size_t pageNo;
};
struct CustomIngredientConfirmation {
    std::size_t pageNo;
    std::string name;
};
struct CustomIngredientPublish {
    std::size_t pageNo;
};

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

    template <utils::range_of<IngredientsDb::mapped_type> R>
    StorageIngredientsList(api::StorageId storageId, R&& ingredients, std::string iq)
        : StorageIdMixin{storageId}, storageIngredients{std::forward<R>(ingredients)}, inlineQuery(std::move(iq)) {}
};

struct StorageIngredientsDeletion : detail::StorageIdMixin {
    std::vector<api::models::ingredient::Ingredient> selectedIngredients;
    std::vector<api::models::ingredient::Ingredient> storageIngredients;
    bool addedToShopList;
    std::size_t pageNo;
};

struct StoragesSelection {
    std::vector<api::models::storage::StorageSummary> selectedStorages;
};
struct SuggestedRecipesList {
    std::vector<api::models::storage::StorageSummary> selectedStorages;
    std::size_t pageNo;
    bool fromStorage;
};
struct RecipeView {
    enum struct AvailabilityType : std::uint8_t { NOT_AVAILABLE, AVAILABLE, OTHER_STORAGES };

    struct IngredientAvailability {
        cookcookhnya::api::models::recipe::IngredientInRecipe ingredient;
        AvailabilityType available = AvailabilityType::NOT_AVAILABLE;
        std::vector<api::models::storage::StorageSummary> storages;
    };

    SuggestedRecipesList prevState;
    std::vector<api::models::storage::StorageSummary> addedStorages;
    std::vector<IngredientAvailability> availability;
    api::RecipeId recipeId;
};

struct RecipeStorageAddition {
    RecipeView prevState;
};

struct ShoppingListCreation {
    RecipeView prevState;
    std::vector<api::models::ingredient::Ingredient> selectedIngredients;
    std::vector<api::models::ingredient::Ingredient> allIngredients;
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

    template <utils::range_of<IngredientsDb::mapped_type> R>
    CustomRecipeIngredientsSearch(api::RecipeId recipeId, R&& ingredients, std::string inlineQuery)
        : recipeId(recipeId), recipeIngredients{std::forward<R>(ingredients)}, query(std::move(inlineQuery)) {}
};

struct RecipeCustomView {
    api::RecipeId recipeId;
    std::size_t pageNo;
    std::vector<api::models::ingredient::Ingredient> ingredients;
};

struct CreateCustomRecipe {
    std::size_t pageNo;
};

struct RecipeIngredientsSearch {
    api::RecipeId recipeId;
    std::size_t pageNo;
};

struct ShoppingListView { // NOLINT(*member-init) // Strange. Flags only this struct due to ItemsDb
    struct SelectableItem : api::models::shopping_list::ShoppingListItem {
        bool selected = false;
        SelectableItem(api::models::shopping_list::ShoppingListItem item) // NOLINT(*explicit*)
            : ShoppingListItem{std::move(item)} {}
    };
    using ItemsDb = utils::FastSortedDb<SelectableItem, &SelectableItem::ingredientId>;

    ItemsDb items;
    bool canBuy;
};
struct ShoppingListStorageSelectionToBuy {
    ShoppingListView prevState;
    std::vector<api::IngredientId> selectedIngredients;
    std::vector<api::models::storage::StorageSummary> storages;
};
struct ShoppingListIngredientSearch {
    ShoppingListView prevState;
    std::string query;
    detail::Pagination pagination;
    std::vector<api::models::ingredient::Ingredient> page;
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
                           StorageIngredientsDeletion,
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
                           ShoppingListIngredientSearch>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;

}; // namespace cookcookhnya::states
