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
    [[nodiscard]] std::vector<models::shopping_list::ShoppingListItem>
    get(UserId user, std::size_t count = 500, std::size_t offset = 0) const; // NOLINT(*magic-number*)

    void put(UserId user, const std::vector<IngredientId>& ingredients) const;

    void remove(UserId user, const std::vector<IngredientId>& ingredients) const;

    void buy(UserId user, const std::vector<IngredientId>& ingredients) const;
};

} // namespace cookcookhnya::api
