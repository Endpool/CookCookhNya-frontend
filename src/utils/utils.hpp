#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace cookcookhnya::utils {

const char* getenvWithError(const char* key) noexcept(false);

std::string utf8str(std::u8string_view sv);

template <typename T>
std::shared_ptr<T> make_shared(T&& t) {
    return std::make_shared<std::remove_cv_t<T>>(std::forward<T>(t));
}

} // namespace cookcookhnya::utils
