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
        std::string rcpIngRender;
        if (req.requestType == "recipe")
            rcpIngRender = utils::utf8str(u8"📖");
        else
            rcpIngRender = utils::utf8str(u8"🥬");
        toPrint += std::format(
            "{} {}: *{}* статус: {} ", rcpIngRender, req.requestType, req.name, utils::to_string(req.status));
        if (req.reason.has_value())
            toPrint += std::format("по причине: {} ", req.reason.value());
        toPrint += std::format("запрос создан: {} ", utils::to_string(req.created));
        if (req.updated.has_value()) {
            toPrint += std::format("последенее обновление: {}", utils::to_string(req.updated.value()));
        }
        toPrint += "\n\n";
    }

    keyboard << makeCallbackButton(u8"↩️ Назад", "back");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(toPrint, chatId, *messageId, std::move(keyboard), "Markdown");
    }
}
} // namespace cookcookhnya::render::personal_account
