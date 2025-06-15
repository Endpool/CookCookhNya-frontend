#pragma once

#include <memory>
#include <utility>

namespace utils {

const char* getenvWithError(const char* key);


template <typename T>
std::shared_ptr<T> make_shared(T&& t) {
    return std::make_shared<std::remove_cv_t<T>>(std::forward<T>(t));
}

} // namespace utils