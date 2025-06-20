#pragma once

// Add here all new namespaces and handlers declarations
#include "initial/initialStart.hpp"

#include "storageList/storageListCreate.hpp"


#include "storageList/storageListDelete.hpp"

#include "storageList/storageListView.hpp"

#include "storageView/storageAddDeleteMembers.hpp"

#include "storageView/storageView.hpp"

#include "storageView/storageViewMembers.hpp"


namespace cookcookhnya::handlers {
    using namespace cookcookhnya::handlers::storageListCreate;
    using namespace cookcookhnya::handlers::storageView;
    using namespace cookcookhnya::handlers::storageListView;
    using namespace cookcookhnya::handlers::storageListDelete;
    using namespace cookcookhnya::handlers::storageViewMembers;
    using namespace cookcookhnya::handlers::storageAddDeleteMembers;
    using namespace cookcookhnya::handlers::init;

    //Const for handle no state and start
    constexpr char startCmd[] = "start";

    //StorageListCreate
    using StorageCreateHandler = Handler<Events::Message{}, createStorage>;
    using StorageCreateButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageCreation>;

    //StorageListDelete
    using storgeDeleteHandler = Handler<Events::CallbackQuery{}, deleteStorage>;
    using StorageDeleteButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageDeletion>;

    //StorageListView
    using StorageListButtonHandler = Handler<Events::CallbackQuery{}, storageListButtonCallback>;   

    //StorageView
    using storageViewButtonHandler = Handler<Events::CallbackQuery{}, storageViewButtonCallback>;

    //StorageViewMembers
    using storageMemberViewButtonHandler = Handler<Events::CallbackQuery{}, storageMemberViewButtonCallback>;
    
    //storageAddDeleteMembers
    using memberAdditionDeletionMessageHandler = Handler<Events::Message{}, addDeleteMember>;
    using cancelAddDeleteMemberHandler = Handler<Events::CallbackQuery{}, cancelAddDeleteMember>;

    //Init
    using startHandler = Handler<Events::Command{startCmd}, start, AnyState{}>;
    using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, HandlerTypes::NoState{}>;
}