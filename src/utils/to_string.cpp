#include "to_string.hpp"

#include "backend/models/publication_request_status.hpp"
#include "utils/utils.hpp"
#include "uuid.hpp"

#include <array>
#include <boost/lexical_cast.hpp>

#include <string>

namespace cookcookhnya::utils {

std::string to_string(const Uuid& u) {
    return boost::lexical_cast<std::string>(u);
}

std::string to_string(const cookcookhnya::api::models::status::PublicationRequestStatus status) {
    const std::array statusStr = {utf8str(u8"🟡 На рассмотрении"),
                                  utf8str(u8"🟢 Принят"),
                                  utf8str(u8"🔴 Отклонен"),
                                  utf8str(u8"⚪️ Вы еще не отправили запрос")};
    return statusStr[static_cast<std::size_t>(status)];
}

} // namespace cookcookhnya::utils
