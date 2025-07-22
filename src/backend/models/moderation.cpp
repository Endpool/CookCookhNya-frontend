#include "moderation.hpp"

#include "publication_request_status.hpp"
#include "utils/serialization.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_to.hpp>

#include <optional>

namespace cookcookhnya::api::models::moderation {

namespace json = boost::json;

PublicationRequest tag_invoke(json::value_to_tag<PublicationRequest> /*tag*/, const json::value& j) {
    return {
        .name = value_to<decltype(PublicationRequest::name)>(j.at("name")),
        .requestType = value_to<decltype(PublicationRequest::requestType)>(j.at("requestType")),
        .status = j.as_object().if_contains("status") ? value_to<PublicationRequestStatus>(j.at("status"))
                                                      : PublicationRequestStatus::NO_REQUEST,
        .created = utils::parseIsoTime(value_to<std::string>(j.at("createdAt"))),
        .updated = j.as_object().if_contains("updatedAt")
                       ? std::optional{utils::parseIsoTime(value_to<std::string>(j.at("updatedAt")))}
                       : std::nullopt,
        .reason = j.as_object().if_contains("reason") ? value_to<decltype(PublicationRequest::reason)>(j.at("reason"))
                                                      : std::nullopt,
    };
}

} // namespace cookcookhnya::api::models::moderation
