#pragma once

#include "backend/api/base.hpp"
#include "backend/id_types.hpp"
#include "backend/models/shopping_list.hpp"

#include <httplib.h>

#include <vector>

namespace cookcookhnya::api {

class ShoppingListApi : ApiBase {
    friend class ApiClient;

    explicit ShoppingListApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] std::vector<models::shopping_list::ShoppingListItem> get(UserId userId) const;

    void put(UserId userId, const std::vector<IngredientId>& ingredientIds) const;

    void remove(UserId userId, const std::vector<IngredientId>& ingredientIds) const;
};

} // namespace cookcookhnya::api
