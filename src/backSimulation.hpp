
#include <format>
#include <memory>
#include <ranges>
#include <string>
#include <vector>
// Simulation of backend
class storage {
    int storageId = 0;
    std::vector<std::string> content = {""};
    std::string name;
    public:
        storage(int storageId, std::string name, std::vector<std::string> content){
            this->storageId = storageId;
            this->name = name; 
            std::copy(content.begin(), content.begin()+content.size(), this->content.begin());
        }
        std::vector<std::string> getContent(){
            return content;
        }
        int getId(){
            return storageId;
        }
        std::string getName(){
            return name;
        }
};

class storages {
    int groupId = 0;
    std::vector<storage> content = {storage(1, "", {""})};

    public:
        storages(int groupId){
            this->groupId = groupId;
        }
        storages(int groupId, std::vector<storage> content){
            this->groupId = groupId;
            std::copy(content.begin(), content.begin()+content.size(), this->content.begin());
        }

        std::vector<storage> getStorages (){
            return content;
        }
        std::vector<storage> getUserStorages(std::int64_t userId){
            return {storage(1,"as",{"asd","asdf"})};
        }
        void createStorage(std::int64_t userId, std::string name){

        }

        bool deleteStorage(std::int64_t userId, std::string name){
            return true;
        }
    };

    
storages backendEx = storages(1);