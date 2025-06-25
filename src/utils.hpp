#pragma once

#include <charconv>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

namespace cookcookhnya::utils {

const char* getenvWithError(const char* key) noexcept(false);

template <typename T>
std::optional<T> parseSafe(std::string_view s) {
    T value;
    if (std::from_chars(s.data(), s.data() + s.size(), value).ec == std::errc{})
        return value;
    return std::nullopt;
}

std::string utf8str(std::u8string_view sv);

template <typename T>
std::shared_ptr<T> make_shared(T&& t) {
    return std::make_shared<std::remove_cv_t<T>>(std::forward<T>(t));
}

} // namespace cookcookhnya::utils
