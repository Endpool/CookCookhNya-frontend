
#include "main.cpp"
#include "states.hpp"

namespace {




class HelloWorldProvider {
    mutable httplib::Client api;

  public:
    explicit HelloWorldProvider(const std::string& apiUrl) : api{apiUrl} {}

    [[nodiscard]] std::string getHelloWorld() const {
        auto res = api.Get("/hello");
        if (!res)
            throw std::runtime_error(std::format("Error getting request: {}", static_cast<int>(res.error())));
        if (res->status != httplib::StatusCode::OK_200)
            throw std::runtime_error(std::format("Error getting request: {}", res->body));
        return std::move(res->body);
    }

};

inline bool filterPublicMessage(MessageRef m, BotRef bot) {
    if (!m.from || m.chat->id != m.from->id) {
        bot.sendMessage(m.chat->id, "Setup stickers in private messages");
        return true;
    }
    return false;
}


void helloWorld(MessageRef m, BotRef bot, const HelloWorldProvider& hwp) {
    bot.sendMessage(m.chat->id, hwp.getHelloWorld());
}
using helloWorldHandler = Handler<Events::Message{}, helloWorld, HandlerTypes::AnyState{}>;

inline void handleNoState(MessageRef m, BotRef bot) {
    if (m.text.starts_with("/start"))
        return;
    bot.sendMessage(m.chat->id, "Use /start please");
};
using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, HandlerTypes::NoState{}>;

constexpr char startCmd[] = "start";
inline void start(MessageRef m, BotRef bot, SMRef stateManager) {
    stateManager.put(StorageList{});
    renderStorageView(m.from->id, m.chat->id, bot);
};
using startHandler = Handler<Events::Command{startCmd}, start, HandlerTypes::AnyState{}>;


inline void StorageViewButtonCallback (const Api& bot, CallbackQueryRef cq, SMRef stateManager, const HelloWorldProvider& hwp){ //BackendProvider bkn
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    if (cq.data == "StorageViewCreate"){
        stateManager.put(StorageCreationEnterName{}); // Go to function create storage, while cancel button is handled on cancel storage creation 
        renderStorageCreate(chatId, bot); // Bot here prints menu of storage creation 
    }
    
    if (cq.data == "StorageViewDelete"){
        stateManager.put(StorageDeletionEnterName{});
        renderStorageDelete(chatId, bot);
    }
}


inline void createStorage(StorageCreationEnterName&, MessageRef m, BotRef bot, SMRef stateManager, const HelloWorldProvider& hwp) { //BackendProvider bkn
    backendEx.createStorage(m.from->id, m.text);
    stateManager.put(StorageList{});
    renderStorageView(m.from->id, m.chat->id, bot);
};
using StorageCreateHandler = Handler<Events::Message{}, createStorage>;

inline void cancelStorageCreation(StorageCreationEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "StorageCreateCancel") { // Here compare with data in button which was pressed (data was put in renderStorageCreate)
        stateManager.put(StorageList{});
        renderStorageView(cq.from->id, cq.message->chat->id, bot);
    }
};
using StorageCreateButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageCreation>;



inline bool deleteStorage(StorageDeletionEnterName&, MessageRef m, BotRef bot, SMRef stateManager, const HelloWorldProvider& hwp) { //BackendProvider bkn
    if (backendEx.deleteStorage(m.from->id, m.text)){
        stateManager.put(StorageList{});
        renderStorageView(m.from->id, m.chat->id, bot);
        return true;
    } else {
        bot.sendMessage(m.chat->id,"You entered wrong name");
        return false;
    }
};
using storgeDeleteHandler = Handler<Events::Message{}, createStorage>;

inline void cancelStorageDeletion(StorageDeletionEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "cancel") {
        stateManager.put(StorageList{});
        renderStorageView(cq.from->id, cq.message->chat->id, bot);
    }
};
using StorageDeleteButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageDeletion>;


} // namespace


