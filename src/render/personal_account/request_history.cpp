#include "request_history.hpp"

#include "backend/models/request_history.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"

#include <cstddef>
#include <format>
#include <vector>

namespace cookcookhnya::render::personal_account::publication_history {
void renderRequestHistory(UserId userId,
                          size_t pageNo,
                          size_t numOfInstances,
                          ChatId chatId,
                          BotRef bot,
                          RequestHistoryApiRef reqHistoryApi) {
    InlineKeyboardBuilder keyboard{1};

    auto history = reqHistoryApi.getAllRequestHistory(userId, 10, pageNo * numOfInstances);
    /*std::vector<api::models::request_history::PublicationHistoryInstance> history = {
        {
            .name = "хуй слона",
            .created = std::chrono::system_clock::now(),
            .status = api::models::recipe::PublicationRequestStatus::PENDING,
        },
        {
            .name = "хуй asd",
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::REJECTED,
            .updated = std::chrono::system_clock::now(),
        },
        {
            .name = "jtgg",
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::REJECTED,
        },
        {
            .name = "jghggf",
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::REJECTED,
            .updated = std::chrono::system_clock::now(),
        },
        {
            .name = "hjfhg",
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::REJECTED,
            .updated = std::chrono::system_clock::now(),
        },
        {
            .name = "loijh",
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::REJECTED,
            .updated = std::chrono::system_clock::now(),
        },
        {
            .name = "ngsdf",
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::REJECTED,
            .updated = std::chrono::system_clock::now(),
        }};*/
    std::string toPrint = utils::utf8str(u8"ℹ️История запросов на публикацию ваших рецептов и ингредиентов\n\n\n");
    for (auto& req : history) {
        toPrint += std::format("*{}* статус: {} ", req.name, utils::to_string(req.status));
        if (req.reason.has_value())
            toPrint += std::format("по причине: {} ", req.reason.value());
        toPrint += std::format("запрос создан: {} ", utils::to_string(req.created));
        if (req.updated.has_value()) {
            toPrint += std::format("последенее обновление {}", utils::to_string(req.updated.value()));
        }
        toPrint += "\n\n";
    }

    keyboard << makeCallbackButton(u8"↩️ Назад", "back");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(toPrint, chatId, *messageId, std::move(keyboard), "Markdown");
    }
}
} // namespace cookcookhnya::render::personal_account::publication_history
