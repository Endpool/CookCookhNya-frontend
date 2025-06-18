#include "handlers.hpp"
#include "states.hpp"
#include "types.hpp"
#include "utils.hpp"

#include <tg_stater/bot.hpp>

#include <string>

int main() {
    using namespace tg_stater;

    Setup<State, Dependencies<HelloWorldProvider>>::Stater<helloWorldHandler,
                                                           noStateHandler,
                                                           startHandler,
                                                           StorageListButtonHandler,
                                                           StorageCreateHandler,
                                                           StorageCreateButtonHandler,
                                                           storgeDeleteHandler,
                                                           StorageDeleteButtonHandler>
        bot{//, StorageCreateButtonHandler, storgeDeleteHandler, StorageDeleteButtonHandler, packListButtonHandler,
            //StorageCreateHandler
            {},
            {HelloWorldProvider{utils::getenvWithError("API_URL")}}};

    bot.start(TgBot::Bot{utils::getenvWithError("BOT_TOKEN")});
}
