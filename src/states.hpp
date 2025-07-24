#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "backend/models/recipe.hpp"
#include "backend/models/shopping_list.hpp"
#include "backend/models/storage.hpp"
#include "utils/fast_sorted_db.hpp"
#include "utils/utils.hpp"

#include <expected>
#include <functional>
#include <tg_stater/state_storage/common.hpp>
#include <tg_stater/state_storage/memory.hpp>

#include <cstddef>
#include <optional>
#include <ranges>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace cookcookhnya::states {

namespace helpers {

struct StorageIdMixin {
    api::StorageId storageId;
    StorageIdMixin(api::StorageId storageId) : storageId{storageId} {} // NOLINT(*explicit*)
};

struct Pagination {
    std::size_t pageNo;
    std::size_t totalItems;
};

struct SelectableShoppingListItem : api::models::shopping_list::ShoppingListItem {
    bool selected = false;
    SelectableShoppingListItem(api::models::shopping_list::ShoppingListItem item) // NOLINT(*explicit*)
        : ShoppingListItem{std::move(item)} {}
};

struct SelectableIngredient : api::models::ingredient::Ingredient {
    bool selected = false;
    SelectableIngredient(api::models::ingredient::Ingredient item) // NOLINT(*explicit*)
        : Ingredient{std::move(item)} {}
};

} // namespace helpers

struct MainMenu {};

struct RecipesSearch {
    std::string query;
    helpers::Pagination pagination;
    std::vector<api::models::recipe::RecipeSummary> page;
};

struct RecipeView {
    std::optional<RecipesSearch> prevState;
    api::models::recipe::RecipeDetails recipe;
    api::RecipeId recipeId;
};

struct PersonalAccountMenu {};
struct TotalPublicationHistory {
    std::size_t pageNo;
};

struct CustomIngredientsList {};
struct CustomIngredientCreationEnterName {};
struct CustomIngredientConfirmation {
    std::string name;

    // All optionals are for "back" from this menu, so this state won't erase all info
    std::optional<api::RecipeId> recipeFrom;
    std::optional<std::vector<api::models::ingredient::Ingredient>> ingredients;
    std::optional<api::StorageId> storageFrom;

    explicit CustomIngredientConfirmation(
        std::string name,
        std::optional<api::RecipeId> recipeId = std::nullopt,
        std::optional<std::vector<api::models::ingredient::Ingredient>> ingredients = std::nullopt,
        std::optional<api::StorageId> storageId = std::nullopt)
        : name(std::move(name)), recipeFrom(recipeId), ingredients(std::move(ingredients)), storageFrom(storageId) {};
};
struct CustomIngredientDeletion {};
struct CustomIngredientPublish {};

struct StorageList {};
struct StorageCreationEnterName {};

struct StorageView : helpers::StorageIdMixin {
    std::string name;
};
struct StorageDeletion : helpers::StorageIdMixin {};
struct StorageMemberView : helpers::StorageIdMixin {};
struct StorageMemberAddition : helpers::StorageIdMixin {};
struct StorageMemberDeletion : helpers::StorageIdMixin {};

struct StorageIngredientsList : helpers::StorageIdMixin {
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

struct StorageIngredientsDeletion : helpers::StorageIdMixin {
    std::vector<api::models::ingredient::Ingredient> selectedIngredients;
    std::vector<api::models::ingredient::Ingredient> storageIngredients;
    bool addedToShopList;
    std::size_t pageNo;
};

struct StoragesSelection {
    std::variant<MainMenu, RecipeView> prevState;
    std::vector<api::models::storage::StorageSummary> selectedStorages;
};
struct SuggestedRecipesList {
  private:
    using StorageSummary = api::models::storage::StorageSummary;

  public:
    using FromMainMenuData = std::pair<MainMenu, StorageSummary>;
    std::variant<FromMainMenuData, StoragesSelection, StorageView> prevState;
    std::size_t pageNo;

    [[nodiscard]] std::vector<api::StorageId> getStorageIds() const {
        if (const auto* prevState_ = std::get_if<StorageView>(&prevState))
            return {prevState_->storageId};

        if (const auto* prevState_ = std::get_if<SuggestedRecipesList::FromMainMenuData>(&prevState))
            return {prevState_->second.id};

        if (const auto* prevState_ = std::get_if<StoragesSelection>(&prevState))
            return prevState_->selectedStorages | std::views::transform(&StorageSummary::id) |
                   std::ranges::to<std::vector>();
        return {};
    }

    [[nodiscard]] std::variant<std::reference_wrapper<std::vector<StorageSummary>>, std::vector<StorageSummary>>
    getStorages() const {
        if (const auto* prevState_ = std::get_if<StorageView>(&prevState))
            return std::vector{StorageSummary{.id = prevState_->storageId, .name = prevState_->name}};
        if (const auto* prevState_ = std::get_if<SuggestedRecipesList::FromMainMenuData>(&prevState))
            return std::vector{prevState_->second};
        if (const auto* prevState_ = std::get_if<StoragesSelection>(&prevState))
            return std::ref(prevState_->selectedStorages);
        return std::vector<StorageSummary>{};
    }
};
struct CookingPlanning {
  private:
    using StorageSummary = api::models::storage::StorageSummary;

  public:
    enum struct AvailabilityType : std::uint8_t { NOT_AVAILABLE, AVAILABLE, OTHER_STORAGES };

    struct IngredientAvailability {
        cookcookhnya::api::models::recipe::IngredientInRecipe ingredient;
        AvailabilityType available = AvailabilityType::NOT_AVAILABLE;
        std::vector<StorageSummary> storages;
    };

    using FromRecipeViewData = std::pair<RecipeView, std::vector<StorageSummary>>;

    std::variant<SuggestedRecipesList, FromRecipeViewData> prevState;
    std::vector<StorageSummary> addedStorages;
    std::vector<IngredientAvailability> availability;
    api::RecipeId recipeId;

    [[nodiscard]] std::variant<std::reference_wrapper<std::vector<StorageSummary>>, std::vector<StorageSummary>>
    getStorages() const {
        if (const auto* prevState_ = std::get_if<SuggestedRecipesList>(&prevState))
            return prevState_->getStorages();
        if (const auto* prevState_ = std::get_if<CookingPlanning::FromRecipeViewData>(&prevState)) {
            return std::ref(prevState_->second);
        }
        return std::vector<StorageSummary>{};
    }
};

struct CookingPlanningStorageAddition {
    CookingPlanning prevState;
};

struct ShoppingListCreation {
    CookingPlanning prevState;
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

struct CustomRecipeView {
    api::RecipeId recipeId;
    std::size_t pageNo;
    std::vector<api::models::ingredient::Ingredient> ingredients;
    std::string recipeName;
};

struct CreateCustomRecipe {
    std::size_t pageNo;
};

struct RecipeIngredientsSearch {
    api::RecipeId recipeId;
    std::size_t pageNo;
};

struct ShoppingListView { // NOLINT(*member-init) // Strange. Flags only this struct due to ItemsDb
    using ItemsDb =
        utils::FastSortedDb<helpers::SelectableShoppingListItem, &helpers::SelectableShoppingListItem::ingredientId>;

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
    helpers::Pagination pagination;
    std::vector<api::models::ingredient::Ingredient> page;
};

struct CustomRecipePublicationHistory {
    api::RecipeId recipeId;
    std::size_t pageNo;
    std::string recipeName;
    std::string errorReport;
};

struct CookingIngredientsSpending { // NOLINT(*member-init)
    CookingPlanning prevState;
    std::optional<api::StorageId> storageId;
    std::vector<helpers::SelectableIngredient> ingredients;
};

using State = std::variant<MainMenu,
                           PersonalAccountMenu,
                           CustomIngredientsList,
                           CustomIngredientCreationEnterName,
                           CustomIngredientConfirmation,
                           CustomIngredientDeletion,
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
                           CookingPlanning,
                           ShoppingListCreation,
                           ShoppingListView,
                           ShoppingListStorageSelectionToBuy,
                           CreateCustomRecipe,
                           CustomRecipeView,
                           CustomRecipeIngredientsSearch,
                           CustomRecipesList,
                           CookingPlanningStorageAddition,
                           RecipeIngredientsSearch,
                           CustomRecipePublicationHistory,
                           TotalPublicationHistory,
                           ShoppingListIngredientSearch,
                           RecipesSearch,
                           RecipeView,
                           CookingIngredientsSpending>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;

}; // namespace cookcookhnya::states
