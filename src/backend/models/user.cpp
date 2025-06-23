#include "backend/models/user.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value_to.hpp>

namespace cookcookhnya::api::models::user {

namespace json = boost::json;

UserDetails tag_invoke(json::value_to_tag<UserDetails> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(UserDetails::id)>(j.at("id")),
        .name = value_to<decltype(UserDetails::name)>(j.at("name")),
    };
}

void tag_invoke(boost::json::value_from_tag /*tag*/, boost::json::value& j, const UserCreateBody& body) {
    j = {{"name", body.name}, {"id", body.id}};
}

void tag_invoke(boost::json::value_to_tag<UserCreateResponse> /*tag*/) {}

} // namespace cookcookhnya::api::models::user
