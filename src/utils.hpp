#pragma once

#include "uuid.hpp"

#include <charconv>
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace cookcookhnya::utils {

const char* getenvWithError(const char* key) noexcept(false);

// parseSafe
template <typename T>
std::optional<T> parseSafe(std::string_view s) {
    T value;
    if (std::from_chars(s.data(), s.data() + s.size(), value).ec == std::errc{})
        return value;
    return std::nullopt;
}

template <>
std::optional<Uuid> parseSafe<Uuid>(std::string_view s);

// to_string
template <typename T>
    requires requires(T t) {
        { std::to_string(t) };
    }
std::string to_string(const T& t) {
    return std::to_string(t);
}

std::string to_string(const Uuid& u);

// other
std::string utf8str(std::u8string_view sv);

template <typename T>
std::shared_ptr<T> make_shared(T&& t) {
    return std::make_shared<std::remove_cv_t<T>>(std::forward<T>(t));
}

template <typename T, auto IdProjection = &T::id, auto SortProjection = &T::name>
class FastSortedDb {
    using Id = std::remove_cvref_t<std::invoke_result_t<decltype(IdProjection), const T&>>;

    struct Comparator {
        bool operator()(const T& l, const T& r) const {
            return std::less<>{}(std::invoke(SortProjection, l), std::invoke(SortProjection, r));
        }
    };

  public:
    using Set = std::set<T, Comparator>;

  private:
    Set items;
    std::unordered_map<Id, typename Set::iterator> index;

  public:
    FastSortedDb() = default;

    FastSortedDb(Set items) : items{std::move(items)} { // NOLINT(*explicit*)
        for (auto it = this->items.begin(); it != this->items.end(); ++it)
            index.try_emplace(std::invoke(IdProjection, std::as_const(*it)), it);
    }

    void put(const T& item) {
        const auto [it, inserted] = items.insert(item);
        if (inserted)
            index.try_emplace(std::invoke(IdProjection, std::as_const(*it)), it);
    }

    void put(T&& item) {
        const auto [it, inserted] = items.insert(std::move(item));
        if (inserted)
            index.try_emplace(std::invoke(IdProjection, std::as_const(*it)), it);
    }

    void remove(const Id& id) {
        auto it = index.find(id);
        if (it == index.end())
            return;
        items.erase(it->second);
        index.erase(it);
    }

    const Set& getAll() const {
        return items;
    }
};

} // namespace cookcookhnya::utils
