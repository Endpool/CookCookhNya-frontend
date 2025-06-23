#pragma once

#include "backend/api/base.hpp"
#include "backend/models/user.hpp"
#include "types.hpp"

#include <httplib.h>


namespace cookcookhnya::api {

class UsersApi : ApiBase {
    friend class ApiClient;

    explicit UsersApi(httplib::Client& api) : ApiBase{api} {}

  public:
    void create(UserId userId, // NOLINT(*-nodiscard)
                     const models::user::UserCreateBody& body) const;
};

} // namespace cookcookhnya::api
