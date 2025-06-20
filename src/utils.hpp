#pragma once

#include <charconv>
#include <cstdlib>
#include <cstring>
#include <format>
#include <memory>
#include <optional>
#include <stdexcept>
#include <system_error>
#include <utility>

namespace cookcookhnya::utils {

const char* getenvWithError(const char* key) noexcept(false);

template <typename T>
std::optional<T> parseSafe(const char* s) {
    T value;
    if (std::from_chars(s, s + std::strlen(s), value).ec == std::errc{})
        return value;
    return std::nullopt;
}

template <typename T>
std::shared_ptr<T> make_shared(T&& t) {
    return std::make_shared<std::remove_cv_t<T>>(std::forward<T>(t));
}

} // namespace cookcookhnya::utils
