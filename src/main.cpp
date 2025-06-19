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
                         StorageCreateButtonHandler,
                         storgeDeleteHandler,
                         StorageDeleteButtonHandler,
                         StorageListButtonHandler,
                         StorageCreateHandler,
                         storageViewButtonHandler,
                         storageMemberViewButtonHandler,
                         memberAdditionDeletionMessageHandler,
                         cancelAddDeleteMemberHandler>
        bot{};

    bot.start(TgBot::Bot{utils::getenvWithError("BOT_TOKEN")});
}
