#include "backend/api/api.hpp"
#include "backend/api/storages.hpp"
#include "handlers/handlers_list.hpp"
#include "states.hpp"
#include "utils.hpp"

#include <tg_stater/bot.hpp>
#include <tg_stater/dependencies.hpp>

#include <string>

int main() {
    using namespace tg_stater;
    using namespace cookcookhnya;
    using namespace cookcookhnya::handlers;
    using namespace cookcookhnya::api;

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
        bot{{}, {ApiClient{utils::getenvWithError("API_URL")}}};

    bot.start(TgBot::Bot{utils::getenvWithError("BOT_TOKEN")});
}
