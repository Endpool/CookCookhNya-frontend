#include "backend/api/api.hpp"
#include "handlers/handlers_list.hpp"
#include "states.hpp"
#include "utils.hpp"

#include <tg_stater/bot.hpp>
#include <tg_stater/dependencies.hpp>

int main() {
    using namespace tg_stater;
    using namespace cookcookhnya;
    using namespace cookcookhnya::handlers::bot_handlers;
    using namespace cookcookhnya::states;
    using namespace cookcookhnya::api;

    Setup<State, Dependencies<ApiClient>>::Stater<noStateHandler,
                                                  startHandler,
                                                  storageCreateButtonHandler,
                                                  storgeDeleteHandler,
                                                  storageDeleteButtonHandler,
                                                  storageListButtonHandler,
                                                  storageCreateHandler,
                                                  storageViewButtonHandler,
                                                  storageMemberViewButtonHandler,
                                                  memberAdditionDeletionMessageHandler,
                                                  cancelAddDeleteMemberHandler,
                                                  storagesSelectionHandler,
                                                  recipesSuggestionListHandler,
                                                  storageIngredientsListButtonHandler,
                                                  storageIngredientsSearchButtonHandler,
                                                  storageIngredientsSearchHandler,
                                                  recipeViewHandler>
        bot{{}, {ApiClient{utils::getenvWithError("API_URL")}}};

    bot.start(TgBot::Bot{utils::getenvWithError("BOT_TOKEN")});
}
