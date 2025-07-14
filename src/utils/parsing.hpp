#pragma once

#include "uuid.hpp"

#include <charconv>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <system_error>

namespace cookcookhnya::utils {

// parseSafe
template <typename T>
std::optional<T> parseSafe(std::string_view s) {
    T value;
    if (std::from_chars(s.data(), s.data() + s.size(), value).ec == std::errc{})
        return value;
    return std::nullopt;
}

template <>
std::optional<Uuid> parseSafe<Uuid>(std::string_view s);

// parse
template <typename T>
T parse(std::string_view s) noexcept(false) {
    T value;
    if (std::from_chars(s.data(), s.data() + s.size(), value).ec == std::errc{})
        return value;
    throw std::runtime_error("Parsing error");
}

template <>
Uuid parse<Uuid>(std::string_view s) noexcept(false);

} // namespace cookcookhnya::utils
