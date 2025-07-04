#include "backend/models/user.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

namespace cookcookhnya::api::models::user {

namespace json = boost::json;

void tag_invoke(json::value_from_tag /*tag*/, json::value& j, const UpdateUserInfoBody& body) {
    if (body.alias)
        j = {{"alias", *body.alias}, {"fullName", body.fullname}};
    else
        j = {{"fullName", body.fullname}};
}

} // namespace cookcookhnya::api::models::user
