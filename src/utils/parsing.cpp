#include "parsing.hpp"

#include "uuid.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/lexical_cast/try_lexical_convert.hpp>

#include <optional>
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

} // namespace cookcookhnya::utils
