#include "parsing.hpp"

#include "uuid.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/lexical_cast/try_lexical_convert.hpp>

#include <chrono>
#include <optional>
#include <string>
#include <string_view>

namespace cookcookhnya::utils {

template <>
std::optional<Uuid> parseSafe<Uuid>(std::string_view s) {
    Uuid value; // NOLINT(*init)
    if (boost::conversion::try_lexical_convert(s, value))
        return value;
    return {};
}

template <>
Uuid parse<Uuid>(std::string_view s) noexcept(false) {
    return boost::lexical_cast<Uuid>(s);
}

std::chrono::system_clock::time_point parseIsoTime(std::string s) {
    std::chrono::system_clock::time_point tp;
    std::istringstream ss(std::move(s));
    ss >> std::chrono::parse("%FT%TZ", tp); // Parse as UTC
    if (ss.fail()) {
        throw std::runtime_error("Could not parse datetime");
    }
    return tp; // Still UTC, but debugger may show local time
}

} // namespace cookcookhnya::utils
