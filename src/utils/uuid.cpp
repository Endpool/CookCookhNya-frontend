#include "uuid.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>

namespace boost::uuids {

uuid tag_invoke(json::value_to_tag<uuid> /*tag*/, const json::value& j) {
    return lexical_cast<uuid>(j.as_string().data());
}

} // namespace boost::uuids
