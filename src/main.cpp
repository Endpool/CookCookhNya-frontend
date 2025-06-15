#include <httplib.h>
#include <tg_stater/bot.hpp>
#include <tg_stater/dependencies.hpp>
#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>
#include <tg_stater/handler/type.hpp>
#include <tgbot/Api.h>
#include <tgbot/Bot.h>
#include <tgbot/types/Message.h>

#include <format>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

#include "utils.hpp"

#include "states.hpp"

namespace {

using namespace TgBot;
using namespace tg_stater;

class HelloWorldProvider {
    mutable httplib::Client api;

  public:
    explicit HelloWorldProvider(const std::string& apiUrl) : api{apiUrl} {}

    [[nodiscard]] std::string getHelloWorld() const {
        auto res = api.Get("/hello");
        if (!res)
            throw std::runtime_error(std::format("Error getting request: {}", static_cast<int>(res.error())));
        if (res->status != httplib::StatusCode::OK_200)
            throw std::runtime_error(std::format("Error getting request: {}", res->body));
        return std::move(res->body);
    }
};

void helloWorld(const Message& m, const Api& bot, const HelloWorldProvider& hwp) {
    bot.sendMessage(m.chat->id, hwp.getHelloWorld());
}

using helloWorldHandler = Handler<Events::Message{}, helloWorld, HandlerTypes::AnyState{}>;

} // namespace

int main() {
    using namespace tg_stater;

    Setup<State, Dependencies<HelloWorldProvider>>::Stater<helloWorldHandler> bot{
        {}, {HelloWorldProvider{utils::getenvWithError("API_URL")}}};

    bot.start(TgBot::Bot{utils::getenvWithError("BOT_TOKEN")});
}
