#include "delete.hpp"

#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "states.hpp"

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

namespace cookcookhnya::render::storage::ingredients {

namespace {

std::vector<TgBot::InlineKeyboardButton::Ptr> constructNavigationButtons(std::size_t pageNo, std::size_t maxPageNum) {
    std::vector<TgBot::InlineKeyboardButton::Ptr> buttons;
    auto forward = makeCallbackButton(u8"▶️", "next");
    auto backward = makeCallbackButton(u8"◀️", "prev");
    auto dont_handle = makeCallbackButton(u8"ㅤ", "dont_handle");
    auto page = makeCallbackButton(std::format("{} из {}", (pageNo + 1), maxPageNum), "dont_handle");
    if (pageNo == maxPageNum) {
        buttons.push_back(backward);
        buttons.push_back(page);
        buttons.push_back(dont_handle);
    } else if (pageNo == 0) {
        buttons.push_back(dont_handle);
        buttons.push_back(page);
        buttons.push_back(forward);
    } else {
        buttons.push_back(backward);
        buttons.push_back(page);
        buttons.push_back(forward);
    }
    return buttons;
}

std::vector<TgBot::InlineKeyboardButton::Ptr>
constructIngredientsButton(std::vector<api::models::ingredient::Ingredient>& selectedIngredients,
                           std::vector<api::models::ingredient::Ingredient>& storageIngredients,
                           std::size_t size,
                           std::size_t offset) {
    size = std::min(offset + size, storageIngredients.size()) - offset;
    std::vector<TgBot::InlineKeyboardButton::Ptr> buttons;
    for (std::size_t i = offset; i != size + offset; ++i) {
        const bool isSelected = std::ranges::contains(
            selectedIngredients, storageIngredients[i].id, &api::models::ingredient::Ingredient::id);
        const std::string emoji = utils::utf8str(isSelected ? u8"[ + ]" : u8"[ᅠ]");
        const char* actionPrefix = isSelected ? "+" : "-";
        const std::string text = std::format("{} {}", emoji, storageIngredients[i].name);
        const std::string data = actionPrefix + utils::to_string(storageIngredients[i].id);
        buttons.push_back(makeCallbackButton(text, data));
    }
    return buttons;
}

std::pair<std::string, InlineKeyboardBuilder>
constructMessage(std::vector<api::models::ingredient::Ingredient>& selectedIngredients, // NOLINT(*complexity*)
                 std::vector<api::models::ingredient::Ingredient>& storageIngredients,
                 std::size_t pageNo,
                 std::size_t numOfIngredientsOnPage,
                 bool withoutPutToShoppingListButton) {
    const std::size_t ingSize = storageIngredients.size();
    const std::size_t maxPageNum =
        std::ceil(static_cast<double>(ingSize) / static_cast<double>(numOfIngredientsOnPage));
    std::size_t buttonRows = std::min(ingSize, numOfIngredientsOnPage);
    if (selectedIngredients.empty()) {
        if (ingSize <= numOfIngredientsOnPage)
            buttonRows += 1; // + back
        else
            buttonRows += 2; // + back + navig
    } else {
        if (ingSize <= numOfIngredientsOnPage) {
            if (withoutPutToShoppingListButton)
                buttonRows += 2; // + back + delete
            else
                buttonRows += 3; // + back + delete + shop
        } else {
            if (withoutPutToShoppingListButton)
                buttonRows += 3; // + back + navig + delete
            else
                buttonRows += 4;
        }
    }

    const std::string text = utils::utf8str(u8"🍅 Выберите ингредиенты для удаления\\.\n\n");
    InlineKeyboardBuilder keyboard{buttonRows};

    for (auto& b : constructIngredientsButton(
             selectedIngredients, storageIngredients, numOfIngredientsOnPage, pageNo * numOfIngredientsOnPage)) {
        keyboard << std::move(b) << NewRow{};
    }

    auto backButton = makeCallbackButton(u8"↩️ Назад", "back");
    auto deleteButton = makeCallbackButton(u8"🗑 Удалить", "delete");
    auto shopButton = makeCallbackButton(u8"🧾 Добавить в Список покупок", "put_to_shop");
    if (selectedIngredients.empty()) {
        if (ingSize <= numOfIngredientsOnPage && pageNo == 0) {
            keyboard << std::move(backButton);
        } else {
            for (auto& b : constructNavigationButtons(pageNo, maxPageNum)) {
                keyboard << std::move(b);
            }
            keyboard << NewRow{};
            keyboard << std::move(backButton);
        }
    } else {
        if (ingSize <= numOfIngredientsOnPage && pageNo == 0) {
            if (!withoutPutToShoppingListButton) {
                keyboard << std::move(shopButton);
                keyboard << NewRow{};
            }
            keyboard << std::move(deleteButton);
            keyboard << NewRow{};
            keyboard << std::move(backButton);
        } else {
            for (auto& b : constructNavigationButtons(pageNo, maxPageNum)) {
                keyboard << std::move(b);
            }
            keyboard << NewRow{};
            if (!withoutPutToShoppingListButton) {
                keyboard << std::move(shopButton);
                keyboard << NewRow{};
            }
            keyboard << std::move(deleteButton);
            keyboard << NewRow{};
            keyboard << std::move(backButton);
        }
    }
    return std::make_pair(text, keyboard);
}

} // namespace

void renderStorageIngredientsDeletion(states::StorageIngredientsDeletion& state,
                                      UserId userId,
                                      ChatId chatId,
                                      BotRef bot) {

    const std::size_t numOfIngredientsOnPage = 7;

    auto res = constructMessage(state.selectedIngredients,
                                state.storageIngredients,
                                state.pageNo,
                                numOfIngredientsOnPage,
                                state.addedToShopList);
    auto text = res.first;
    auto keyboard = res.second;
    if (auto messageId = message::getMessageId(userId))
        bot.editMessageText(text, chatId, *messageId, std::move(keyboard), "MarkdownV2");
}

} // namespace cookcookhnya::render::storage::ingredients
