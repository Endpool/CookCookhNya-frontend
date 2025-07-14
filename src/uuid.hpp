#pragma once

#include <boost/json/value.hpp>
#include <boost/json/value_to.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_hash.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace boost::uuids {

uuid tag_invoke(json::value_to_tag<uuid> /*tag*/, const json::value& j);

} // namespace boost::uuids

namespace cookcookhnya {

using Uuid = boost::uuids::uuid;

} // namespace cookcookhnya
