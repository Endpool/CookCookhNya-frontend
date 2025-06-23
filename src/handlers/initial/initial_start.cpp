#include "initial_start.hpp"

#include "handlers/type_refs.hpp"
#include "render/storage_list/storage_list_render.hpp"

namespace cookcookhnya::handlers::init {
using namespace cookcookhnya::forHandlers;
using namespace cookcookhnya::render::storageList;

void start(MessageRef m, BotRef bot, SMRef stateManager, UserApiRef userApi, StorageApiRef storageApi) {
    stateManager.put(StorageList{});
    renderStorageList(m.from->id, m.chat->id, bot, storageApi);
    userApi.create(m.from->id, models::user::UserCreateBody{.name=m.from->username, .id=m.from->id});
    std::cerr << "start handled";
};

void handleNoState(MessageRef m, BotRef bot) {
    if (m.text.starts_with("/start"))
        return;
    bot.sendMessage(m.chat->id, "Use /start please");
};
} // namespace cookcookhnya::handlers::init
