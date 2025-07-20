#include "backend/models/publication_request_status.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

namespace cookcookhnya::api::models::status {

PublicationRequestStatus tag_invoke(boost::json::value_to_tag<PublicationRequestStatus> /*tag*/,
                                    const boost::json::value& j) {
    if (j.at("status") == "Pending")
        return PublicationRequestStatus::PENDING;
    if (j.at("status") == "Accepted")
        return PublicationRequestStatus::ACCEPTED;
    if (j.at("status") == "Rejected")
        return PublicationRequestStatus::REJECTED;
    return PublicationRequestStatus::NO_REQUEST;
}

} // namespace cookcookhnya::api::models::status
