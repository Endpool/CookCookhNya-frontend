#include "publication_history.hpp"

#include "backend/api/recipes.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <ranges>
#include <sstream>
#include <vector>

namespace cookcookhnya::render::personal_account::publication_history {

using namespace std::views;

void renderPublicationHistory(
    UserId userId, ChatId chatId, api::RecipeId recipeId, BotRef bot, RecipesApiRef recipesApi) {
    // auto history = recipesApi.getModerationHistory(userId, recipeId);
    std::vector<api::models::recipe::CustomRecipePublication> history = {{
        .created = "asd",
        .updated = "12342",
        .reason = "gddf",
        .status = api::models::recipe::PublicationRequestStatus::Pending,
    }};
    InlineKeyboardBuilder keyboard{2}; // confirm and back

    std::ostringstream toPrint;
    toPrint << utils::utf8str(u8"История запросов на публикацию данного рецепта\n");
    const std::vector<std::string> prefixes = {"Статус", "Обновлен", "Создан", "Причина"};
    const std::vector<std::string> statusStr = {"На рассмотрении", "Принят", "Отклонен"};
    // What if i forgot what for loop is? Хы-хы (Yes it's probably possible to zip "fields" and "prefixes" and interate
    // through it but it would require "for" and i don't know what is it)
    // reverse to print lastest request the last in list
    auto res =
        history | reverse | transform([&prefixes, &statusStr](const api::models::recipe::CustomRecipePublication& req) {
            //!!!!IMPORTANT See that tie to match prefixes!!!!
            auto fields = std::tie(req.status, req.updated, req.created, req.reason);
            return iota(0U, prefixes.size()) | transform([fields, &prefixes, &statusStr](size_t idx) -> std::string {
                       switch (idx) {
                       case 0:
                           // statusStr to convert enum to string
                           return prefixes[0] + ": " + statusStr[static_cast<int>(std::get<0>(fields))];
                       case 1:
                           return prefixes[1] + ": " + std::get<1>(fields);
                       case 2:
                           return prefixes[2] + ": " + std::get<2>(fields);
                       case 3: {
                           // Check if optional field is filled
                           if (std::get<3>(fields).has_value()) {
                               return prefixes[3] + ": " + std::get<3>(fields).value();
                           }
                           return "";
                       }
                       default:
                           return "";
                       }
                   });
        }) |
        join; // Join here instead of in the loop

    std::ranges::copy(res, std::ostream_iterator<std::string>(toPrint, "\n"));

    // Text is created moving to the markup
    keyboard << makeCallbackButton(u8"▶️Подтвердить", "confirm") << NewRow{};
    keyboard << makeCallbackButton(u8"↩️ Назад", "back");

    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(toPrint.str(), chatId, *messageId, std::move(keyboard));
    }
}
} // namespace cookcookhnya::render::personal_account::publication_history
