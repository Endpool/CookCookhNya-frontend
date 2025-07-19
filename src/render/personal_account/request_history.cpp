#include "request_history.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"
#include "view.hpp"

#include <cstddef>
#include <utility>

namespace cookcookhnya::render::personal_account::history {
void renderRequestHistory(
    UserId userId, size_t pageNo, size_t numOfInstances, ChatId chatId, BotRef bot, RecipesApiRef recipeApi) {
    InlineKeyboardBuilder keyboard{1};

    auto history = recipeApi.getAllRequestHistory(userId, 10, pageNo * numOfInstances);
    std::string toPrint;
    for (auto& req : history) {
        toPrint += std::format("Статус: {} ", utils::to_string(req.status));
        if (req.reason.has_value())
            toPrint += std::format("по причине: {} ", req.reason.value());
        toPrint += std::format("запрос создан: {}\n", utils::to_string(req.created));
        toPrint += '\n';
    }
}
} // namespace cookcookhnya::render::personal_account::history
