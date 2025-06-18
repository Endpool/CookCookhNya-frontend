#include "handlers.hpp"
#include "states.hpp"
#include "utils.hpp"

#include <tg_stater/bot.hpp>

#include <string>

int main() {
    using namespace tg_stater;
    using namespace cookcookhnya;
    using namespace cookcookhnya::handlers;

    Setup<State>::Stater<noStateHandler,
                         startHandler,
                         StorageListButtonHandler,
                         StorageCreateHandler,
                         StorageCreateButtonHandler,
                         storgeDeleteHandler,
                         StorageDeleteButtonHandler,
                         StorageCreateButtonHandler,
                         storgeDeleteHandler,
                         StorageDeleteButtonHandler,
                         StorageListButtonHandler,
                         StorageCreateHandler>
        bot{};

    bot.start(TgBot::Bot{utils::getenvWithError("BOT_TOKEN")});
}
