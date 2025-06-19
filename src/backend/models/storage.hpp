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

    friend StorageSummary tag_invoke(boost::json::value_to_tag<StorageSummary>, const boost::json::value& j);
};

struct StorageDetails {
    std::string name;
    UserId ownerId;

    friend StorageDetails tag_invoke(boost::json::value_to_tag<StorageDetails>, const boost::json::value& j);
};

struct StorageCreateBody {
    std::string name;

    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, const StorageCreateBody& body);
    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, StorageCreateBody&& body);
};

struct StorageCreateResponse {
    StorageId id;

    friend StorageCreateResponse tag_invoke(boost::json::value_to_tag<StorageCreateResponse>,
                                            const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::storage
