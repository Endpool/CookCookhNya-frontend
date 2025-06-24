#include "backend/api/storages.hpp"

#include "backend/models/storage.hpp"

#include <format>
#include <vector>

namespace cookcookhnya::api {

using namespace models::storage;

std::vector<StorageSummary> StoragesApi::getStoragesList(UserId userId) const {
    return jsonGetAuthed<std::vector<StorageSummary>>(userId, "/my/storages");
}

StorageDetails StoragesApi::get(UserId userId, StorageId id) const {
    return jsonGetAuthed<StorageDetails>(userId, std::format("/my/storages/{}", id));
}

StorageId StoragesApi::create(UserId userId, const StorageCreateBody& body) const {
    return jsonPostWithJsonAuthed<StorageCreateResponse>(userId, "/my/storages", body).id;
}

void StoragesApi::delete_(UserId userId, StorageId id) const {
    jsonDeleteAuthed<void>(userId, std::format("/my/storages/{}", id));
}

std::vector<StoragesApi::UserId> StoragesApi::getStorageMembers(UserId userId, StorageId id) const {
    return jsonGetAuthed<std::vector<UserId>>(userId, std::format("/my/storages/{}/members", id));
}

void StoragesApi::addMember(UserId userId, StorageId id, UserId memberId) const {
    jsonPutAuthed<void>(userId, std::format("/my/storages/{}/members/{}", id, memberId));
}

void StoragesApi::deleteMember(UserId userId, StorageId id, UserId memberId) const {
    jsonDeleteAuthed<void>(userId, std::format("/my/storages/{}/members/{}", id, memberId));
}

} // namespace cookcookhnya::api
