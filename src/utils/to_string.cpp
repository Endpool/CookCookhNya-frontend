#include "to_string.hpp"

#include "uuid.hpp"

#include <boost/lexical_cast.hpp>

#include <string>

namespace cookcookhnya::utils {

std::string to_string(const Uuid& u) {
    return boost::lexical_cast<std::string>(u);
}

} // namespace cookcookhnya::utils
