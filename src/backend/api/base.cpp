#include "backend/api/base.hpp"

#include <format>
#include <httplib.h>

#include <functional>
#include <stdexcept>

namespace cookcookhnya::api {

ApiBase::ApiBase(httplib::Client& api) : api{api} {}

void ApiBase::assertSuccess(const httplib::Result& result) noexcept(false) {
    constexpr static int STATUS_GROUP = 100;
    if (result.error() != httplib::Error::Success || result->status / STATUS_GROUP != 2)
        throw std::runtime_error{std::format("API request error (code {})", static_cast<int>(result.error()))};
}

} // namespace cookcookhnya::api
