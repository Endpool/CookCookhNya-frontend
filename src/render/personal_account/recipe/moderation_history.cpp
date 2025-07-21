#include "moderation_history.hpp"

#include "backend/id_types.hpp"
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
                              api::RecipeId recipeId,
                              std::string& recipeName,
                              bool isPeek,
                              BotRef bot,
                              api::RecipesApiRef recipesApi) {
    auto history = recipesApi.getRecipeRequestHistory(userId, recipeId);

    InlineKeyboardBuilder keyboard{2}; // confirm and back

    std::string toPrint;
    toPrint = (utils::utf8str(u8"История запросов на публикацию *") + recipeName + "*\n");
    if (!history.empty()) {
        const size_t lastUpdate = history.size() - 1;
        // Construct current status string
        toPrint += utils::utf8str(u8"ℹ️ Текущий статус: ") + utils::to_string(history[lastUpdate].status);
        if (history[lastUpdate].reason.has_value())
            toPrint += std::format(" по причине {}", history[lastUpdate].reason.value());
        toPrint += " " + utils::to_string(history[lastUpdate].created) + "\n\n";

        // Remove the lastest history instance as it's showed differently
        history.erase(history.end());

        for (auto& req : history | reverse) {
            toPrint += std::format("Статус: {} ", utils::to_string(req.status));
            if (req.reason.has_value())
                toPrint += std::format("по причине: {} ", req.reason.value());
            toPrint += std::format("запрос создан: {} ", utils::to_string(req.created));
            if (req.updated.has_value()) {
                toPrint += std::format("последенее обновление: {}", utils::to_string(req.updated.value()));
            }
            toPrint += "\n\n";
        }
    }

    // Text is created moving to the markup
    // If not peeking then show accept button
    if (!isPeek) {
        keyboard << makeCallbackButton(u8"▶️Подтвердить", "confirm") << NewRow{};
    }
    keyboard << makeCallbackButton(u8"↩️ Назад", "back");

    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(toPrint, chatId, *messageId, std::move(keyboard), "Markdown");
    }
}
} // namespace cookcookhnya::render::personal_account::recipe
