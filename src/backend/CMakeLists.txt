target_sources(main PRIVATE
    src/backend/api/base.cpp
    src/backend/api/users.cpp
    src/backend/api/recipes.cpp
    src/backend/api/storages.cpp
    src/backend/api/ingredients.cpp
    src/backend/models/user.cpp
    src/backend/models/recipe.cpp
    src/backend/models/storage.cpp
    src/backend/models/ingredient.cpp
)
