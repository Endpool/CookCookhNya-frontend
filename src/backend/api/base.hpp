#pragma once

#include "types.hpp"

#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>
#include <boost/json/value_from.hpp>
#include <boost/json/value_to.hpp>
#include <httplib.h>

#include <functional>
#include <string>
#include <type_traits>
#include <utility>

namespace cookcookhnya::api {

class ApiBase {
  protected:
    std::reference_wrapper<httplib::Client> api; // NOLINT(*-non-private-member-*)

    explicit ApiBase(httplib::Client& api);

    static void assertSuccess(const httplib::Result& result);

    template <typename JsonOut>
    [[nodiscard]] JsonOut jsonGet(const std::string& path, const httplib::Headers& headers = {}) const {
        httplib::Result response = api.get().Get(path, headers);
        assertSuccess(response);
        if constexpr (!std::is_void_v<JsonOut>)
            return boost::json::value_to<JsonOut>(boost::json::parse(response->body));
    }

    template <typename JsonOut>
    [[nodiscard]] JsonOut jsonGetAuthed(UserId userId, const std::string& path) const {
        return jsonGet<JsonOut>(path, {{"Authorization", "Bearer " + std::to_string(userId)}});
    }

    template <typename JsonOut, typename JsonIn>
    JsonOut jsonPostWithJson(const std::string& path, JsonIn&& body, const httplib::Headers& headers = {}) const {
        using namespace boost::json;
        std::string rawBody = serialize(value_from(std::forward<JsonIn>(body)));
        httplib::Result response = api.get().Post(path, headers, rawBody, "application/json");
        assertSuccess(response);
        if constexpr (!std::is_void_v<JsonOut>)
            return value_to<JsonOut>(parse(response->body));
    }

    template <typename JsonOut, typename JsonIn>
    JsonOut jsonPostWithJsonAuthed(UserId userId, const std::string& path, JsonIn&& body) const {
        return jsonPostWithJson<JsonOut>(
            path, std::forward<JsonIn>(body), {{"Authorization", "Bearer " + std::to_string(userId)}});
    }

    template <typename JsonOut, typename JsonIn>
    JsonOut jsonPutWithJson(const std::string& path, JsonIn&& body, const httplib::Headers& headers = {}) const {
        using namespace boost::json;
        std::string rawBody = serialize(value_from(std::forward<JsonIn>(body)));
        httplib::Result response = api.get().Put(path, headers, rawBody, "application/json");
        assertSuccess(response);
        if constexpr (!std::is_void_v<JsonOut>)
            return value_to<JsonOut>(parse(response->body));
    }

    template <typename JsonOut, typename JsonIn>
    JsonOut jsonPutWithJsonAuthed(UserId userId, const std::string& path, JsonIn&& body) const {
        return jsonPutWithJson<JsonOut>(
            path, std::forward<JsonIn>(body), {{"Authorization", "Bearer " + std::to_string(userId)}});
    }

    template <typename JsonOut>
    JsonOut jsonDelete(const std::string& path, const httplib::Headers& headers = {}) const {
        httplib::Result response = api.get().Delete(path, headers);
        assertSuccess(response);
        if constexpr (!std::is_void_v<JsonOut>)
            return boost::json::value_to<JsonOut>(boost::json::parse(response->body));
    }
    template <typename JsonOut>
    JsonOut jsonDeleteAuthed(UserId userId, const std::string& path) const {
        return jsonDelete<JsonOut>(path, {{"Authorization", "Bearer " + std::to_string(userId)}});
    }
};

} // namespace cookcookhnya::api
