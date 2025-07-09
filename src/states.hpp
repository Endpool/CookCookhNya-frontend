#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"

#include <tg_stater/state_storage/common.hpp>
#include <tg_stater/state_storage/memory.hpp>

#include <cstddef>
#include <set>
#include <unordered_map>
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
struct PackMemberView : detail::StorageIdMixin {};
struct MemberAddition : detail::StorageIdMixin {};
struct MemberDeletion : detail::StorageIdMixin {};

class StorageIngredientsList : public detail::StorageIdMixin {
  private:
    using Ingredient = api::models::ingredient::Ingredient;

    struct IngredientComparator {
        bool operator()(const Ingredient& l, const Ingredient& r) const {
            return l.name < r.name;
        }
    };

    std::set<Ingredient, IngredientComparator> storageIngredients;
    std::unordered_map<api::IngredientId, decltype(storageIngredients)::iterator> ingredientIndex;

  public:
    std::size_t totalFound = 0;                                                       // NOLINT(*non-private*)
    std::size_t pageNo = 0;                                                           // NOLINT(*non-private*)
    std::vector<api::models::ingredient::IngredientSearchForStorageItem> searchItems; // NOLINT(*non-private*)

    StorageIngredientsList(api::StorageId storageId, decltype(storageIngredients) storageIngredients);

    void putIngredient(Ingredient ingredient);

    void removeIngredient(api::IngredientId id);

    const decltype(storageIngredients)& getStorageIngredients() const {
        return storageIngredients;
    }
};

struct StorageSelection {
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

struct ShoppingListCreation {
    std::vector<api::StorageId> storageIdsFrom;
    api::RecipeId recipeIdFrom;
    std::vector<api::IngredientId> ingredientIdsInList;
    bool fromStorage;
    std::size_t pageNo;
};

struct ShoppingListView {};

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
                           PackMemberView,
                           MemberAddition,
                           MemberDeletion,
                           StorageIngredientsList,
                           StorageSelection,
                           SuggestedRecipeList,
                           RecipeView,
                           ShoppingListCreation,
                           ShoppingListView>;

using StateManager = tg_stater::StateProxy<tg_stater::MemoryStateStorage<State>>;

} // namespace cookcookhnya::states
