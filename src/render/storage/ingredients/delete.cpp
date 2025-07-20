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
    auto page = makeCallbackButton(std::format("{} из {}", (pageNo + 1), (maxPageNum + 1)), "dont_handle");
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
        std::string emoji = utils::utf8str(isSelected ? u8"[ + ]" : u8"[ᅠ]");
        const char* actionPrefix = isSelected ? "+" : "-";
        std::string text = std::format("{} {}", emoji, storageIngredients[i].name);
        std::string data = actionPrefix + utils::to_string(storageIngredients[i].id);
        buttons.push_back(makeCallbackButton(text, data));
    }
    return buttons;
}

std::pair<std::string, InlineKeyboardBuilder>
constructMessage(std::vector<api::models::ingredient::Ingredient>& selectedIngredients,
                 std::vector<api::models::ingredient::Ingredient>& storageIngredients,
                 std::size_t pageNo,
                 std::size_t numOfIngredientsOnPage,
                 std::vector<api::models::ingredient::Ingredient>& ingredientsList) {
    std::size_t ingSize = ingredientsList.size();
    const std::size_t maxPageNum =
        std::ceil(static_cast<double>(ingSize) / static_cast<double>(numOfIngredientsOnPage));
    std::size_t buttonRows = std::min(ingSize, numOfIngredientsOnPage);
    if (selectedIngredients.empty()) {
        if (ingSize <= numOfIngredientsOnPage) {
            buttonRows += 1; // + back
        } else {
            buttonRows += 2; // + back + navig
        }
    } else {
        if (ingredientsList.size() <= numOfIngredientsOnPage) {
            buttonRows += 2; // + back & delete + delete with shop
        } else {
            buttonRows += 3; // + back & delete + navig + delete with shop
        }
    }

    std::string text = utils::utf8str(u8"🍅 Выберите ингредиенты для удаления\\.\n\n");
    InlineKeyboardBuilder keyboard{buttonRows};

    for (auto& b : constructIngredientsButton(
             selectedIngredients, storageIngredients, numOfIngredientsOnPage, pageNo * numOfIngredientsOnPage)) {
        keyboard << std::move(b);
        keyboard << NewRow{};
    }

    auto backButton = makeCallbackButton(u8"↩️ Назад", "back");
    auto deleteButton = makeCallbackButton(u8"🗑 Удалить", "delete");
    auto deleteWithShopButton = makeCallbackButton(u8"🛒 Удалить (и добавить в 🗒 Список покупок)", "delete_w_shop");
    if (selectedIngredients.empty()) {
        if (ingredientsList.size() <= numOfIngredientsOnPage && pageNo == 0) {
            keyboard << std::move(backButton);
        } else {
            for (auto& b : constructNavigationButtons(pageNo, maxPageNum)) {
                keyboard << std::move(b);
            }
            keyboard << NewRow{};
            keyboard << std::move(backButton);
        }
    } else {
        if (ingredientsList.size() <= numOfIngredientsOnPage && pageNo == 0) {
            keyboard << std::move(deleteWithShopButton);
            keyboard << NewRow{};
            keyboard << std::move(backButton);
            keyboard << std::move(deleteButton);
        } else {
            for (auto& b : constructNavigationButtons(pageNo, maxPageNum)) {
                keyboard << std::move(b);
            }
            keyboard << NewRow{};
            keyboard << std::move(deleteWithShopButton);
            keyboard << NewRow{};
            keyboard << std::move(backButton);
            keyboard << std::move(deleteButton);
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
                                state.storageIngredients);
    auto text = res.first;
    auto keyboard = res.second;
    if (auto messageId = message::getMessageId(userId))
        bot.editMessageText(text, chatId, *messageId, std::move(keyboard), "MarkdownV2");
}

} // namespace cookcookhnya::render::storage::ingredients
