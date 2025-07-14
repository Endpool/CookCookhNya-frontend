#pragma once

#include "uuid.hpp"

#include <string>

namespace cookcookhnya::utils {

template <typename T>
    requires requires(T t) {
        { std::to_string(t) };
    }
std::string to_string(const T& t) {
    return std::to_string(t);
}

std::string to_string(const Uuid& u);

} // namespace cookcookhnya::utils
