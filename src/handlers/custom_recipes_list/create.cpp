#include "create.hpp"

#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/storage_list/view.hpp"
#include "utils.hpp"
#include <optional>

namespace cookcookhnya::handlers::create_custom_recipe {

using namespace render::storage_list;

void createRecipe(
    CreateCustomRecipe& /*unused*/, MessageRef m, BotRef bot, SMRef stateManager, RecipesApiRef recipeApi) {
    storageApi.create(m.from->id, api::models::storage::StorageCreateBody{m.text});
    /*auto text = utils::utf8str(u8"🏷 Введите новое имя хранилища");
    auto messageId = message::getMessageId(m.from->id);
    if (messageId) {
        bot.editMessageText(text, m.chat->id, *messageId);
    }*/
    renderStorageList(false, m.from->id, m.chat->id, bot, storageApi);
    stateManager.put(StorageList{});
};

void cancelRecipeCreation(
    CreateCustomRecipe& /*unused*/, CallbackQueryRef cq, BotRef bot, SMRef stateManager, RecipesApiRef recipeApi) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "cancel_storage_creation") {
        renderStorageList(true, cq.from->id, cq.message->chat->id, bot, storageApi);
        stateManager.put(StorageList{});
    }
};

} // namespace cookcookhnya::handlers::create_custom_recipe
