#include "request_history.hpp"
#include "backend/models/recipe.hpp"
#include "utils/serialization.hpp"
#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_to.hpp>

namespace cookcookhnya::api::models::request_history {

namespace json = boost::json;

PublicationHistoryInstance tag_invoke(json::value_to_tag<PublicationHistoryInstance> /*tag*/, const json::value& j) {
    return {
        .name = value_to<decltype(PublicationHistoryInstance::name)>(j.at("name")),
        .created = utils::parseIsoTime(value_to<std::string>(j.at("created"))),
        .reason = j.as_object().if_contains("reason")
                      ? value_to<decltype(PublicationHistoryInstance::reason)>(j.at("reason"))
                      : "",
        .status = j.as_object().if_contains("status") ? value_to<recipe::PublicationRequestStatus>(j.at("status"))
                                                      : recipe::PublicationRequestStatus::NO_REQUEST,
        .updated = j.as_object().if_contains("updated") ? utils::parseIsoTime(value_to<std::string>(j.at("updated")))
                                                        : std::chrono::time_point<std::chrono::system_clock>(),
    };
}

} // namespace cookcookhnya::api::models::request_history
