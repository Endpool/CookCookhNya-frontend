#include "publicity_filter.hpp"

#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace cookcookhnya::utils {

std::string to_string(PublicityFilterType value) {
    static constexpr std::array names = {"All", "Custom", "Public"};

    if (static_cast<std::uint8_t>(value) >= names.size())
        throw std::invalid_argument("Invalid FilterType value");
    return names[static_cast<std::uint8_t>(value)];
}

} // namespace cookcookhnya::utils
