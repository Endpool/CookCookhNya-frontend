#pragma once

#include "publication_request_status.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <optional>
#include <string>

namespace cookcookhnya::api::models::moderation {

struct PublicationRequest {
    std::string name;
    std::string requestType;
    PublicationRequestStatusStruct status;
    std::chrono::system_clock::time_point created;
    std::optional<std::chrono::system_clock::time_point> updated;

    friend PublicationRequest tag_invoke(boost::json::value_to_tag<PublicationRequest>, const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::moderation
