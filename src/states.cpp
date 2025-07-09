#include "states.hpp"

#include "backend/id_types.hpp"

#include <utility>

namespace cookcookhnya::states {

StorageIngredientsList::StorageIngredientsList(api::StorageId storageId,
                                               decltype(storageIngredients) storageIngredients)
    : StorageIdMixin{storageId}, storageIngredients{std::move(storageIngredients)} {
    for (auto it = this->storageIngredients.begin(); it != this->storageIngredients.end(); ++it)
        ingredientIndex.try_emplace(it->id, it);
}

void StorageIngredientsList::putIngredient(Ingredient ingredient) {
    const auto [it, inserted] = storageIngredients.insert(std::move(ingredient));
    if (inserted)
        ingredientIndex.try_emplace(it->id, it);
}

void StorageIngredientsList::removeIngredient(api::IngredientId id) {
    auto it = ingredientIndex.find(id);
    if (it == ingredientIndex.end())
        return;
    storageIngredients.erase(it->second);
    ingredientIndex.erase(it);
}

} // namespace cookcookhnya::states
