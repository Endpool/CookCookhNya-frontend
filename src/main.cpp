
#include "types.hpp"
#include <string>


#include "utils.hpp"
#include "states.hpp"
#include "handlers.hpp"
#include <tg_stater/bot.hpp>


int main() {
    using namespace tg_stater;

    Setup<State, Dependencies<HelloWorldProvider>>::Stater<
    helloWorldHandler, 
    noStateHandler, 
    startHandler,
    StorageListButtonHandler,//
    StorageCreateHandler,
    StorageCreateButtonHandler,
    storgeDeleteHandler, 
    StorageDeleteButtonHandler
    > bot{ //, StorageCreateButtonHandler, storgeDeleteHandler, StorageDeleteButtonHandler, packListButtonHandler, StorageCreateHandler
        {}, {HelloWorldProvider{utils::getenvWithError("API_URL")}}};

    bot.start(TgBot::Bot{utils::getenvWithError("BOT_TOKEN")});
}