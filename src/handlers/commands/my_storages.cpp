#include "my_storages.hpp"

#include "handlers/common.hpp"
#include "render/storages_list/view.hpp"
#include "states.hpp"

namespace cookcookhnya::handlers::commands {

using namespace render::storages_list;

void handleMyStoragesCmd(MessageRef m, BotRef bot, SMRef stateManager, ApiClientRef api) {
    renderStorageList(false, m.from->id, m.chat->id, bot, api);
    stateManager.put(StorageList{});
};

} // namespace cookcookhnya::handlers::commands
