#pragma once

#include "render/common.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <cstdint>
#include <format>
#include <memory>
#include <ranges>
#include <type_traits>
#include <utility>

// forward declaration
namespace TgBot {
class InlineKeyboardButton;
} // namespace TgBot

namespace cookcookhnya::render {

template <utils::range_of<std::shared_ptr<TgBot::InlineKeyboardButton>> ItemButtons>
struct PaginationMarkup {
    ItemButtons itemButtons;
    std::shared_ptr<TgBot::InlineKeyboardButton> leftButton;
    std::shared_ptr<TgBot::InlineKeyboardButton> pageButton;
    std::shared_ptr<TgBot::InlineKeyboardButton> rightButton;

    friend InlineKeyboardBuilder& operator<<(InlineKeyboardBuilder& keyboard, PaginationMarkup&& markup) {
        for (auto&& b : std::move(markup).itemButtons)
            keyboard << std::move(b) << NewRow{};
        if (markup.leftButton)
            keyboard << std::move(markup).leftButton << std::move(markup).pageButton << std::move(markup).rightButton
                     << NewRow{};
        return keyboard;
    }
};

/**
 *  Single column list with an extra row of page switching if more than one page.
 */
template <std::ranges::sized_range R, typename ItemButtonMaker>
    requires std::is_invocable_r_v<std::shared_ptr<TgBot::InlineKeyboardButton>,
                                   ItemButtonMaker,
                                   std::ranges::range_reference_t<R>>
auto constructPagination(
    std::size_t pageNo, std::size_t pageSize, std::size_t totalItems, R&& page, ItemButtonMaker&& makeItemButton) {
    using namespace std::views;
    const std::size_t pagesCount = pageSize != 0 ? (totalItems + pageSize - 1) / pageSize : 0; // ceiling
    const bool lastPage = pageNo + 1 >= pagesCount;

    auto itemButtons = std::forward<R>(page) | transform(std::forward<ItemButtonMaker>(makeItemButton));
    std::shared_ptr<TgBot::InlineKeyboardButton> leftButton = nullptr;
    std::shared_ptr<TgBot::InlineKeyboardButton> pageButton = nullptr;
    std::shared_ptr<TgBot::InlineKeyboardButton> rightButton = nullptr;

    if (pagesCount > 1) {
        enum PageArrows : std::uint8_t {
            NOTHING = 0b00U,
            LEFT = 0b01U,
            RIGHT = 0b10U,
            BOTH = 0b11U,
        };

        const char8_t* const emptyText = u8"ㅤ"; // not empty! invisible symbol
        PageArrows b = NOTHING;
        if (pageNo != 0)
            b = static_cast<PageArrows>(b | LEFT);
        if (!lastPage)
            b = static_cast<PageArrows>(b | RIGHT);

        leftButton =
            (b & LEFT) != 0 ? makeCallbackButton(u8"◀️", "page_left") : makeCallbackButton(emptyText, "do_not_handle");
        rightButton =
            (b & RIGHT) != 0 ? makeCallbackButton(u8"▶️", "page_right") : makeCallbackButton(emptyText, "do_not_handle");
        pageButton =
            makeCallbackButton(std::format("{} {} {}", pageNo + 1, utils::utf8str(u8"из"), pagesCount), "dont_handle");
    }

    return PaginationMarkup{
        std::move(itemButtons), std::move(leftButton), std::move(pageButton), std::move(rightButton)};
}

} // namespace cookcookhnya::render
