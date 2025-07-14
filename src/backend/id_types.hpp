#pragma once

#include "utils/uuid.hpp"

#include <string>

namespace cookcookhnya::api {

using StorageId = Uuid;
using IngredientId = Uuid;
using RecipeId = Uuid;
using InvitationId = std::string;

} // namespace cookcookhnya::api
