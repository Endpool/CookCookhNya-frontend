#include <cstddef>
#include <format>
#include <string>
#include <utility>

namespace cookcookhnya::utils {

namespace detail {

template <typename T>
struct TransformUtf8 {
    using type = T;
    static T&& transform(T&& t) { // NOLINT(*not-moved)
        return std::forward<T>(t);
    }
};

template <std::size_t N>
struct TransformUtf8<const char8_t (&)[N]> { // NOLINT(*c-arrays)
    using type = std::string;
    static std::string transform(const char8_t (&literal)[N]) { // NOLINT(*not-moved,*c-arrays)
        return {literal, literal + N - 1};                      // NOLINT(*decay)
    }
};

template <typename T>
using TransformUtf8_t = TransformUtf8<T>::type;

} // namespace detail

template <typename... Args>
std::string u8format(std::format_string<detail::TransformUtf8_t<Args>...> format, Args&&... args) {
    return std::format(format, detail::TransformUtf8<Args>::transform(std::forward<Args>(args))...);
}

} // namespace cookcookhnya::utils
