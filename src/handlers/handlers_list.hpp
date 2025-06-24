#pragma once

// Handler callbacks
#include "initial/initial_start.hpp"
#include "storage_list/storage_list_create.hpp"
#include "storage_list/storage_list_delete.hpp"
#include "storage_list/storage_list_view.hpp"
#include "storage_view/storage_add_delete_members.hpp"
#include "storage_view/storage_view.hpp"
#include "storage_view/storage_view_members.hpp"

#include "handlers/type_refs.hpp"

#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>

namespace cookcookhnya::handlers {

using namespace cookcookhnya::handlers::init;
using namespace cookcookhnya::handlers::storage_create;
using namespace cookcookhnya::handlers::storage_delete;
using namespace cookcookhnya::handlers::storage_list_view;
using namespace cookcookhnya::handlers::storage_view;
using namespace cookcookhnya::handlers::storage_view_members;
using namespace cookcookhnya::handlers::storage_add_delete_members;

using namespace tg_stater;

namespace bot_handlers {

// Init
constexpr char startCmd[] = "start";                                        // NOLINT(*c-arrays)
using startHandler = Handler<Events::Command{startCmd}, start, AnyState{}>; // NOLINT(*decay)
using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, NoState{}>;

// StorageListCreate
using storageCreateHandler = Handler<Events::Message{}, createStorage>;
using storageCreateButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageCreation>;

// StorageListDelete
using storgeDeleteHandler = Handler<Events::CallbackQuery{}, deleteStorage>;
using storageDeleteButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageDeletion>;

// StorageListView
using storageListButtonHandler = Handler<Events::CallbackQuery{}, storageListButtonCallback>;

// StorageView
using storageViewButtonHandler = Handler<Events::CallbackQuery{}, storageViewButtonCallback>;

// StorageViewMembers
using storageMemberViewButtonHandler = Handler<Events::CallbackQuery{}, storageMemberViewButtonCallback>;

// storageAddDeleteMembers
using memberAdditionDeletionMessageHandler = Handler<Events::Message{}, addDeleteMember>;
using cancelAddDeleteMemberHandler = Handler<Events::CallbackQuery{}, cancelAddDeleteMember>;

} // namespace bot_handlers

} // namespace cookcookhnya::handlers
