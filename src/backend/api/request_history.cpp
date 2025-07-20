#include "request_history.hpp"
#include "backend/models/request_history.hpp"

#include <httplib.h>

#include <cstddef>
#include <vector>
namespace cookcookhnya::api {

using namespace models::request_history;
// GET /publication-requests?size={}&offset={}
[[nodiscard]] std::vector<models::request_history::PublicationHistoryInstance>
RequestHistoryApi::getAllRequestHistory(UserId user, std::size_t size, std::size_t offset) const {
    return jsonGetAuthed<std::vector<PublicationHistoryInstance>>(user,
                                                                  "/publication-requests",
                                                                  {
                                                                      {"size", utils::to_string(size)},
                                                                      {"offset", utils::to_string(offset)},
                                                                  });
}
} // namespace cookcookhnya::api
