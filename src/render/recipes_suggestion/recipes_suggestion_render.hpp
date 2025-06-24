#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::recipes_suggestion {
/*
@brief shared function between render and edit of suggestion message
*/
InlineKeyboard constructMarkup(std::vector<StorageId> storages, int pageNo, StorageApiRef storageApi);

void renderRecipesSuggestion(
    std::vector<StorageId> storages, int pageNo, ChatId chatId, BotRef bot, StorageApiRef storageApi);

void editSuggestionMessage(std::vector<StorageId> storages,
                           int pageNo,
                           ChatId chatId,
                           MessageId messageId,
                           BotRef bot,
                           StorageApiRef storageApi);

} // namespace cookcookhnya::render::recipes_suggestion
