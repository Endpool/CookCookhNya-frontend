#include "moderation_history.hpp"

#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <chrono>
#include <cstddef>
#include <format>
#include <ranges>
#include <vector>

namespace cookcookhnya::render::personal_account::recipe {

using namespace std::views;

void renderPublicationHistory(UserId userId,
                              ChatId chatId,
                              std::string& recipeName,
                              std::string& errorReport,
                              std::vector<api::models::recipe::RecipePublicationRequest> history,
                              BotRef bot) {

    bool isConfirm = true;
    InlineKeyboardBuilder keyboard{3}; // confirm, back and rules

    std::string toPrint;
    toPrint = (utils::utf8str(u8"История запросов на публикацию *") + recipeName + "*\n\n");
    if (!history.empty()) {
        // Show confirm only if in those states
        isConfirm =
            history[history.size() - 1].status.status == api::models::moderation::PublicationRequestStatus::REJECTED;
        const size_t lastUpdatedInstance = history.size() - 1;
        // Construct current status string
        toPrint +=
            utils::utf8str(u8"ℹ️ Текущий статус: ") + utils::to_string(history[lastUpdatedInstance].status.status);
        if (auto reason = history[lastUpdatedInstance].status.reason) {
            toPrint += std::format(" по причине {}", reason.value());
        }
        toPrint += " " + utils::to_string(history[lastUpdatedInstance].created) + "\n\n";

        // Remove the lastest history instance as it's showed differently
        history.erase(history.end());

        for (auto& req : history | reverse) {
            toPrint += std::format("Статус: {} ", utils::to_string(req.status.status));
            if (req.status.reason.has_value())
                toPrint += std::format("по причине: {} ", req.status.reason.value());
            toPrint += std::format("запрос создан: {} ", utils::to_string(req.created));
            if (req.updated.has_value()) {
                toPrint += std::format("последенее обновление: {}", utils::to_string(req.updated.value()));
            }
            toPrint += "\n\n";
        }
    }

    toPrint += errorReport;

    if (isConfirm) {
        keyboard << makeCallbackButton(u8"▶️Подтвердить", "confirm") << NewRow{};
    }
    keyboard << makeCallbackButton(u8"❗️Правила", "rules") << NewRow{};
    keyboard << makeCallbackButton(u8"↩️ Назад", "back");

    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(toPrint, chatId, *messageId, std::move(keyboard), "Markdown");
    }
}

void renderPublicationRules(UserId userId, ChatId chatId, BotRef bot) {
    // Rules
    const std::string toPrint =
        utils::utf8str(u8"❗️ *Правила публикации рецептов:*") +
        "\n1. *Статус рецепта*\nНельзя отправить запрос на публикацию, если рецепт:\n    - уже принят "
        "(опубликован);\n    - находится на рассмотрении модерации.  \n2. *Ингредиенты*  \n   Запрещено публиковать "
        "рецепт, если в нём есть:\n    - ингредиенты, не прошедшие модерацию (_ожидают проверки_);\n    - "
        "ингредиенты, отклонённые администрацией.\n3. *Название рецепта*\n    - Не должно содержать "
        "нецензурную лексику, оскорбления или спам;\n    - Должно точно отражать содержание рецепта (например: "
        "\"Спагетти карбонара\", а не \"Вкуснятина\").  \n4. *Дополнительно*\n    - Запрещено размещать контактные "
        "данные или рекламу." +
        utils::utf8str(u8"\n⚠️ Нарушение правил приведёт к отклонению рецепта ");

    InlineKeyboardBuilder keyboard{1}; // back
    keyboard << makeCallbackButton(u8"↩️ Назад", "backFromRules");

    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(toPrint, chatId, *messageId, std::move(keyboard), "Markdown");
    }
}
} // namespace cookcookhnya::render::personal_account::recipe
