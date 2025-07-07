target_sources(main PRIVATE
    src/render/main_menu/main_menu_render.cpp

    src/render/storage_list/create_storage_render.cpp
    src/render/storage_list/delete_storage_render.cpp
    src/render/storage_list/storage_list_render.cpp

    src/render/storage_view/storage_members_render.cpp
    src/render/storage_view/storage_view_render.cpp
    src/render/storage_view/ingredients/list.cpp
    src/render/storage_view/ingredients/search.cpp

    src/render/recipes_suggestion/select_storages_render.cpp
    src/render/recipes_suggestion/recipes_suggestion_render.cpp
    src/render/recipes_suggestion/recipe_view_render.cpp

    src/render/shopping_list/shopping_list_creation_render.cpp
    src/render/shopping_list/shopping_list_view_render.cpp
)
