#include "backend/models/storage.hpp"

#include <boost/json/value_to.hpp>

namespace cookcookhnya::api::models::storage {

namespace json = boost::json;

StorageSummary tag_invoke(json::value_to_tag<StorageSummary> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(StorageSummary::id)>(j.at("id")),
        .name = value_to<decltype(StorageSummary::name)>(j.at("name")),
    };
}

} // namespace cookcookhnya::api::models::storage
