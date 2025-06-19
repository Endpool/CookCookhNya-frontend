#pragma once
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
            this->content = content; 
        }
        storage(int storageId){
            this->storageId = storageId;
        }
        std::vector<std::string> getContent(std::int64_t userId){
            return content;
        }
        // Function which simulates look for storage (we should known user and Id of the storage)
        // However may be implemented without userId - depends on what backend will need
        storage getStorage(std::int64_t userId, int storageId){  
            return storage(1);
        }
        int getId(std::int64_t userId){
            return storageId;
        }
        std::string* getName(){
            return &name;
        }
};
storage backendExStorage = storage(1, "asd", {"asd", "asdf"});
class storages {
    int groupId = 0;
    std::vector<storage> content;

    public:
        storages(int groupId){
            this->groupId = groupId;
        }
        storages(int groupId, std::vector<storage> content){
            this->groupId = groupId;
            this->content = content;
        }

        std::vector<storage> getStorages (){
            return content;
        }

        std::vector<storage> getUserStorages(std::int64_t userId){
            return content;
        }
        void createStorage(std::int64_t userId, std::string name){
            content.push_back(storage(userId, name, {}));
        }

        bool deleteStorage(std::int64_t userId, int id){
            for (int i=0;i < content.size();i++){
                if (content[i].getId(123)==id){
                    // Deletes the second through third elements (vec[1], vec[2])
                    content.erase(std::next(content.begin(), i), std::next(content.begin(), i+1));
                }
            }
            return true;
        }
    };

    
storages backendEx = storages(1, {backendExStorage});