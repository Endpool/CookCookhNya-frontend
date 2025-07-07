#pragma once

#include "backend/api/api.hpp"
#include "backend/api/ingredients.hpp"
#include "backend/api/recipes.hpp"
#include "backend/api/storages.hpp"
#include "backend/api/users.hpp"
#include "states.hpp"

#include <tg_stater/handler/type.hpp>
#include <tgbot/Api.h>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/Message.h>

namespace cookcookhnya::handlers {

// Reimports of states
using states::MainMenu;

using states::StorageList;

using states::RecipeView;
using states::StorageCreationEnterName;
using states::StorageDeletion;
using states::StorageView;

using states::MemberAddition;
using states::MemberDeletion;
using states::PackMemberView;
using states::StorageMemberView;

using states::StorageIngredientsList;
using states::StorageIngredientsSearch;

using states::StorageSelection;
using states::SuggestedRecipeList;

using states::ShoppingListCreation;
using states::ShoppingListView;
// Type aliases
using ApiClientRef = const api::ApiClient&;
using UserApiRef = const api::UsersApi&;
using StorageApiRef = const api::StoragesApi&;
using IngredientsApiRef = const api::IngredientsApi&;
using RecipesApiRef = const api::RecipesApi&;

using BotRef = const TgBot::Api&;
using SMRef = const states::StateManager&;
using MessageRef = const TgBot::Message&;
using CallbackQueryRef = const TgBot::CallbackQuery&;
using InlineQueryRef = const TgBot::InlineQuery&;

using NoState = tg_stater::HandlerTypes::NoState;
using AnyState = tg_stater::HandlerTypes::AnyState;

} // namespace cookcookhnya::handlers
