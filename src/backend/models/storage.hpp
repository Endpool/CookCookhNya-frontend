#pragma once

#include "backend/id_types.hpp"
#include "tg_types.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <optional>
#include <string>

namespace cookcookhnya::api::models::storage {

struct StorageSummary {
    StorageId id;
    std::string name;
    tg_types::UserId ownerId;

    friend StorageSummary tag_invoke(boost::json::value_to_tag<StorageSummary>, const boost::json::value& j);
};

struct StorageDetails {
    std::string name;
    tg_types::UserId ownerId;

    friend StorageDetails tag_invoke(boost::json::value_to_tag<StorageDetails>, const boost::json::value& j);
};

struct StorageCreateBody {
    std::string name;

    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, const StorageCreateBody& body);
};

struct StorageMemberDetails {
    tg_types::UserId userId;
    std::optional<std::string> alias;
    std::string fullName;

    friend StorageMemberDetails tag_invoke(boost::json::value_to_tag<StorageMemberDetails>,
                                           const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::storage
