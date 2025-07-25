#include "backend/api/base.hpp"

#include <httplib.h>

#include <format>
#include <stdexcept>

namespace cookcookhnya::api {

ApiBase::ApiBase(httplib::Client& api) : api{api} {}

void ApiBase::assertSuccess(const httplib::Result& result) noexcept(false) {
    constexpr static int STATUS_GROUP = 100;
    if (result.error() != httplib::Error::Success)
        throw std::runtime_error{std::format("API request network error ({})", httplib::to_string(result.error()))};
    if (result->status / STATUS_GROUP != 2)
        throw std::runtime_error{std::format("API request HTTP error {}:\n{}\n", result->status, result->body)};
}

} // namespace cookcookhnya::api
