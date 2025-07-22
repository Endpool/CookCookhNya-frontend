#include "wanna_eat.hpp"

#include "backend/api/api.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/storages_selection/view.hpp"
#include "states.hpp"
#include "utils/utils.hpp"

#include <optional>

namespace cookcookhnya::handlers::commands {

using namespace render::select_storages;
using namespace render::main_menu;
using namespace render::recipes_suggestions;

void handleWannaEatCmd(MessageRef m, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    auto storages = api.getStoragesApi().getStoragesList(m.from->id);
    if (storages.empty()) {
        bot.sendMessage(m.chat->id, utils::utf8str(u8"😔 К сожалению, у вас пока что нет хранилищ."));
        renderMainMenu(false, std::nullopt, m.from->id, m.chat->id, bot, api);
        stateManager.put(MainMenu{});
    } else if (storages.size() == 1) {
        message::deleteMessageId(m.from->id);
        renderRecipesSuggestion({storages}, 0, m.from->id, m.chat->id, bot, api);
        stateManager.put(SuggestedRecipesList{
            .selectedStorages = storages,
            .pageNo = 0,
            .fromStorage = false,
        });
    } else {
        message::deleteMessageId(m.from->id);
        auto newState = StoragesSelection{};
        renderStorageSelection(newState, m.from->id, m.chat->id, bot, api);
        stateManager.put(newState);
    }
};

} // namespace cookcookhnya::handlers::commands
