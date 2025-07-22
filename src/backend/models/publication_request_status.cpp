#include "publication_request_status.hpp"

#include "utils/utils.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <array>
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
}

} // namespace cookcookhnya::api::models::moderation

namespace cookcookhnya::utils {

std::string to_string(api::models::moderation::PublicationRequestStatus status) {
    static constexpr std::array statusStr = {
        u8"🟡 На рассмотрении", u8"🟢 Принят", u8"🔴 Отклонен", u8"⚪️ Вы еще не отправили запрос"};
    return utf8str(statusStr[static_cast<std::size_t>(status)]);
}

} // namespace cookcookhnya::utils
