target_sources(main PRIVATE
    src/render/main_menu/view.cpp

    src/render/personal_account/ingredients_list/create.cpp
    src/render/personal_account/ingredients_list/publish.cpp
    src/render/personal_account/ingredients_list/view.cpp
    src/render/personal_account/ingredients_list/delete.cpp

    src/render/personal_account/recipes_list/create.cpp
    src/render/personal_account/recipes_list/view.cpp

    src/render/personal_account/recipe/search_ingredients.cpp
    src/render/personal_account/recipe/view.cpp

    src/render/personal_account/view.cpp

    src/render/recipe/add_storage.cpp
    src/render/recipe/view.cpp

    src/render/recipes_suggestions/view.cpp

    src/render/shopping_list/create.cpp
    src/render/shopping_list/storage_selection_to_buy.cpp
    src/render/shopping_list/view.cpp

    src/render/storage/ingredients/view.cpp

    src/render/storage/members/add.cpp
    src/render/storage/members/delete.cpp
    src/render/storage/members/view.cpp

    src/render/storage/view.cpp
    src/render/storage/delete.cpp

    src/render/storages_list/create.cpp
    src/render/storages_list/view.cpp

    src/render/storages_selection/view.cpp
)
