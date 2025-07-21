#pragma once

#include "states.hpp"

#include <tg_stater/handler/type.hpp>
#include <tgbot/Api.h>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/InlineQuery.h>
#include <tgbot/types/Message.h>

namespace cookcookhnya::handlers {

// Reimports of states
using states::MainMenu;

using states::PersonalAccountMenu;

using states::CustomIngredientConfirmation;
using states::CustomIngredientCreationEnterName;
using states::CustomIngredientPublish;
using states::CustomIngredientsList;

using states::StorageList;

using states::RecipeStorageAddition;
using states::RecipeView;

using states::StorageCreationEnterName;
using states::StorageDeletion;
using states::StorageView;

using states::StorageMemberAddition;
using states::StorageMemberDeletion;
using states::StorageMemberView;

using states::StorageIngredientsDeletion;
using states::StorageIngredientsList;

using states::StoragesSelection;
using states::SuggestedRecipesList;

using states::ShoppingListCreation;
using states::ShoppingListIngredientSearch;
using states::ShoppingListStorageSelectionToBuy;
using states::ShoppingListView;

using states::CreateCustomRecipe;
using states::CustomRecipeIngredientsSearch;
using states::CustomRecipesList;
using states::RecipeCustomView;

// Type aliases
using BotRef = const TgBot::Api&;
using SMRef = const states::StateManager&;
using MessageRef = const TgBot::Message&;
using CallbackQueryRef = const TgBot::CallbackQuery&;
using InlineQueryRef = const TgBot::InlineQuery&;

} // namespace cookcookhnya::handlers
