#include "serialization.hpp"
#include <chrono>
#include <string>

namespace cookcookhnya::utils {

std::chrono::system_clock::time_point parseIsoTime(std::string s) {
    std::istringstream ss{std::move(s)};
    std::chrono::system_clock::time_point tp;
    ss >> std::chrono::parse("%FT%T%z", tp);
    return ss.fail() ? throw std::runtime_error("Could not parse datetime") : tp;
}

} // namespace cookcookhnya::utils
