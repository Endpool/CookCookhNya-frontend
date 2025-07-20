#include "publication_request_status.hpp"

#include "utils/utils.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <array>
#include <cstddef>
#include <string>

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

namespace cookcookhnya::utils {

std::string to_string(api::models::status::PublicationRequestStatus status) {
    static constexpr std::array statusStr = {
        u8"🟡 На рассмотрении", u8"🟢 Принят", u8"🔴 Отклонен", u8"⚪️ Вы еще не отправили запрос"};
    return utf8str(statusStr[static_cast<std::size_t>(status)]);
}

} // namespace cookcookhnya::utils
