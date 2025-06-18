#include "backend/api/storages.hpp"

#include "backend/models/storage.hpp"

#include <boost/json.hpp>
#include <boost/json/parse.hpp>
#include <httplib.h>

#include <format>
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

StorageDetails StoragesApi::get(StorageId id) const {
    httplib::Result response = api.get().Get(std::format("/my/storages/{}", id));
    assertSuccess(response);
    return json::value_to<StorageDetails>(json::parse(response->body));
}

} // namespace cookcookhnya::api
