#include "publication_history.hpp"

#include "backend/models/moderation.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"

#include <cstddef>
#include <format>
#include <ranges>

namespace cookcookhnya::render::personal_account {

using namespace std::views;

void renderRequestHistory(UserId userId,
                          size_t pageNo,
                          size_t numOfInstances,
                          ChatId chatId,
                          BotRef bot,
                          api::ModerationApiRef moderationApi) {
    InlineKeyboardBuilder keyboard{1};

    const std::size_t maxShownItems = 20;
    auto history = moderationApi.getAllPublicationRequests(userId, maxShownItems, pageNo * numOfInstances);

    std::string toPrint = utils::utf8str(u8"ℹ️История запросов на публикацию ваших рецептов и ингредиентов\n\n");
    for (auto& req : history | reverse) {
        std::string emoji = utils::utf8str(req.requestType == "recipe" ? u8"📖" : u8"🥬");
        toPrint += std::format("{} *{}*\nСтатус: {}\n", emoji, req.name, utils::to_string(req.status.status));
        if (req.status.reason.has_value())
            toPrint += std::format("По причине: {}\n", req.status.reason.value());
        toPrint += std::format("Запрос создан: {}\n", utils::to_string(req.created));
        if (req.updated.has_value()) {
            toPrint += std::format("Последенее обновление: {}\n", utils::to_string(req.updated.value()));
        }
        toPrint += "\n\n";
    }

    keyboard << makeCallbackButton(u8"↩️ Назад", "back");
    if (auto messageId = message::getMessageId(userId))
        bot.editMessageText(toPrint, chatId, *messageId, std::move(keyboard), "Markdown");
}

} // namespace cookcookhnya::render::personal_account
