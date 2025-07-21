#pragma once

#include <boost/json/conversion.hpp>

#include <cstdint>

namespace cookcookhnya::api::models::moderation {

enum class PublicationRequestStatus : std::uint8_t { PENDING, ACCEPTED, REJECTED, NO_REQUEST };

PublicationRequestStatus tag_invoke(boost::json::value_to_tag<PublicationRequestStatus>, const boost::json::value& j);

} // namespace cookcookhnya::api::models::moderation
