#include "delete.hpp"

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <utility>
#include <vector>

namespace cookcookhnya::render::storage::members {

void renderStorageMemberDeletion(
    const api::StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, api::StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);

    auto members = storageApi.getStorageMembers(userId, storageId);
    const std::size_t buttonRows = members.size();
    InlineKeyboard keyboard(buttonRows);
    keyboard[0].push_back(makeCallbackButton(u8"↩️ Назад", "cancel_member_deletion"));
    size_t k = 1;
    for (auto& member : members) {
        if (member.userId != storage.ownerId) {
            keyboard[k++].push_back(makeCallbackButton(std::format("{} {}", utils::utf8str(u8"👤"), member.fullName),
                                                       "mem_" + utils::to_string(member.userId)));
        }
    }

    auto text = utils::utf8str(u8"🚷 Выберите участника для удаления\n");
    auto messageId = message::getMessageId(userId);
    if (messageId)
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::storage::members
