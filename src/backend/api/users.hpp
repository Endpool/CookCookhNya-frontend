#pragma once

#include "backend/api/base.hpp"
#include "backend/models/user.hpp"

#include <httplib.h>

namespace cookcookhnya::api {

class UsersApi : ApiBase {
    friend class ApiClient;

    explicit UsersApi(httplib::Client& api) : ApiBase{api} {}

  public:
    void updateInfo(UserId user, // NOLINT(*-nodiscard)
                    const models::user::UpdateUserInfoBody& body) const;
};

using UserApiRef = const api::UsersApi&;

} // namespace cookcookhnya::api
