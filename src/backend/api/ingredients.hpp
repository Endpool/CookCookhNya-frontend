#pragma once

#include "backend/api/base.hpp"
#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"

#include <httplib.h>

#include <vector>

namespace cookcookhnya::api {

class IngredientsApi : ApiBase {
    friend class ApiClient;

    explicit IngredientsApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] std::vector<models::ingredient::Ingredient> getStorageIngredients(UserId user,
                                                                                    StorageId storage) const;
    void put(UserId user, StorageId storage, IngredientId id) const;
    void delete_(UserId user, StorageId storage, IngredientId id) const;
    [[nodiscard]] std::vector<models::ingredient::Ingredient> getAllIngredients() const;
};

} // namespace cookcookhnya::api
