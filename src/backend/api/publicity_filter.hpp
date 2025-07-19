#pragma once

#include <cstdint>
#include <string>

enum struct PublicityFilterType : std::uint8_t { All, Custom, Public };

namespace cookcookhnya::utils {

std::string to_string(PublicityFilterType value);

} // namespace cookcookhnya::utils
