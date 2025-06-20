#include "initialStart.hpp"

#include "../../render/storageList/storageListRender.hpp"

namespace cookcookhnya::handlers::init {
using namespace cookcookhnya::forHandlers;
using namespace cookcookhnya::render::storageList;

void start(MessageRef m, BotRef bot, SMRef stateManager, BackendApiRef api) {
    stateManager.put(StorageList{});
    renderStorageList(m.from->id, m.chat->id, bot, api);
    std::cerr << "hello.";
};

void handleNoState(MessageRef m, BotRef bot) {
    if (m.text.starts_with("/start"))
        return;
    bot.sendMessage(m.chat->id, "Use /start please");
};
}  // namespace cookcookhnya::handlers::init
