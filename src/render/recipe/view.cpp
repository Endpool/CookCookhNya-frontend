#include "view.hpp"

#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/u8format.hpp"

#include <format>
#include <string>
#include <utility>

namespace cookcookhnya::render::recipe {

void renderRecipeView(const api::models::recipe::RecipeDetails& recipe, UserId userId, ChatId chatId, BotRef bot) {
    std::string text = utils::u8format("{} *{}*\n\n{}", u8"📖 Рецепт", recipe.name, u8"Ингредиенты:\n");
    for (const auto& ing : recipe.ingredients)
        text += utils::u8format("{} {}\n", u8"•", ing.name);
    if (recipe.link)
        text += utils::u8format("\n{}: {}\n", u8"🌐 Источник", *recipe.link);
    if (recipe.creator)
        text += utils::u8format("\n{}: {}\n", u8"👤 Автор", recipe.creator->fullName);

    InlineKeyboardBuilder keyboard{2}; // share, back

    auto shareButton = std::make_shared<TgBot::InlineKeyboardButton>();
    shareButton->text = utils::utf8str(u8"📤 Поделиться");
    const std::string telegramBotAlias = bot.getUnderlying().getMe()->username;
    const std::string inviteText = utils::u8format(
        "{} **{}**\nhttps://t.me/{}?start={}", u8"Хочу поделиться с тобой рецептом", recipe.name, telegramBotAlias, "");
    shareButton->url = "https://t.me/share/url?url="
                       "%D0%9D%D0%B0%D0%B6%D0%BC%D0%B8%20%D0%BD%D0%B0%20%D1%81%D1%81%D1%8B%D0%BB%D0%BA%D1%83%2C%20%D1%"
                       "87%D1%82%D0%BE%D0%B1%D1%8B%20%D1%81%D1%82%D0%B0%D1%82%D1%8C%20%D1%83%D1%87%D0%B0%D1%81%D1%82%"
                       "D0%BD%D0%B8%D0%BA%D0%BE%D0%BC%20%D1%85%D1%80%D0%B0%D0%BD%D0%B8%D0%BB%D0%B8%D1%89%D0%B0%20%F0%"
                       "9F%8D%B1%2A%2Aasd%2A%2A%20%D0%B2%20CookCookhNya%21%0Ahttps%3A%2F%2Ft.me%2Fstage_stand_bot%"
                       "3Fstart%3Dabf48eb2a98a1798e113b896b39b506c8c06bc8406449e3356aa0199f6099f23";
    keyboard << std::move(shareButton) << NewRow{} << makeCallbackButton(u8"↩️ Назад", "back");

    if (auto mMessageId = message::getMessageId(userId))
        bot.editMessageText(text, chatId, *mMessageId, std::move(keyboard), "Markdown");
}

} // namespace cookcookhnya::render::recipe
