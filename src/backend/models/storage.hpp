#pragma once

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <string>

namespace cookcookhnya::api::models::storage {

using StorageId = long;

struct StorageSummary {
    StorageId id;
    std::string name;

    friend StorageSummary tag_invoke(boost::json::value_to_tag<StorageSummary>, const boost::json::value& j);
};

struct StorageDetails {
    std::string name;

    friend StorageDetails tag_invoke(boost::json::value_to_tag<StorageDetails>, const boost::json::value& j);
};

struct StorageCreateBody {
    std::string name;

    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, const StorageCreateBody& body);
};

} // namespace cookcookhnya::api::models::storage
