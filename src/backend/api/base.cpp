#include "backend/api/base.hpp"

#include <httplib.h>

#include <format>
#include <stdexcept>

namespace cookcookhnya::api {

ApiBase::ApiBase(httplib::Client& api) : api{api} {}

void ApiBase::assertSuccess(const httplib::Result& result) noexcept(false) {
    constexpr static int STATUS_GROUP = 100;
    if (result.error() != httplib::Error::Success)
        throw std::runtime_error{std::format("API request network error (code {})", static_cast<int>(result.error()))};
    if (result->status / STATUS_GROUP != 2)
        throw std::runtime_error{std::format("API request HTTP error {}:\n{}\n", result->status, result->body)};
}

std::string ApiBase::post(const std::string& path, const httplib::Headers& headers) const { // NOLINT(*nodiscard)
    httplib::Result response = api.get().Post(path, headers);
    assertSuccess(response);
    return response->body;
}

} // namespace cookcookhnya::api
