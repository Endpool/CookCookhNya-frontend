target_sources(main PRIVATE
    src/backend/api/base.cpp

    src/backend/api/publicity_filter.cpp

    src/backend/api/storages.cpp
    src/backend/api/ingredients.cpp
    src/backend/api/users.cpp
    src/backend/api/recipes.cpp
    src/backend/api/shopping_lists.cpp
    src/backend/api/moderation.cpp

    src/backend/models/storage.cpp
    src/backend/models/ingredient.cpp
    src/backend/models/user.cpp
    src/backend/models/recipe.cpp
    src/backend/models/shopping_list.cpp
    src/backend/models/publication_request_status.cpp
    src/backend/models/moderation.cpp
)
