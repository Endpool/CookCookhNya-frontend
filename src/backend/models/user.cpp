#include "backend/models/user.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_to.hpp>
#include <optional>

namespace cookcookhnya::api::models::user {

namespace json = boost::json;

void tag_invoke(json::value_from_tag /*tag*/, json::value& j, const UpdateUserInfoBody& body) {
    if (body.alias)
        j = {{"alias", *body.alias}, {"fullName", body.fullName}};
    else
        j = {{"fullName", body.fullName}};
}

UserDetails tag_invoke(json::value_to_tag<UserDetails> /*tag*/, const json::value& j) {
    return {
        .userId = value_to<decltype(UserDetails::userId)>(j.at("id")),
        .alias =
            j.as_object().if_contains("alias") ? value_to<decltype(UserDetails::alias)>(j.at("alias")) : std::nullopt,
        .fullName = value_to<decltype(UserDetails::fullName)>(j.at("fullName")),
    };
}
} // namespace cookcookhnya::api::models::user
