#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>

enum struct filterType : std::uint8_t { All, Custom, Public };

static std::string filterStr(filterType value) { // NOLINT(*unused*)
    static const std::unordered_map<filterType, std::string> map = {
        {filterType::All, "All"}, {filterType::Custom, "Custom"}, {filterType::Public, "Public"}};

    auto it = map.find(value);
    if (it != map.end()) {
        return it->second;
    }
    throw std::invalid_argument("Invalid filterValue value");
}
