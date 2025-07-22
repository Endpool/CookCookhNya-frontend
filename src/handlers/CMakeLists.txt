target_sources(main PRIVATE
    src/handlers/commands/start.cpp
    src/handlers/commands/wanna_eat.cpp
    src/handlers/commands/shopping_list.cpp
    src/handlers/commands/personal_account.cpp
    src/handlers/commands/my_storages.cpp

    src/handlers/main_menu/view.cpp

    src/handlers/personal_account/ingredients_list/create.cpp
    src/handlers/personal_account/ingredients_list/publish.cpp
    src/handlers/personal_account/ingredients_list/view.cpp
    src/handlers/personal_account/ingredients_list/delete.cpp

    src/handlers/personal_account/recipes_list/create.cpp
    src/handlers/personal_account/recipes_list/view.cpp

    src/handlers/personal_account/recipe/search_ingredients.cpp
    src/handlers/personal_account/recipe/view.cpp
    src/handlers/personal_account/recipe/moderation_history.cpp

    src/handlers/personal_account/view.cpp
    src/handlers/personal_account/publication_history.cpp

    src/handlers/recipe/add_storage.cpp
    src/handlers/recipe/view.cpp

    src/handlers/recipes_suggestions/view.cpp

    src/handlers/shopping_list/create.cpp
    src/handlers/shopping_list/search.cpp
    src/handlers/shopping_list/storage_selection_to_buy.cpp
    src/handlers/shopping_list/view.cpp

    src/handlers/storage/ingredients/view.cpp
    src/handlers/storage/ingredients/delete.cpp

    src/handlers/storage/members/add.cpp
    src/handlers/storage/members/delete.cpp
    src/handlers/storage/members/view.cpp

    src/handlers/storage/view.cpp
    src/handlers/storage/delete.cpp

    src/handlers/storages_list/create.cpp
    src/handlers/storages_list/view.cpp

    src/handlers/storages_selection/view.cpp
)
