#include <utils.hpp>

#include <cstdlib>
#include <format>
#include <stdexcept>

namespace utils {

const char* getenvWithError(const char* key) noexcept(false) {
    const char* value = std::getenv(key);
    if (value == nullptr)
        throw std::runtime_error(std::format("Environmental variable {} is not set.", key));
    return value;
}

} // namespace utils
