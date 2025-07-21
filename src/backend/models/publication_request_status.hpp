#pragma once

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <cstdint>
#include <string>

namespace cookcookhnya::api::models::moderation {

enum class PublicationRequestStatus : std::uint8_t { NO_REQUEST, PENDING, ACCEPTED, REJECTED };

PublicationRequestStatus tag_invoke(boost::json::value_to_tag<PublicationRequestStatus>, const boost::json::value& j);

} // namespace cookcookhnya::api::models::moderation

namespace cookcookhnya::utils {

std::string to_string(api::models::moderation::PublicationRequestStatus status);

} // namespace cookcookhnya::utils
