#include "create.hpp"

#include "backend/api/storages.hpp"
#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/storages_list/view.hpp"
#include "utils/utils.hpp"

#include <optional>

namespace cookcookhnya::handlers::storages_list {

using namespace render::storages_list;

void handleStorageCreationEnterNameMsg(
    StorageCreationEnterName& /*unused*/, MessageRef m, BotRef bot, SMRef stateManager, api::StorageApiRef storageApi) {
    storageApi.create(m.from->id, api::models::storage::StorageCreateBody{m.text}); // Create storage body with new name
    auto text = utils::utf8str(u8"🏷 Введите новое имя хранилища");
    auto messageId = message::getMessageId(m.from->id);
    if (messageId) {
        bot.editMessageText(text, m.chat->id, *messageId);
    }
    renderStorageList(false, m.from->id, m.chat->id, bot, storageApi);
    stateManager.put(StorageList{});
};

void handleStorageCreationEnterNameCQ(StorageCreationEnterName& /*unused*/,
                                      CallbackQueryRef cq,
                                      BotRef bot,
                                      SMRef stateManager,
                                      api::StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "back") {
        renderStorageList(true, cq.from->id, cq.message->chat->id, bot, storageApi);
        stateManager.put(StorageList{});
    }
};

} // namespace cookcookhnya::handlers::storages_list
