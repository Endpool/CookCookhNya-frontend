#pragma once

#include <functional>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace cookcookhnya::utils {

template <typename T, auto IdProjection = &T::id, auto SortProjection = &T::name>
class FastSortedDb {
    using Id = std::remove_cvref_t<std::invoke_result_t<decltype(IdProjection), const T&>>;

    struct Comparator {
        bool operator()(const T& l, const T& r) const {
            return std::ranges::less{}(std::invoke(SortProjection, l), std::invoke(SortProjection, r));
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
