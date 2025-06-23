#include "backend/api/users.hpp"

#include "backend/models/user.hpp"
#include "types.hpp"

#include <format>
#include <vector>

namespace cookcookhnya::api {

using namespace models::user;

void UsersApi::create(UserId userId, const UserCreateBody& body) const {
    jsonPostWithJsonAuthed<void>(userId, "/my/users", body);
}

} // namespace cookcookhnya::api
