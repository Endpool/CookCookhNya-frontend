#include "backend/api/api.hpp"
#include "handlers/handlers_list.hpp"
#include "states.hpp"
#include "utils/parsing.hpp"
#include "utils/utils.hpp"

#include <tg_stater/bot.hpp>
#include <tg_stater/dependencies.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <tgbot/types/InputFile.h>
#include <utility>

int main(int argc, char* argv[]) {
    using namespace std::literals;
    using namespace tg_stater;
    using namespace cookcookhnya;
    using namespace cookcookhnya::handlers::bot_handlers;
    using namespace cookcookhnya::states;
    using namespace cookcookhnya::api;

    bool useWebhook = false;
    if (argc > 1) {
        if (std::find(argv + 1, argv + argc, "--webhook"sv) == argv + argc) {
            std::cout << "The only option is \"--webhook\"\n";
            return 1;
        }
        useWebhook = true;
    }

    Setup<State, Dependencies<ApiClient>>::Stater<startCmdHandler,
                                                  noStateHandler,
                                                  mainMenuCQHandler,
                                                  customIngredientsListCQHandler,
                                                  customIngredientCreationEnterNameMsgHandler,
                                                  customIngredientCreationEnterNameCQHandler,
                                                  customIngredientConfirmationCQHandler,
                                                  customIngredientPublishCQHandler,
                                                  storageListCQHandler,
                                                  storageCreationEnterNameMsgHandler,
                                                  storageCreationEnterNameCQHandler,
                                                  storageDeletionCQHandler,
                                                  storageViewCQHandler,
                                                  storageMemberViewCQHandler,
                                                  storageMemberAdditionMsgHandler,
                                                  storageMemberAdditionCQHandler,
                                                  storageMemberDeletionCQHandler,
                                                  storageSelectionCQHandler,
                                                  storageIngredientsListCQHandler,
                                                  storageIngredientsListIQHandler,
                                                  suggestedRecipeListCQHandler,
                                                  recipeViewCQHandler,
                                                  recipeStorageAdditionCQHandler,
                                                  shoppingListCreationCQHandler,
                                                  shoppingListViewCQHandler,
                                                  personalAccountMenuCQHandler,
                                                  customRecipesListCQHandler,
                                                  createCustomRecipeMsgHandler,
                                                  createCustomRecipeCQHandler,
                                                  recipeCustomViewCQHandler,
                                                  customRecipeIngredientsSearchCQHandler,
                                                  customRecipeIngredientsSearchIQHandler>
        bot{{}, {ApiClient{utils::getenvWithError("API_URL")}}};

    TgBot::Bot tgBot{utils::getenvWithError("BOT_TOKEN")};
    if (useWebhook) {
        std::string path = "/"s + utils::getenvWithError("WEBHOOK_SECRET");
        bot.startWebhook(
            std::move(tgBot),
            utils::parse<unsigned short>(utils::getenvWithError("WEBHOOK_PORT")),
            utils::getenvWithError("WEBHOOK_HOST") + path,
            path,
            TgBot::InputFile::fromFile(utils::getenvWithError("WEBHOOK_CERTIFICATE_PATH"), "application/x-pem-file"));
    } else {
        bot.start(std::move(tgBot));
    }
}
