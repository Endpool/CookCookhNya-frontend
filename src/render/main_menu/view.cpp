#include "view.hpp"

#include "backend/api/storages.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <optional>
#include <string>
#include <utility>

namespace cookcookhnya::render::main_menu {

using namespace tg_types;

void renderMainMenu(bool toBeEdited,
                    std::optional<std::optional<std::string>> inviteStorage,
                    UserId userId,
                    ChatId chatId,
                    BotRef bot,
                    api::StorageApiRef storageApi) {
    auto storages = storageApi.getStoragesList(userId);

    auto text = utils::utf8str(
        u8"🍳 Добро пожаловать в CookCookNya — ваш личный бот для быстрого подбора рецептов и многого другого!");
    if (inviteStorage) {
        if (*inviteStorage)
            text += utils::utf8str(u8"\n\nВы были успешно добавлены в хранилище 🍱") + **inviteStorage;
        else
            text += utils::utf8str(u8"\n\nК сожалению, данное приглашение уже было использовано 🥲");
    }

    const std::size_t rowsCount = 5;
    InlineKeyboardBuilder keyboard{rowsCount};
    keyboard << makeCallbackButton(u8"🍱 Хранилища", "storage_list") << NewRow{};
    if (!storages.empty())
        keyboard << makeCallbackButton(u8"😋 Хочу кушать!", "wanna_eat") << NewRow{};
    keyboard << makeCallbackButton(u8"🧾 Список покупок", "shopping_list") << NewRow{}
             << makeCallbackButton(u8"👨‍🍳 Рецепты", "recipes_search") << NewRow{}
             << makeCallbackButton(u8"👤 Личный кабинет", "personal_account");

    if (toBeEdited) {
        if (auto messageId = message::getMessageId(userId))
            bot.editMessageText(text, chatId, *messageId, std::move(keyboard));
    } else {
        auto message = bot.sendMessage(chatId, text, std::move(keyboard));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::main_menu
