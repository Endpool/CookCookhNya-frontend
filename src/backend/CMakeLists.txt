target_sources(main PRIVATE
    src/backend/api/base.cpp

    src/backend/api/publicity_filter.cpp

    src/backend/api/storages.cpp
    src/backend/api/ingredients.cpp
    src/backend/api/users.cpp
    src/backend/api/recipes.cpp
    src/backend/api/shopping_lists.cpp
    src/backend/api/request_history.cpp

    src/backend/models/storage.cpp
    src/backend/models/ingredient.cpp
    src/backend/models/user.cpp
    src/backend/models/recipe.cpp
    src/backend/models/shopping_list.cpp
    src/backend/models/request_history.cpp
)
