target_sources(main PRIVATE
    src/handlers/initial/initial_start.cpp
    src/handlers/storage_list/storage_list_create.cpp
    src/handlers/storage_list/storage_list_delete.cpp
    src/handlers/storage_list/storage_list_view.cpp
    src/handlers/storage_view/storage_add_delete_members.cpp
    src/handlers/storage_view/storage_view.cpp
    src/handlers/storage_view/storage_view_members.cpp
    src/handlers/storage_view/ingredients/list.cpp
    src/handlers/storage_view/ingredients/search.cpp
    src/handlers/recipes_suggestion/storages_select.cpp
    src/handlers/recipes_suggestion/recipes_suggestion_list.cpp
    src/handlers/recipes_suggestion/recipe_view.cpp
)
