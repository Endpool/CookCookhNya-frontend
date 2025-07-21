#include "to_string.hpp"

#include <chrono>
#include <ctime>
#include <string>
#include <utility>

namespace cookcookhnya::utils {

std::string to_string(std::chrono::system_clock::time_point tp) {
    const std::time_t time = std::chrono::system_clock::to_time_t(tp);
    const std::tm tm = *std::gmtime(&time);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    return std::move(oss).str();
}

} // namespace cookcookhnya::utils
