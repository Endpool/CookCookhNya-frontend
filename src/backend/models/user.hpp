#pragma once

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <optional>
#include <string>

namespace cookcookhnya::api::models::user {

struct UserUpdateInfoBody {
    std::optional<std::string> alias;
    std::string fullname;

    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, const UserUpdateInfoBody& body);
};

} // namespace cookcookhnya::api::models::user
