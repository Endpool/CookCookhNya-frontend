#include "delete.hpp"

#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::storage::delete_member {

void renderMemberDeletionPrompt(
    const api::StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);

    auto members = storageApi.getStorageMembers(userId, storageId);
    const unsigned int buttonRows = members.size();
    InlineKeyboard keyboard(buttonRows);
    keyboard[0].push_back(detail::makeCallbackButton(u8"↩️ Назад", "cancel_member_deletion"));
    size_t k = 1;
    for (auto& member : members) {
        if (member.userId != storage.ownerId) {
            keyboard[k++].push_back(detail::makeCallbackButton(
                std::format("{} {}", utils::utf8str(u8"👤"), member.fullName), "mem_" + std::to_string(member.userId)));
        }
    }

    auto text = utils::utf8str(u8"🚷 Выберите участника для удаления\n");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    }
};

} // namespace cookcookhnya::render::storage::delete_member
