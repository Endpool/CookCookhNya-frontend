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
#include "handlers.hpp"
#include "states.hpp"


int main() {
    using namespace tg_stater;

    Setup<states::State>::Stater< 
    handlers::startHandler, 
    handlers::storageViewButtonHandler, 
    handlers::storageMemberViewButtonHandler, 
    handlers::memberAdditionDeletionMessageHandler,
    handlers::cancelAddDeleteMemberHandler> bot{};

    bot.start(TgBot::Bot{utils::getenvWithError("BOT_TOKEN")});
}
