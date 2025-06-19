#include "backend/models/storage.hpp"

#include <boost/json/value_to.hpp>

namespace cookcookhnya::api::models::storage {

namespace json = boost::json;

StorageSummary tag_invoke(json::value_to_tag<StorageSummary> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(StorageSummary::id)>(j.at("id")),
        .name = value_to<decltype(StorageSummary::name)>(j.at("name")),
        .ownerId = value_to<decltype(StorageSummary::ownerId)>(j.at("ownerId")),
    };
}

StorageDetails tag_invoke(json::value_to_tag<StorageDetails> /*tag*/, const json::value& j) {
    return {
        .name = value_to<decltype(StorageDetails::name)>(j),
    };
}

void tag_invoke(boost::json::value_from_tag /*tag*/, boost::json::value& j, const StorageCreateBody& body) {
    j = {{"name", body.name}};
}

} // namespace cookcookhnya::api::models::storage
