#include "backend/api/users.hpp"

#include "backend/models/user.hpp"

namespace cookcookhnya::api {

using namespace models::user;

void UsersApi::updateInfo(UserId userId, const UpdateUserInfoBody& body) const {
    jsonPutWithJsonAuthed<void>(userId, "/users", body);
}

} // namespace cookcookhnya::api
