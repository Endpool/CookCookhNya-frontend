#include "backend/api/storages.hpp"

#include "backend/models/storage.hpp"

#include <format>
#include <vector>

namespace cookcookhnya::api {

using namespace models::storage;

StoragesApi::StoragesApi(httplib::Client& api) : ApiBase{api} {}

std::vector<StorageSummary> StoragesApi::getStoragesList(UserId userId) const {
    return getJsonAuthed<std::vector<StorageSummary>>(userId, "/my/storages");
}

StorageDetails StoragesApi::get(UserId userId, StorageId id) const {
    return getJsonAuthed<StorageDetails>(userId, std::format("/my/storages/{}", id));
}

StorageId StoragesApi::create(UserId userId, const StorageCreateBody& body) const {
    return postJsonAuthed<StorageId>(userId, "/my/storages/create", body);
}

} // namespace cookcookhnya::api
