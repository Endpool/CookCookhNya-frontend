#include "backend/api/api.hpp"
#include "backend/api/storages.hpp"
#include "handlers.hpp"
#include "states.hpp"
#include "utils.hpp"

#include <tg_stater/bot.hpp>

#include <string>
#include <tg_stater/dependencies.hpp>
using namespace cookcookhnya::api;
int main() {
    using namespace tg_stater;
    using namespace cookcookhnya;
    using namespace cookcookhnya::handlers;

    Setup<State, Dependencies<ApiClient>>::Stater<noStateHandler,
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
        bot{{},{ApiClient{utils::getenvWithError("API_URL")}}};

    bot.start(TgBot::Bot{utils::getenvWithError("BOT_TOKEN")});
}
