#include "backend/api/storages.hpp"

#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "backend/models/user.hpp"
#include "utils/parsing.hpp"

#include <format>
#include <vector>

namespace cookcookhnya::api {

using namespace models::storage;
using namespace models::user;

// GET /storages
std::vector<StorageSummary> StoragesApi::getStoragesList(UserId user) const {
    return jsonGetAuthed<std::vector<StorageSummary>>(user, "/storages");
}

// GET /storages/{storageId}
StorageDetails StoragesApi::get(UserId user, StorageId storage) const {
    return jsonGetAuthed<StorageDetails>(user, std::format("/storages/{}", storage));
}

// POST /storages
StorageId StoragesApi::create(UserId user, const StorageCreateBody& body) const {
    return utils::parse<StorageId>(postWithJsonAuthed(user, "/storages", body));
}

// DELETE /storages/{storageId}
void StoragesApi::delete_(UserId user, StorageId storage) const {
    jsonDeleteAuthed<void>(user, std::format("/storages/{}", storage));
}

// GET /storages/{storageId}/members
std::vector<UserDetails> StoragesApi::getStorageMembers(UserId user, StorageId storage) const {
    return jsonGetAuthed<std::vector<UserDetails>>(user, std::format("/storages/{}/members", storage));
}

// PUT /storages/{storageId}/members/{memberId}
void StoragesApi::addMember(UserId user, StorageId storage, UserId member) const {
    jsonPutAuthed<void>(user, std::format("/storages/{}/members/{}", storage, member));
}

// DELETE /storages/{storageId}/members/{memberId}
void StoragesApi::deleteMember(UserId user, StorageId storage, UserId member) const {
    jsonDeleteAuthed<void>(user, std::format("/storages/{}/members/{}", storage, member));
}

// POST /invitations/to/{storageId}
InvitationId StoragesApi::inviteMember(UserId user, StorageId storage) const {
    return postAuthed(user, std::format("/invitations/to/{}", storage));
}

// POST /invitations/{invitationHash}/activate
void StoragesApi::activate(UserId user, InvitationId invitation) const {
    postAuthed(user, std::format("/invitations/{}/activate", invitation));
}

} // namespace cookcookhnya::api
