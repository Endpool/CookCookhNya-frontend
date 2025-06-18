#pragma once

#include "types.hpp"

#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>
#include <boost/json/value_from.hpp>
#include <boost/json/value_to.hpp>
#include <httplib.h>

#include <functional>
#include <string>
#include <utility>

namespace cookcookhnya::api {

class ApiBase {
  protected:
    std::reference_wrapper<httplib::Client> api; // NOLINT(*-non-private-member-*)

    explicit ApiBase(httplib::Client& api);

    static void assertSuccess(const httplib::Result& result);

    template <typename JsonOut>
    [[nodiscard]] JsonOut getJson(const std::string& path) const {
        httplib::Result response = api.get().Get(path);
        assertSuccess(response);
        return boost::json::value_to<JsonOut>(boost::json::parse(response->body));
    }

    template <typename JsonOut>
    [[nodiscard]] JsonOut getJsonAuthed(UserId userId, const std::string& path) const {
        httplib::Result response = api.get().Get(path, {{"Authorization", std::to_string(userId)}});
        assertSuccess(response);
        return boost::json::value_to<JsonOut>(boost::json::parse(response->body));
    }

    template <typename JsonOut, typename JsonIn>
    JsonOut postJson(const std::string& path, JsonIn&& body) const {
        using namespace boost::json;
        std::string rawBody = serialize(value_from(std::forward<JsonIn>(body)));
        httplib::Result response = api.get().Post(path, rawBody, "application/json");
        assertSuccess(response);
        return value_to<JsonOut>(parse(response->body));
    }

    template <typename JsonOut, typename JsonIn>
    JsonOut postJsonAuthed(UserId userId, const std::string& path, JsonIn&& body) const {
        using namespace boost::json;
        std::string rawBody = serialize(value_from(std::forward<JsonIn>(body)));
        httplib::Result response =
            api.get().Post(path, {{"Authorization", std::to_string(userId)}}, rawBody, "application/json");
        assertSuccess(response);
        return value_to<JsonOut>(parse(response->body));
    }
};

} // namespace cookcookhnya::api
