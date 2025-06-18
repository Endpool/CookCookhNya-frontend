#include "backend/api/storages.hpp"

#include "backend/models/storage.hpp"

#include <boost/json.hpp>
#include <boost/json/parse.hpp>
#include <httplib.h>

#include <vector>

namespace cookcookhnya::api {

using namespace models::storage;
namespace json = boost::json;

StoragesApi::StoragesApi(httplib::Client& api) : ApiBase{api} {}

std::vector<StorageSummary> StoragesApi::getStoragesList() const {
    httplib::Result response = api.get().Get("/my/storages");
    assertSuccess(response);
    return json::value_to<std::vector<StorageSummary>>(json::parse(response->body));
}

} // namespace cookcookhnya::api
