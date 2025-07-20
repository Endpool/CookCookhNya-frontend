#include "backend/models/publication_request_status.hpp"

namespace cookcookhnya::api::models::status {

PublicationRequestStatus tag_invoke(boost::json::value_to_tag<PublicationRequestStatus> /*tag*/,
                                    const boost::json::value& j) {
    if (j == "pending")
        return PublicationRequestStatus::PENDING;
    if (j == "accepted")
        return PublicationRequestStatus::ACCEPTED;
    if (j == "rejected")
        return PublicationRequestStatus::REJECTED;
    return PublicationRequestStatus::NO_REQUEST;
}

} // namespace cookcookhnya::api::models::status
