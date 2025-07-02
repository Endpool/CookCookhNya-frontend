#pragma once

#include "backend/api/base.hpp"
#include "backend/models/shopping_list.hpp"

#include <httplib.h>

#include <vector>

namespace cookcookhnya::api {

class ShoppingListApi : ApiBase {
    friend class ApiClient;

    explicit ShoppingListApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] std::vector<models::shopping_list::ShoppingListItem> get(UserId userId) const;
};

} // namespace cookcookhnya::api
