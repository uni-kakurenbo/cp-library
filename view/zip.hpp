#pragma once


#include "internal/dev_env.hpp"
#include "internal/ranges.hpp"
#include "internal/type_traits.hpp"

#include "utility/functional.hpp"

#include <ranges>
#include <concepts>
#include <tuple>
#include <iterator>


namespace uni {


template<std::ranges::input_range... Views> requires(std::ranges::view<Views> && ...) && (sizeof...(Views) > 0)
struct zip_view : std::ranges::view_interface<zip_view<Views...>> {
  private:
    std::tuple<Views...> _views;

  public:
    template<bool> struct iterator;
    template<bool> struct sentinel;

    zip_view() = default;

    constexpr explicit zip_view(Views... views) noexcept(NO_EXCEPT) : _views(std::move(views)...) {}

    constexpr auto begin() noexcept(NO_EXCEPT) requires(!(internal::simple_view<Views> && ...))
    {
        return iterator<false>(tuple_transform(std::ranges::begin, this->_views));
    }

    constexpr auto begin() const noexcept(NO_EXCEPT) requires(std::ranges::range<const Views> && ...)
    {
        return iterator<true>(tuple_transform(std::ranges::begin, this->_views));
    }

    constexpr auto end() noexcept(NO_EXCEPT) requires(!(internal::simple_view<Views> && ...))
    {
        if constexpr(!internal::zip_is_common<Views...>)
            return sentinel<false>(tuple_transform(std::ranges::end, this->_views));
        else if constexpr((std::ranges::random_access_range<Views> && ...))
            return begin() + std::iter_difference_t<iterator<false>>(this->size());
        else
            return iterator<false>(tuple_transform(std::ranges::end, this->_views));
    }

    constexpr auto end() const noexcept(NO_EXCEPT) requires(std::ranges::range<const Views> && ...)
    {
        if constexpr(!internal::zip_is_common<const Views...>)
            return sentinel<true>(tuple_transform(std::ranges::end, this->_views));
        else if constexpr((std::ranges::random_access_range<const Views> && ...))
            return this->begin() + std::iter_difference_t<iterator<true>>(this->size());
        else
            return iterator<true>(tuple_transform(std::ranges::end, _views));
    }

    constexpr auto size() noexcept(NO_EXCEPT) requires(std::ranges::sized_range<Views> && ...)
    {
        return std::apply(
            [](auto... sizes)
            {
                using size_type = std::make_unsigned_t<std::common_type_t<decltype(sizes)...>>;
                return uni::min(size_type(sizes)...);
            },
           tuple_transform(std::ranges::size, _views)
        );
    }

    constexpr auto size() const noexcept(NO_EXCEPT) requires(std::ranges::sized_range<const Views> && ...)
    {
        return std::apply(
            [](auto... sizes)
            {
                using size_type = std::make_unsigned_t<std::common_type_t<decltype(sizes)...>>;
                return uni::min(size_type(sizes)...);
            },
           tuple_transform(std::ranges::size, _views)
        );
    }
};

template<class... Ranges> zip_view(Ranges &&...) -> zip_view<std::views::all_t<Ranges>...>;

namespace internal {


template<class iterator>
constexpr const typename iterator::iterator_collection& get_current(const iterator& itr) noexcept(NO_EXCEPT);


} // namespace internal


template<std::ranges::input_range... Views> requires(std::ranges::view<Views> && ...) && (sizeof...(Views) > 0)
template<bool Const>
struct zip_view<Views...>::iterator
    : internal::zip_view_iterator_category<Const, Views...> {
    using iterator_collection = internal::tuple_or_pair_t<
            std::ranges::iterator_t<internal::maybe_const_t<Const, Views>>...
        >;

  private:
    friend struct zip_view;
    template<bool> friend struct zip_view::sentinel;

    iterator_collection _current;

    constexpr explicit iterator(decltype(_current) current) : _current(std::move(current)) {}

    // template<std::copy_constructible F, std::ranges::input_range... Vs>
    //     requires
    //         (std::ranges::view<Vs> && ...) && (sizeof...(Vs) > 0) && std::is_object_v<F> &&
    //         std::regular_invocable<F&, std::ranges::range_reference_t<Vs>...> &&
    //         internal::can_reference<std::invoke_result_t<F&, std::ranges::range_reference_t<Vs>...>>
    // friend struct zip_transform_view;


  public:
    using iterator_concept = internal::most_primitive_iterator_concept<Const, Views...>;

    using value_type = internal::tuple_or_pair_t<std::ranges::range_value_t<internal::maybe_const_t<Const, Views>>...>;

    using difference_type = std::common_type_t<std::ranges::range_difference_t<internal::maybe_const_t<Const, Views>>...>;

    iterator() = default;

    constexpr iterator(iterator<!Const> itr) noexcept(NO_EXCEPT)
        requires Const &&
                (
                    std::convertible_to<
                        std::ranges::iterator_t<Views>,
                        std::ranges::iterator_t<internal::maybe_const_t<Const, Views>>
                    > && ...
                )
      : _current(std::move(itr._current))
    {}


    constexpr auto operator*() const noexcept(NO_EXCEPT) {
        const auto f = [](auto &itr) -> decltype(auto) { return *itr; };
        return tuple_transform(f, this->_current);
    }

    constexpr iterator& operator++() noexcept(NO_EXCEPT) {
        tuple_for_each([](auto &itr) { ++itr; }, this->_current);
        return *this;
    }

    constexpr void operator++(int) noexcept(NO_EXCEPT) { ++*this; }

    constexpr iterator operator++(int) noexcept(NO_EXCEPT)
        requires internal::all_forward<Const, Views...>
    {
        const auto res = *this; ++*this; return res;
    }

    constexpr iterator& operator--() noexcept(NO_EXCEPT)
        requires internal::all_bidirectional<Const, Views...>
    {
        tuple_for_each([](auto &itr) { --itr; }, this->_current);
        return *this;
    }

    constexpr iterator operator--(int) noexcept(NO_EXCEPT)
        requires internal::all_bidirectional<Const, Views...>
    {
        const auto res = *this; --*this; return res;
    }

    constexpr iterator& operator+=(const difference_type diff) noexcept(NO_EXCEPT)
        requires internal::all_random_access<Const, Views...>
    {
        const auto f = [&]<class Itr>(Itr& itr) constexpr noexcept(NO_EXCEPT) {
            itr += std::iter_difference_t<Itr>(diff);
        };
        tuple_for_each(f, this->_current);
        return *this;
    }

    constexpr iterator& operator-=(const difference_type diff) noexcept(NO_EXCEPT)
        requires internal::all_random_access<Const, Views...>
    {
        const auto f = [&]<class Itr>(Itr& itr) constexpr noexcept(NO_EXCEPT) {
            itr -= std::iter_difference_t<Itr>(diff);
        };
        tuple_for_each(f, this->_current);
        return *this;
    }

    constexpr auto operator[](const difference_type diff) const noexcept(NO_EXCEPT)
        requires internal::all_random_access<Const, Views...>
    {
        const auto f = [&]<class Itr>(Itr& itr) constexpr noexcept(NO_EXCEPT) -> decltype(auto) {
            return itr[std::iter_difference_t<Itr>(diff)];
        };
        return tuple_transform(f, _current);
    }

    friend constexpr bool operator==(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT)
        requires (
            std::equality_comparable<
                std::ranges::iterator_t<internal::maybe_const_t<Const, Views>>
            > && ...
        )
    {
        if constexpr(internal::all_bidirectional<Const, Views...>)
            return lhs._current == rhs._current;
        else
            return [&]<std::size_t... Is>(std::index_sequence<Is...>) constexpr noexcept(NO_EXCEPT) {
                return (
                    (std::get<Is>(lhs._current) == std::get<Is>(rhs._current)) || ...
                );
            }(std::make_index_sequence<sizeof...(Views)>{});
    }

    friend constexpr auto operator<=>(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT)
        requires internal::all_random_access<Const, Views...>
    {
        return lhs._current <=> rhs._current;
    }

    friend constexpr iterator operator+(const iterator& itr, const difference_type diff) noexcept(NO_EXCEPT)
        requires internal::all_random_access<Const, Views...>
    {
        auto res = itr; res += diff; return res;
    }

    friend constexpr iterator operator+(const difference_type diff, const iterator& itr) noexcept(NO_EXCEPT)
        requires internal::all_random_access<Const, Views...>
    {
        auto res = itr; res += diff; return res;
    }

    friend constexpr iterator operator-(const iterator& itr, const difference_type diff) noexcept(NO_EXCEPT)
        requires internal::all_random_access<Const, Views...>
    {
        auto res = itr; res -= diff; return res;
    }

    friend constexpr difference_type operator-(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT)
        requires (
            std::sized_sentinel_for<
                std::ranges::iterator_t<internal::maybe_const_t<Const, Views>>,
                std::ranges::iterator_t<internal::maybe_const_t<Const, Views>>
            > && ...
        )
    {
        return [&]<std::size_t... Is>(std::index_sequence<Is...>) constexpr noexcept(NO_EXCEPT) {
            return std::ranges::min(
                {
                    difference_type(std::get<Is>(lhs._current) - std::get<Is>(rhs._current)) ...
                },
                std::ranges::less{},
                [](const difference_type diff) constexpr noexcept(NO_EXCEPT) {
                    return to_unsigned(diff < 0 ? -diff : diff);
                }
            );
        }(std::make_index_sequence<sizeof...(Views)>{});
    }

    friend constexpr auto iter_move(const iterator& itr) noexcept(NO_EXCEPT) {
        return tuple_transform(std::ranges::iter_move, itr._current);
    }

    friend constexpr void iter_swap(const iterator& lhs, const iterator& res) noexcept(NO_EXCEPT)
        requires (
            std::indirectly_swappable<
                std::ranges::iterator_t<internal::maybe_const_t<Const, Views>>
            > && ...
        )
    {
        [&]<std::size_t... Is>(std::index_sequence<Is...>) constexpr noexcept(NO_EXCEPT) {
            (
                std::ranges::iter_swap(std::get<Is>(lhs._current), std::get<Is>(res._current)), ...
            );
        }(std::make_index_sequence<sizeof...(Views)>{});
    }

    template<class Itr> friend constexpr const typename Itr::iterator_collection& internal::get_current(const Itr&) noexcept(NO_EXCEPT);
};


template<class iterator>
constexpr const typename iterator::iterator_collection& internal::get_current(const iterator& itr) noexcept(NO_EXCEPT) { return itr._current; };


template<std::ranges::input_range... Views> requires(std::ranges::view<Views> && ...) && (sizeof...(Views) > 0)
template<bool Const>
struct zip_view<Views...>::sentinel {
    friend struct zip_view;
    template<bool> friend struct zip_view::iterator;

    using sentinel_collection = internal::tuple_or_pair_t<
            std::ranges::sentinel_t<internal::maybe_const_t<Const, Views>>...
        >;

    sentinel_collection _end;

    constexpr explicit sentinel(sentinel_collection end) noexcept(NO_EXCEPT) : _end(end) {}

  public:
    sentinel() = default;

    constexpr sentinel(sentinel<!Const> itr) noexcept(NO_EXCEPT)
        requires Const &&
                (
                    std::convertible_to<
                        std::ranges::sentinel_t<Views>,
                        std::ranges::sentinel_t<internal::maybe_const_t<Const, Views>>
                    > && ...
                )
      : _end(std::move(itr._end))
    {}

    template<bool Const_>
        requires (
            std::sentinel_for<
                std::ranges::sentinel_t<internal::maybe_const_t<Const, Views>>,
                std::ranges::iterator_t<internal::maybe_const_t<Const_, Views>>
            > && ...
        )
    friend constexpr bool operator==(const iterator<Const_>& lhs, const sentinel& rhs) noexcept(NO_EXCEPT)
    {
        return [&]<std::size_t... Is>(std::index_sequence<Is...>) constexpr noexcept(NO_EXCEPT) {
            return (
                (std::get<Is>(internal::get_current(lhs)) == std::get<Is>(rhs._end)) || ...);
        }(std::make_index_sequence<sizeof...(Views)>{});
    }

    template<bool Const_>
        requires (
            std::sized_sentinel_for<
                std::ranges::sentinel_t<internal::maybe_const_t<Const, Views>>,
                std::ranges::iterator_t<internal::maybe_const_t<Const_, Views>>
            > && ...
        )
    friend constexpr auto operator-(const iterator<Const_>& lhs, const sentinel& rhs) noexcept(NO_EXCEPT)
    {
        using return_type = std::common_type_t<std::ranges::range_difference_t<internal::maybe_const_t<Const_, Views>>...>;
        return [&]<std::size_t... Is>(std::index_sequence<Is...>) constexpr noexcept(NO_EXCEPT) {
            return std::ranges::min(
                { return_type(std::get<Is>(internal::get_current(lhs)) - std::get<Is>(rhs._end))... },
                std::ranges::less{},
                [](const return_type diff) {
                    return to_unsigned(diff < 0 ? -diff : diff);
                }
            );
        }(std::make_index_sequence<sizeof...(Views)>{});
    }

    template<bool Const_>
        requires (
            std::sized_sentinel_for<
                std::ranges::sentinel_t<internal::maybe_const_t<Const, Views>>,
                std::ranges::iterator_t<internal::maybe_const_t<Const_, Views>>
            > && ...
        )
    friend constexpr auto operator-(const sentinel &lhs, const iterator<Const_>& rhs) noexcept(NO_EXCEPT)
    {
        return -(rhs - lhs);
    }
};


namespace views {

namespace internal {


template<class... Ts>
concept can_zip_view = requires { zip_view<std::views::all_t<Ts>...>(std::declval<Ts>()...); };


} // namespace internal


struct Zip {
    template<class... Ts> requires(sizeof...(Ts) == 0 || internal::can_zip_view<Ts...>)
    constexpr auto operator() [[nodiscard]] (Ts&&... vs) const {
        if constexpr(sizeof...(Ts) == 0) return std::views::empty<std::tuple<>>;
        else return zip_view<std::views::all_t<Ts>...>(std::forward<Ts>(vs)...);
    }
};


inline constexpr Zip zip;


} // namespace views

} // namespace uni.


namespace std::ranges {


template<class... Views>
inline constexpr bool enable_borrowed_range<uni::zip_view<Views...>> = (enable_borrowed_range<Views> && ...);


}
