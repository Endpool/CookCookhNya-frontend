#include "backend/models/publication_request_status.hpp"

namespace cookcookhnya::api::models::status {

PublicationRequestStatus tag_invoke(boost::json::value_to_tag<PublicationRequestStatus> /*tag*/,
                                    const boost::json::value& j) {
    if (j.at("status") == "Pending")
        return PublicationRequestStatus::Pending;
    if (j.at("status") == "Accepted")
        return PublicationRequestStatus::Accepted;
    if (j.at("status") == "Rejected")
        return PublicationRequestStatus::Rejected;
    return PublicationRequestStatus::Idle;
}

} // namespace cookcookhnya::api::models::status
