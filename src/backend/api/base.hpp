#pragma once

#include <httplib.h>

#include <functional>

namespace cookcookhnya::api {

class ApiBase {
  protected:
    std::reference_wrapper<httplib::Client> api; // NOLINT(*-non-private-member-*)

    explicit ApiBase(httplib::Client& api);

    static void assertSuccess(const httplib::Result& result);
};

} // namespace cookcookhnya::api
