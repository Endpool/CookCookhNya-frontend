#pragma once

#include "backend/api/base.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"

#include <httplib.h>

#include <vector>

namespace cookcookhnya::api {

class RecipesApi : ApiBase {
    friend class ApiClient;

    explicit RecipesApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] models::recipe::RecipesSummary
    getRecipeList(UserId userId, int size, int offset, std::vector<StorageId> storageId) const;
    // Functions below is for future
    /*RecipeId create(UserId userId, // NOLINT(*-nodiscard)
                     const models::recipe::RecipeCreateBody& body) const;
    void delete_(UserId userId, RecipeId id) const;*/
};

} // namespace cookcookhnya::api
