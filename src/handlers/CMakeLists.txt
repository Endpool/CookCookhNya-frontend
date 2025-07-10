target_sources(main PRIVATE
    src/handlers/initial/start.cpp

    src/handlers/main_menu/view.cpp

    src/handlers/personal_account/view.cpp
    src/handlers/personal_account/ingredients/view.cpp
    src/handlers/personal_account/ingredients/create.cpp
    src/handlers/personal_account/ingredients/publish.cpp


    src/handlers/recipes_suggestion/recipe/view.cpp
    src/handlers/recipes_suggestion/storage_selection/select.cpp
    src/handlers/recipes_suggestion/suggest.cpp
    src/handlers/recipes_suggestion/recipe/add_storage.cpp

    src/handlers/shopping_list/create.cpp
    src/handlers/shopping_list/view.cpp

    src/handlers/storage_list/create.cpp
    src/handlers/storage_list/delete.cpp
    src/handlers/storage_list/view.cpp

    src/handlers/storage_view/ingredients/view.cpp
    src/handlers/storage_view/members/add.cpp
    src/handlers/storage_view/members/delete.cpp
    src/handlers/storage_view/members/view.cpp
    src/handlers/storage_view/view.cpp

    src/handlers/personal_account/view.cpp
    src/handlers/custom_recipes_list/view.cpp
    src/handlers/custom_recipes_list/create.cpp
    src/handlers/custom_recipes_list/custom_recipe/view.cpp
    src/handlers/custom_recipes_list/custom_recipe/search.cpp
)
