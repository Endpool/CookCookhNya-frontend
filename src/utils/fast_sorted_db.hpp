#pragma once

#include "utils/utils.hpp"

#include <functional>
#include <map>
#include <ranges>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace cookcookhnya::utils {

template <typename T, auto IdProjection = &T::id, auto SortProjection = &T::name>
class FastSortedDb {
    using Id = std::remove_cvref_t<std::invoke_result_t<decltype(IdProjection), const T&>>;
    using SortKey = std::remove_cvref_t<std::invoke_result_t<decltype(SortProjection), const T&>>;

  public:
    using mapped_type = T;
    using Map = std::map<SortKey, T>;

  private:
    Map items;
    std::unordered_map<Id, typename Map::iterator> index;

  public:
    FastSortedDb() = default;

    template <range_of<T> R>
    FastSortedDb(R&& items) { // NOLINT(*explicit*)
        for (auto&& item : std::ranges::views::all(std::forward<R>(items)))
            put(std::forward<decltype(item)>(item));
    }

    void put(const T& item) {
        SortKey key = std::invoke(SortProjection, item);
        auto [it, inserted] = this->items.emplace(std::move(key), item);
        if (inserted)
            index.try_emplace(std::invoke(IdProjection, std::as_const(it->second)), std::move(it));
    }

    void put(T&& item) {
        SortKey key = std::invoke(SortProjection, std::as_const(item));
        auto [it, inserted] = this->items.emplace(std::move(key), std::move(item));
        if (inserted)
            index.try_emplace(std::invoke(IdProjection, std::as_const(it->second)), std::move(it));
    }

    void remove(const Id& id) {
        auto it = index.find(id);
        if (it == index.end())
            return;
        items.erase(it->second);
        index.erase(it);
    }

    // as optional non-owning reference
    [[nodiscard]] T* operator[](const Id& id) {
        auto it = index.find(id);
        if (it == index.end())
            return nullptr;
        return &it->second->second;
    }

    // as optional non-owning reference
    [[nodiscard]] const T* operator[](const Id& id) const {
        auto it = index.find(id);
        if (it == index.end())
            return nullptr;
        return &it->second->second;
    }

    [[nodiscard]] Map& getAll() {
        return items;
    }

    [[nodiscard]] const Map& getAll() const {
        return items;
    }

    [[nodiscard]] auto getValues() {
        return items | std::views::values;
    }

    [[nodiscard]] auto getValues() const {
        return items | std::views::values;
    }
};

} // namespace cookcookhnya::utils
