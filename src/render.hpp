#include "utils.hpp"
#include <tgbot/Api.h>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/InlineKeyboardButton.h>
#include <tgbot/types/Message.h>


#include <format>
#include <iterator>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>




namespace detail {
    inline std::shared_ptr<TgBot::InlineKeyboardButton> makeCallbackButton(std::string_view text, std::string_view data) {
        TgBot::InlineKeyboardButton button{};
        button.text = text;
        button.callbackData = data;
        return utils::make_shared(std::move(button));
    }
}
