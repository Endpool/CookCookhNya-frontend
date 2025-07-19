#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>

enum struct FilterType : std::uint8_t { All, Custom, Public };

inline std::string filterStr(FilterType value) {
    static const std::unordered_map<FilterType, std::string> map = {
        {FilterType::All, "All"}, {FilterType::Custom, "Custom"}, {FilterType::Public, "Public"}};

    auto it = map.find(value);
    if (it != map.end()) {
        return it->second;
    }
    throw std::invalid_argument("Invalid FilterType value");
}
