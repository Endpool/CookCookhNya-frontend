#pragma once

#include "backend/models/request_history.hpp"
#include "base.hpp"

#include <httplib.h>

#include <cstddef>
#include <vector>

namespace cookcookhnya::api {

class RequestHistoryApi : ApiBase {
    friend class ApiClient;

    explicit RequestHistoryApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] std::vector<models::request_history::PublicationHistoryInstance>
    getAllRequestHistory(UserId user, std::size_t size = 10, std::size_t offset = 0) const; // NOLINT(*magic-number*)
};

} // namespace cookcookhnya::api
