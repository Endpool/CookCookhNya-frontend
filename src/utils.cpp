#include "utils.hpp"
#include "uuid.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/lexical_cast/try_lexical_convert.hpp>

#include <cstdlib>
#include <format>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace cookcookhnya::utils {

const char* getenvWithError(const char* key) noexcept(false) {
    const char* value = std::getenv(key);
    if (value == nullptr)
        throw std::runtime_error(std::format("Environmental variable {} is not set.", key));
    return value;
}

std::string utf8str(std::u8string_view sv) {
    return {sv.begin(), sv.end()};
}

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

std::string to_string(const Uuid& u) {
    return boost::lexical_cast<std::string>(u);
}

} // namespace cookcookhnya::utils
