#pragma once

#include "publication_request_status.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <optional>
#include <string>

namespace cookcookhnya::api::models::request_history {

struct PublicationHistoryInstance {
    std::string name;
    std::chrono::system_clock::time_point created;
    std::optional<std::string> reason;
    api::models::status::PublicationRequestStatus status{};
    std::optional<std::chrono::system_clock::time_point> updated;

    friend PublicationHistoryInstance tag_invoke(boost::json::value_to_tag<PublicationHistoryInstance>,
                                                 const boost::json::value& j);
};
} // namespace cookcookhnya::api::models::request_history
