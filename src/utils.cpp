#include "utils.hpp"

#include <cstdlib>
#include <format>
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

} // namespace cookcookhnya::utils
