#pragma once


#include "backend/id_types.hpp"
#include "types.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <string>

namespace cookcookhnya::api::models::storage {

struct StorageSummary {
    StorageId id;
    std::string name;
    UserId ownerId;

    friend StorageSummary tag_invoke(boost::json::value_to_tag<StorageSummary>, const boost::json::value& j);
};

struct StorageDetails {
    std::string name;
    StorageId id;
    UserId ownerId;

    friend StorageDetails tag_invoke(boost::json::value_to_tag<StorageDetails>, const boost::json::value& j);
};

struct StorageCreateBody {
    std::string name;

    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, const StorageCreateBody& body);
};

} // namespace cookcookhnya::api::models::storage
