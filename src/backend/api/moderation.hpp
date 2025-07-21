#pragma once

#include "backend/models/moderation.hpp"
#include "base.hpp"

#include <httplib.h>

#include <cstddef>
#include <vector>

namespace cookcookhnya::api {

class ModerationApi : ApiBase {
    friend class ApiClient;

    explicit ModerationApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] std::vector<models::moderation::PublicationRequest> getAllPublicationRequests(
        UserId user, std::size_t size = 30, std::size_t offset = 0) const; // NOLINT(*magic-number*)
};

using ModerationApiRef = const ModerationApi&;

} // namespace cookcookhnya::api
