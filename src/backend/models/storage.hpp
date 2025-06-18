#pragma once

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <string>

namespace cookcookhnya::api::models::storage {

using StorageId = long;

struct StorageSummary {
    StorageId id;
    std::string name;
};

StorageSummary tag_invoke(boost::json::value_to_tag<StorageSummary>, const boost::json::value& j);

} // namespace cookcookhnya::api::models::storage
