#include "moderation.hpp"

#include "backend/models/moderation.hpp"

#include <httplib.h>

#include <cstddef>
#include <vector>

namespace cookcookhnya::api {

using namespace models::moderation;

// GET /publication-requests?size={}&offset={}
[[nodiscard]] std::vector<PublicationRequest>
ModerationApi::getAllPublicationRequests(UserId user, std::size_t size, std::size_t offset) const {
    return jsonGetAuthed<std::vector<PublicationRequest>>(user,
                                                          "/publication-requests",
                                                          {
                                                              {"size", utils::to_string(size)},
                                                              {"offset", utils::to_string(offset)},
                                                          });
}

} // namespace cookcookhnya::api
