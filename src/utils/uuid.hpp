#pragma once

#include <boost/json/fwd.hpp>
#include <boost/json/value.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_hash.hpp> // NOLINT
#include <boost/uuid/uuid_io.hpp>   // NOLINT

#include <format>
#include <string>

namespace boost::uuids {

uuid tag_invoke(json::value_to_tag<uuid> /*tag*/, const json::value& j);

} // namespace boost::uuids

namespace cookcookhnya {

using Uuid = boost::uuids::uuid;

} // namespace cookcookhnya

template <>
struct std::formatter<cookcookhnya::Uuid> : formatter<string> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return formatter<string>::parse(ctx);
    }

    template <class FmtContext>
    FmtContext::iterator format(const cookcookhnya::Uuid& u, FmtContext& ctx) const {
        return formatter<string>::format(boost::lexical_cast<string>(u), ctx);
    }
};
