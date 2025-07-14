target_sources(main PRIVATE
    src/handlers/initial/start.cpp

    src/handlers/main_menu/view.cpp

    src/handlers/personal_account/ingredients_list/create.cpp
    src/handlers/personal_account/ingredients_list/publish.cpp
    src/handlers/personal_account/ingredients_list/view.cpp

    src/handlers/personal_account/recipes_list/create.cpp
    src/handlers/personal_account/recipes_list/recipe/search_ingredients.cpp
    src/handlers/personal_account/recipes_list/recipe/view.cpp
    src/handlers/personal_account/recipes_list/view.cpp

    src/handlers/personal_account/view.cpp

    src/handlers/recipe/add_storage.cpp
    src/handlers/recipe/view.cpp

    src/handlers/recipes_suggestions/view.cpp

    src/handlers/shopping_list/create.cpp
    src/handlers/shopping_list/view.cpp

    src/handlers/storage/ingredients/view.cpp

    src/handlers/storage/members/add.cpp
    src/handlers/storage/members/delete.cpp
    src/handlers/storage/members/view.cpp

    src/handlers/storage/view.cpp

    src/handlers/storages_list/create.cpp
    src/handlers/storages_list/delete.cpp
    src/handlers/storages_list/view.cpp

    src/handlers/storages_selection/view.cpp
)
