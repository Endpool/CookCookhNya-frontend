#pragma once

#include <chrono>
#include <string>

namespace cookcookhnya::utils {

std::chrono::system_clock::time_point parseIsoTime(std::string s);

} // namespace cookcookhnya::utils
