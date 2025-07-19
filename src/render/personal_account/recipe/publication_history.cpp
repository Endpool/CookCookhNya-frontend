#include "publication_history.hpp"

#include "backend/api/recipes.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <chrono>
#include <format>
#include <ranges>
#include <vector>

namespace cookcookhnya::render::personal_account::publication_history {

using namespace std::views;

namespace {
std::string convertTimeToStrFormat(std::chrono::system_clock::time_point time) {
    const auto* moscow_tz = std::chrono::locate_zone("Europe/Moscow");
    auto moscow_time = std::chrono::zoned_time(moscow_tz, time);
    return std::format("{:%d-%m-%Y %H:%M}", moscow_time.get_local_time());
}
} // namespace

void renderPublicationHistory(UserId userId,
                              ChatId chatId,
                              api::RecipeId recipeId,
                              std::string& recipeName,
                              BotRef bot,
                              RecipesApiRef recipesApi) {
    // auto history = recipesApi.getModerationHistory(userId, recipeId);
    std::vector<api::models::recipe::CustomRecipePublication> history = {
        {
            .created = std::chrono::system_clock::now(),
            .status = api::models::recipe::PublicationRequestStatus::Pending,
        },
        {
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::Rejected,
        },
        {
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::Rejected,
        },
        {
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::Rejected,
        },
        {
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::Rejected,
        },
        {
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::Rejected,
        },
        {
            .created = std::chrono::system_clock::now(),
            .reason = "ПИДАРАС",
            .status = api::models::recipe::PublicationRequestStatus::Rejected,
        }};
    InlineKeyboardBuilder keyboard{2}; // confirm and back

    std::string toPrint;
    toPrint = (utils::utf8str(u8"История запросов на публикацию *") + recipeName + "*\n");

    toPrint += utils::utf8str(u8"ℹ️ Текущий статус: ") + utils::to_string(history[0].status) +
               (history[0].reason.has_value() ? std::format(" по причине {}", history[0].reason.value()) : " ") +
               convertTimeToStrFormat(history[0].created) + "\n\n";
    // Remove the lastest history instance as it's showed differently
    history.erase(history.begin());

    const std::vector<std::string> prefixes = {
        utils::utf8str(u8"Статус"), utils::utf8str(u8"по причине"), utils::utf8str(u8"запрос создан")};

    // What if you forgot what for loop is? Хы-хы (Yes it's probably possible to zip "fields" and "prefixes" and
    // interate through it but it would require "for" and i don't know what is it) reverse to print lastest request the
    // last in list
    auto res = history | reverse | transform([&prefixes](const api::models::recipe::CustomRecipePublication& req) {
                   //!!!!IMPORTANT See that tie to match prefixes!!!!
                   auto fields = std::tie(req.status, req.reason, req.created);
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
                              case 2: {
                                  return prefixes[2] + ": " + convertTimeToStrFormat(std::get<2>(fields)) + "\n\n";
                              }
                              default:
                                  return "";
                              }
                          });
               }) |
               join; // Join here instead of in the loop

    std::ranges::for_each(res, [&toPrint](const std::string& s) { toPrint += s; });

    // Text is created moving to the markup
    keyboard << makeCallbackButton(u8"▶️Подтвердить", "confirm") << NewRow{};
    keyboard << makeCallbackButton(u8"↩️ Назад", "back");

    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(toPrint, chatId, *messageId, std::move(keyboard), "Markdown");
    }
}
} // namespace cookcookhnya::render::personal_account::publication_history
