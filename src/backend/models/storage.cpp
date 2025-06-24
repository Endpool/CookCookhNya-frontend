#include "backend/models/storage.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value_to.hpp>

namespace cookcookhnya::api::models::storage {

namespace json = boost::json;

StorageSummary tag_invoke(json::value_to_tag<StorageSummary> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(StorageSummary::id)>(j.at("id")),
        .name = value_to<decltype(StorageSummary::name)>(j.at("name")),
    };
}

StorageDetails tag_invoke(json::value_to_tag<StorageDetails> /*tag*/, const json::value& j) {
    return {
        .name = value_to<decltype(StorageDetails::name)>(j.at("name")),
        .ownerId = value_to<decltype(StorageDetails::ownerId)>(j.at("id")),
    };
}

void tag_invoke(json::value_from_tag /*tag*/, json::value& j, const StorageCreateBody& body) {
    j = {{"name", body.name}};
}

StorageCreateResponse tag_invoke(json::value_to_tag<StorageCreateResponse> /*tag*/, const json::value& j) {
    return {.id = value_to<decltype(StorageSummary::id)>(j.at("id"))};
}

} // namespace cookcookhnya::api::models::storage
