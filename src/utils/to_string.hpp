#pragma once

#include "backend/models/publication_request_status.hpp"

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

std::string to_string(cookcookhnya::api::models::status::PublicationRequestStatus status);

std::string to_string(std::chrono::system_clock::time_point tp);

} // namespace cookcookhnya::utils
