#pragma once

#include "backend/api/base.hpp"
#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"

#include <httplib.h>

#include <vector>

namespace cookcookhnya::api {

class StoragesApi : ApiBase {
    friend class ApiClient;

    explicit StoragesApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] std::vector<models::storage::StorageSummary> getStoragesList(UserId userId) const;
    [[nodiscard]] models::storage::StorageDetails get(UserId userId, StorageId id) const;
    StorageId create(UserId userId, // NOLINT(*-nodiscard)
                     const models::storage::StorageCreateBody& body) const;
    void delete_(UserId userId, StorageId id) const;
    [[nodiscard]] std::vector<models::storage::StorageMemberDetails> getStorageMembers(UserId userId,
                                                                                       StorageId id) const;

    void addMember(UserId userId, StorageId id, UserId memberId) const;
    void deleteMember(UserId userId, StorageId id, UserId memberId) const;
    [[nodiscard]] Hash inviteMember(UserId userId, StorageId storageId) const;
    void activate(UserId userId, Hash hash) const;
};

} // namespace cookcookhnya::api
