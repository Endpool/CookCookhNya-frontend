#include "to_string.hpp"

#include "backend/models/publication_request_status.hpp"
#include "utils/utils.hpp"
#include "uuid.hpp"

#include <boost/lexical_cast.hpp>

#include <array>
#include <string>

namespace cookcookhnya::utils {

std::string to_string(const Uuid& u) {
    return boost::lexical_cast<std::string>(u);
}

std::string to_string(const api::models::status::PublicationRequestStatus status) {
    static constexpr std::array statusStr = {
        u8"🟡 На рассмотрении", u8"🟢 Принят", u8"🔴 Отклонен", u8"⚪️ Вы еще не отправили запрос"};
    return utf8str(statusStr[static_cast<std::size_t>(status)]);
}

} // namespace cookcookhnya::utils
