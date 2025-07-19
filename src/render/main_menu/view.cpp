#include "view.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <optional>
#include <utility>

namespace cookcookhnya::render::main_menu {

using namespace tg_types;

void renderMainMenu(bool toBeEdited,
                    std::optional<std::optional<std::string>> inviteStorage,
                    UserId userId,
                    ChatId chatId,
                    BotRef bot,
                    StorageApiRef storageApi) {
    auto storages = storageApi.getStoragesList(userId);

    const std::size_t buttonRows = storages.empty() ? 3 : 4;
    InlineKeyboard keyboard(buttonRows);

    if (!storages.empty()) {
        keyboard[0].push_back(makeCallbackButton(u8"🍱 Хранилища", "storage_list"));
        keyboard[1].push_back(makeCallbackButton(u8"😋 Хочу кушать!", "wanna_eat"));
        keyboard[2].push_back(makeCallbackButton(u8"🗒 Список покупок", "shopping_list"));
        keyboard[3].push_back(makeCallbackButton(u8"👤 Личный кабинет", "personal_account"));
    } else {
        keyboard[0].push_back(makeCallbackButton(u8"🍱 Хранилища", "storage_list"));
        keyboard[1].push_back(makeCallbackButton(u8"🗒 Список покупок", "shopping_list"));
        keyboard[2].push_back(makeCallbackButton(u8"👤 Личный кабинет", "personal_account"));
    }

    auto text = utils::utf8str(
        u8"🍳 Добро пожаловать в CookCookNya — ваш личный бот для быстро подбора рецептов и многого другого!");
    if (inviteStorage) {
        if (*inviteStorage) {
            text += utils::utf8str(u8"\n\nВы были успешно добавлены в хранилище 🍱") + **inviteStorage;
        } else {
            text += utils::utf8str(u8"\n\nК сожалению, данное приглашение уже было использовано 🥲");
        }
    }
    if (toBeEdited) {
        if (auto messageId = message::getMessageId(userId))
            bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
    } else {
        auto message = bot.sendMessage(chatId, text, makeKeyboardMarkup(std::move(keyboard)));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::main_menu
