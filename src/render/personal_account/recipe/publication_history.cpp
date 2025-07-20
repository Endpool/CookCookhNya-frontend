#include "publication_history.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <chrono>
#include <format>
#include <vector>

namespace cookcookhnya::render::personal_account::recipe::publication_history {

void renderPublicationHistory(UserId userId,
                              ChatId chatId,
                              api::RecipeId recipeId,
                              std::string& recipeName,
                              bool isPeek,
                              BotRef bot,
                              RecipesApiRef recipesApi) {
    auto history = recipesApi.getRecipeRequestHistory(userId, recipeId);

    InlineKeyboardBuilder keyboard{2}; // confirm and back

    std::string toPrint;
    toPrint = (utils::utf8str(u8"История запросов на публикацию *") + recipeName + "*\n");
    if (!history.empty()) {
        toPrint += utils::utf8str(u8"ℹ️ Текущий статус: ") + utils::to_string(history[0].status) +
                   (history[0].reason.has_value() ? std::format(" по причине {}", history[0].reason.value()) : " ") +
                   utils::to_string(history[0].created) + "\n\n";
        // Remove the lastest history instance as it's showed differently
        history.erase(history.begin());

        for (auto& req : history) {
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
} // namespace cookcookhnya::render::personal_account::recipe::publication_history

// Uncomment in case of EMERGENCY (or if you know what for is)
// use instead of history | reverse | ...
/*
const std::vector<std::string> prefixes = {utils::utf8str(u8"Статус"),
                                                utils::utf8str(u8"по причине"),
                                                utils::utf8str(u8"запрос создан"),
                                                utils::utf8str(u8"последенее обновление")};

        // What if you forgot what for loop is? Хы-хы (Yes it's probably possible to zip "fields" and "prefixes" and
        // interate through it but it would require "for" and i don't know what is it) reverse to print lastest request
    the
        // last in list
        auto res =
            history | reverse | transform([&prefixes](const api::models::recipe::PublicationHistoryRecipe& req) {
                //!!!!IMPORTANT See that tie to match prefixes!!!!
                auto fields = std::tie(req.status, req.reason, req.created, req.updated);
                return iota(0U, prefixes.size()) | transform([fields, &prefixes](size_t idx) -> std::string {
                        switch (idx) {
                        case 0:
                            // statusStr to convert enum to string
                            return prefixes[0] + ": " + utils::to_string(std::get<0>(fields)) + " ";
                        case 1:
                            if (std::get<1>(fields).has_value()) {
                                return prefixes[1] + ": " + std::get<1>(fields).value() + " ";
                            }
                            return ", ";
                        // Need to work with chrono
                        case 2:
                            return prefixes[2] + ": " + utils::to_string(std::get<2>(fields)) + ", ";
                        case 3:
                            if (std::get<3>(fields).has_value()) {
                                return prefixes[3] + ": " + utils::to_string(std::get<3>(fields).value()) + "\n\n";
                            }
                            return "\n\n";
                        default:
                            return "";
                        }
                    });
            }) |
            join; // Join here instead of in the loop

        std::ranges::for_each(res, [&toPrint](const std::string& s) { toPrint += s; });
    }
*/
