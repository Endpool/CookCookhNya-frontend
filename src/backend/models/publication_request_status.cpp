#include "publication_request_status.hpp"

#include "utils/utils.hpp"

#include <array>
#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_from.hpp>
#include <boost/json/value_to.hpp>
#include <cstddef>
#include <string>

namespace cookcookhnya::api::models::moderation {

PublicationRequestStatus tag_invoke(boost::json::value_to_tag<PublicationRequestStatus> /*tag*/,
                                    const boost::json::value& j) {
    if (j == "pending")
        return PublicationRequestStatus::PENDING;
    if (j == "accepted")
        return PublicationRequestStatus::ACCEPTED;
    if (j == "rejected")
        return PublicationRequestStatus::REJECTED;
    return PublicationRequestStatus::NO_REQUEST;
};

PublicationRequestStatusStruct tag_invoke(boost::json::value_to_tag<PublicationRequestStatusStruct> /*unused*/,
                                          const boost::json::value& j) {
    if (j.is_object()) {
        return {.status = j.as_object().if_contains("type") ? value_to<PublicationRequestStatus>(j.at("type"))
                                                            : PublicationRequestStatus::NO_REQUEST,
                .reason = j.as_object().if_contains("reason")
                              ? value_to<decltype(PublicationRequestStatusStruct::reason)>(j.at("reason"))
                              : std::nullopt};
    }
    return {.status = PublicationRequestStatus::NO_REQUEST, .reason = std::nullopt};
}

} // namespace cookcookhnya::api::models::moderation

namespace cookcookhnya::utils {

std::string to_string(api::models::moderation::PublicationRequestStatus status) {
    static constexpr std::array statusStr = {
        u8"🟡 На рассмотрении", u8"🟢 Принят", u8"🔴 Отклонен", u8"⚪️ Вы еще не отправили запрос"};
    return utf8str(statusStr[static_cast<std::size_t>(status)]);
}

} // namespace cookcookhnya::utils
