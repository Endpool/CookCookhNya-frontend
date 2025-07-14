#include "backend/api/users.hpp"

#include "backend/models/user.hpp"

namespace cookcookhnya::api {

using namespace models::user;

void UsersApi::updateInfo(UserId user, const UpdateUserInfoBody& body) const {
    jsonPutWithJsonAuthed<void>(user, "/users", body);
}

} // namespace cookcookhnya::api
