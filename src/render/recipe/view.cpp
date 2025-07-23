#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/u8format.hpp"

#include <boost/url/url.hpp>

#include <format>
#include <string>
#include <utility>

namespace cookcookhnya::render::recipe {

void renderRecipeView(const api::models::recipe::RecipeDetails& recipe,
                      const api::RecipeId& recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot) {
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
    const std::string botAlias = bot.getUnderlying().getMe()->username;
    const std::string recipeUrl = std::format("https://t.me/{}?start=recipe_{}", botAlias, recipeId);
    const std::string shareText = utils::u8format("{} **{}**", u8"Хочу поделиться с тобой рецептом", recipe.name);

    boost::urls::url url{"https://t.me/share/url"};
    url.params().append({"url", recipeUrl});
    url.params().append({"text", shareText});
    shareButton->url = url.buffer();

    keyboard << std::move(shareButton) << NewRow{} << makeCallbackButton(u8"↩️ Назад", "back");

    if (auto mMessageId = message::getMessageId(userId))
        bot.editMessageText(text, chatId, *mMessageId, std::move(keyboard), "Markdown");
    else {
        auto messageId = bot.sendMessage(chatId, text, std::move(keyboard), "Markdown")->messageId;
        message::addMessageId(userId, messageId);
    }
}

} // namespace cookcookhnya::render::recipe
