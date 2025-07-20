#include "to_string.hpp"

#include "backend/models/recipe.hpp"
#include "utils/utils.hpp"
#include "uuid.hpp"

#include <boost/lexical_cast.hpp>

#include <chrono>
#include <string>

namespace cookcookhnya::utils {

std::string to_string(const Uuid& u) {
    return boost::lexical_cast<std::string>(u);
}

std::string to_string(const cookcookhnya::api::models::recipe::PublicationRequestStatus status) {
    const std::vector<std::string> statusStr = {utf8str(u8"🟡 На рассмотрении"),
                                                utf8str(u8"🟢 Принят"),
                                                utf8str(u8"🔴 Отклонен"),
                                                utf8str(u8"⚪️ Вы еще не отправили запрос")};
    return statusStr[static_cast<int>(status)];
}

std::string to_string(std::chrono::system_clock::time_point tp) {
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::gmtime(&time);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");

    return oss.str();
}

} // namespace cookcookhnya::utils
