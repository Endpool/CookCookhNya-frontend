#pragma once

#include "tg_types.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <optional>
#include <string>

namespace cookcookhnya::api::models::user {

struct UpdateUserInfoBody {
    std::optional<std::string> alias;
    std::string fullName;

    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, const UpdateUserInfoBody& body);
};

struct UserDetails {
    tg_types::UserId userId;
    std::optional<std::string> alias;
    std::string fullName;

    friend UserDetails tag_invoke(boost::json::value_to_tag<UserDetails>, const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::user
