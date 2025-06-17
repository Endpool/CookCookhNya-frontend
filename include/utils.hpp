#pragma once

#include <cstring>
#include <memory>
#include <cstdlib>
#include <format>
#include <stdexcept>
#include <utility>

namespace utils {

const char* getenvWithError(const char* key) noexcept(false) ;

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
  
} // namespace utils