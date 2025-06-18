#include "backend/api/storages.hpp"

#include "backend/models/storage.hpp"
#include "types.hpp"

#include <format>
#include <vector>

namespace cookcookhnya::api {

using namespace models::storage;

StoragesApi::StoragesApi(httplib::Client& api) : ApiBase{api} {}

std::vector<StorageSummary> StoragesApi::getStoragesList(UserId userId) const {
    return jsonGetAuthed<std::vector<StorageSummary>>(userId, "/my/storages");
}

StorageDetails StoragesApi::get(UserId userId, StorageId id) const {
    return jsonGetAuthed<StorageDetails>(userId, std::format("/my/storages/{}/name", id));
}

StorageId StoragesApi::create(UserId userId, const StorageCreateBody& body) const {
    return jsonPostWithJsonAuthed<StorageId>(userId, "/my/storages", body);
}

void StoragesApi::delete_(UserId userId, StorageId id) const {
    jsonDeleteAuthed<void>(userId, std::format("/my/storages/{}", id));
}

std::vector<UserId> StoragesApi::getStorageMembers(UserId userId, StorageId id) const {
    return jsonGetAuthed<std::vector<UserId>>(userId, std::format("/my/storages/{}/members", id));
}

} // namespace cookcookhnya::api
