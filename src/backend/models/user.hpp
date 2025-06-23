#pragma once
#include "types.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <string>

namespace cookcookhnya::api::models::user {


struct UserDetails {
    UserId id;
    std::string name;

    friend UserDetails tag_invoke(boost::json::value_to_tag<UserDetails>, const boost::json::value& j);
};

struct UserCreateBody {
    std::string name;
    UserId id;

    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, const UserCreateBody& body);
    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, UserCreateBody&& body);
};

struct UserCreateResponse {
    friend void tag_invoke(boost::json::value_to_tag<UserCreateResponse>);
};

} // namespace cookcookhnya::api::models::user
