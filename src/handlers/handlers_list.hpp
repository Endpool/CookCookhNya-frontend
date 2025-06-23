#pragma once

// Add here all new namespaces and handlers declarations
#include "initial/initial_start.hpp"
#include "storage_list/storage_list_create.hpp"
#include "storage_list/storage_list_delete.hpp"
#include "storage_list/storage_list_view.hpp"
#include "storage_view/storage_add_delete_members.hpp"
#include "storage_view/storage_view.hpp"
#include "storage_view/storage_view_members.hpp"

namespace cookcookhnya::handlers {
using namespace cookcookhnya::handlers::storageListCreate;
using namespace cookcookhnya::handlers::storageView;
using namespace cookcookhnya::handlers::storageListView;
using namespace cookcookhnya::handlers::storageListDelete;
using namespace cookcookhnya::handlers::storageViewMembers;
using namespace cookcookhnya::handlers::storageAddDeleteMembers;
using namespace cookcookhnya::handlers::init;

// Const for handle no state and start
constexpr char startCmd[] = "start"; // NOLINT(*c-arrays)

// StorageListCreate
using StorageCreateHandler = Handler<Events::Message{}, createStorage>;
using StorageCreateButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageCreation>;

// StorageListDelete
using storgeDeleteHandler = Handler<Events::CallbackQuery{}, deleteStorage>;
using StorageDeleteButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageDeletion>;

// StorageListView
using StorageListButtonHandler = Handler<Events::CallbackQuery{}, storageListButtonCallback>;

// StorageView
using storageViewButtonHandler = Handler<Events::CallbackQuery{}, storageViewButtonCallback>;

// StorageViewMembers
using storageMemberViewButtonHandler = Handler<Events::CallbackQuery{}, storageMemberViewButtonCallback>;

// storageAddDeleteMembers
using memberAdditionDeletionMessageHandler = Handler<Events::Message{}, addDeleteMember>;
using cancelAddDeleteMemberHandler = Handler<Events::CallbackQuery{}, cancelAddDeleteMember>;

// Init
using startHandler = Handler<Events::Command{startCmd}, start, AnyState{}>;
using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, HandlerTypes::NoState{}>;
} // namespace cookcookhnya::handlers

