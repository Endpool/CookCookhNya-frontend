#include "backend/api/storages.hpp"

#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"

#include <format>
#include <vector>

namespace cookcookhnya::api {

using namespace models::storage;

std::vector<StorageSummary> StoragesApi::getStoragesList(UserId userId) const {
    return jsonGetAuthed<std::vector<StorageSummary>>(userId, "/my/storages");
}

StorageDetails StoragesApi::get(UserId userId, StorageId storageId) const {
    return jsonGetAuthed<StorageDetails>(userId, std::format("/my/storages/{}", storageId));
}

StorageId StoragesApi::create(UserId userId, const StorageCreateBody& body) const {
    return jsonPostWithJsonAuthed<StorageId>(userId, "/my/storages", body);
}

void StoragesApi::delete_(UserId userId, StorageId storageId) const {
    jsonDeleteAuthed<void>(userId, std::format("/my/storages/{}", storageId));
}

std::vector<StorageMemberDetails> StoragesApi::getStorageMembers(UserId userId, StorageId storageId) const {
    return jsonGetAuthed<std::vector<StorageMemberDetails>>(userId, std::format("/my/storages/{}/members", storageId));
}

void StoragesApi::addMember(UserId userId, StorageId storageId, UserId memberId) const {
    jsonPutAuthed<void>(userId, std::format("/my/storages/{}/members/{}", storageId, memberId));
}

void StoragesApi::deleteMember(UserId userId, StorageId storageId, UserId memberId) const {
    jsonDeleteAuthed<void>(userId, std::format("/my/storages/{}/members/{}", storageId, memberId));
}

InvitationId StoragesApi::inviteMember(UserId userId, StorageId storageId) const {
    return postAuthed(userId, std::format("/invitations/to/{}", storageId));
}

void StoragesApi::activate(UserId userId, InvitationId id) const {
    postAuthed(userId, std::format("/invitations/{}/activate", id));
}

} // namespace cookcookhnya::api
