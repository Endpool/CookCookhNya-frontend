#include "backend/api/storages.hpp"

#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "utils/parsing.hpp"

#include <format>
#include <vector>

namespace cookcookhnya::api {

using namespace models::storage;

std::vector<StorageSummary> StoragesApi::getStoragesList(UserId user) const {
    return jsonGetAuthed<std::vector<StorageSummary>>(user, "/storages");
}

StorageDetails StoragesApi::get(UserId user, StorageId storage) const {
    return jsonGetAuthed<StorageDetails>(user, std::format("/storages/{}", storage));
}

StorageId StoragesApi::create(UserId user, const StorageCreateBody& body) const {
    return utils::parse<StorageId>(postWithJsonAuthed(user, "/storages", body));
}

void StoragesApi::delete_(UserId user, StorageId storage) const {
    jsonDeleteAuthed<void>(user, std::format("/storages/{}", storage));
}

std::vector<StorageMemberDetails> StoragesApi::getStorageMembers(UserId user, StorageId storage) const {
    return jsonGetAuthed<std::vector<StorageMemberDetails>>(user, std::format("/storages/{}/members", storage));
}

void StoragesApi::addMember(UserId user, StorageId storage, UserId member) const {
    jsonPutAuthed<void>(user, std::format("/storages/{}/members/{}", storage, member));
}

void StoragesApi::deleteMember(UserId user, StorageId storage, UserId member) const {
    jsonDeleteAuthed<void>(user, std::format("/storages/{}/members/{}", storage, member));
}

InvitationId StoragesApi::inviteMember(UserId user, StorageId storage) const {
    return postAuthed(user, std::format("/invitations/to/{}", storage));
}

void StoragesApi::activate(UserId user, InvitationId invitation) const {
    postAuthed(user, std::format("/invitations/{}/activate", invitation));
}

} // namespace cookcookhnya::api
