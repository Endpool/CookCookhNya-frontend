#pragma once

#include "backend/api/base.hpp"
#include "backend/models/storage.hpp"
#include "types.hpp"

#include <httplib.h>

#include <vector>

namespace cookcookhnya::api {

class StoragesApi : ApiBase {
    friend class ApiClient;

    explicit StoragesApi(httplib::Client& api);

  public:
    [[nodiscard]] std::vector<models::storage::StorageSummary> getStoragesList(UserId userId) const;
    [[nodiscard]] models::storage::StorageDetails get(UserId userId, models::storage::StorageId id) const;
    models::storage::StorageId create(UserId userId, // NOLINT(*-nodiscard)
                                      const models::storage::StorageCreateBody& body) const;
};

} // namespace cookcookhnya::api
