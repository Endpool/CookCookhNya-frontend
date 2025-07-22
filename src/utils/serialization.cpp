#include "serialization.hpp"
#include <chrono>
#include <string>

namespace cookcookhnya::utils {

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
