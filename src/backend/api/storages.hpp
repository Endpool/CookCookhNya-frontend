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
    [[nodiscard]] std::vector<models::storage::StorageSummary> getStoragesList(UserId user) const;
    [[nodiscard]] models::storage::StorageDetails get(UserId user, StorageId storage) const;
    StorageId create(UserId user, // NOLINT(*-nodiscard)
                     const models::storage::StorageCreateBody& body) const;
    void delete_(UserId user, StorageId storage) const;
    [[nodiscard]] std::vector<models::storage::StorageMemberDetails> getStorageMembers(UserId user,
                                                                                       StorageId storage) const;
    void addMember(UserId user, StorageId storage, UserId member) const;
    void deleteMember(UserId user, StorageId storage, UserId member) const;
    [[nodiscard]] InvitationId inviteMember(UserId user, StorageId storage) const;
    void activate(UserId user, InvitationId invitation) const;
};

} // namespace cookcookhnya::api
