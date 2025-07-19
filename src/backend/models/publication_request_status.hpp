#pragma once

#include <boost/json/conversion.hpp>

#include <cstdint>

namespace cookcookhnya::api::models::status {

enum class PublicationRequestStatus : std::uint8_t { Pending = 0, Accepted = 1, Rejected = 2, Idle = 3 };

PublicationRequestStatus tag_invoke(boost::json::value_to_tag<PublicationRequestStatus>, const boost::json::value& j);

} // namespace cookcookhnya::api::models::status
