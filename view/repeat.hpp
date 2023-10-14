#pragma once


#include <cassert>
#include <utility>
#include <type_traits>
#include <tuple>


#include "internal/dev_env.hpp"
#include "internal/iterator.hpp"

#include "numeric/arithmetic.hpp"
#include "view/internal/box.hpp"

#include <concepts>
#include <ranges>


namespace lib {


template <std::move_constructible T, std::semiregular Bound = std::unreachable_sentinel_t>
    requires
        std::is_object_v<T> && std::same_as<T, std::remove_cv_t<T>> &&
        (
            std::is_integral_v<Bound> ||
            std::same_as<Bound, std::unreachable_sentinel_t>
        )
struct repeat_view : public std::ranges::view_interface<repeat_view<T, Bound>> {
  private:
    internal::box<T> _value;
    [[no_unique_address]] Bound _bound = Bound();

    struct Iterator;

    template <typename Range>
    friend constexpr auto take_of_repeat_view(Range&&, std::ranges::range_difference_t<Range>);

    template <typename Range>
    friend constexpr auto drop_of_repeat_view(Range &&, std::ranges::range_difference_t<Range>);

  public:
    repeat_view() requires std::default_initializable<T> = default;

    constexpr explicit repeat_view(const T &val, Bound bound = Bound())
        requires std::copy_constructible<T> : _value(val), _bound(bound) {
        if constexpr(!std::same_as<Bound, std::unreachable_sentinel_t>) assert(bound >= 0);
    }

    constexpr explicit repeat_view(T &&val, Bound bound = Bound())
      : _value(std::move(val)), _bound(bound)
    {}

    template <typename... Args, typename... BoundArgs>
        requires std::constructible_from<T, Args...> && std::constructible_from<Bound, BoundArgs...>
    constexpr explicit repeat_view(
        std::piecewise_construct_t, std::tuple<Args...> args,
        std::tuple<BoundArgs...> bound_args = std::tuple<>{}
    )
      : _value(std::make_from_tuple<T>(std::move(args))),
        _bound(std::make_from_tuple<Bound>(std::move(bound_args)))
    {}

    constexpr Iterator begin() const {
        return Iterator(std::addressof(*this->_value));
    }

    constexpr Iterator end() const
        requires(!std::same_as<Bound, std::unreachable_sentinel_t>)
    {
        return Iterator(std::addressof(*this->_value), this->_bound);
    }

    constexpr std::unreachable_sentinel_t end() const noexcept {
        return std::unreachable_sentinel;
    }

    constexpr auto size() const
        requires(!std::same_as<Bound, std::unreachable_sentinel_t>)
    {
        return to_unsigned(this->_bound);
    }
};

template <typename T, typename Bound>
repeat_view(T, Bound) -> repeat_view<T, Bound>;

template <std::move_constructible T, std::semiregular Bound>
    requires
        std::is_object_v<T> && std::same_as<T, std::remove_cv_t<T>> &&
        (std::is_integral_v<Bound> || std::same_as<Bound, std::unreachable_sentinel_t>)
struct repeat_view<T, Bound>::Iterator {
  private:
    using index_type = std::conditional_t<std::same_as<Bound, std::unreachable_sentinel_t>, std::ptrdiff_t, Bound>;

    const T* _value = nullptr;
    index_type _crrent = index_type();

    constexpr explicit Iterator(const T* val, index_type bound = index_type())
      : _value(val), _crrent(bound)
    {
        if constexpr(!std::same_as<Bound, std::unreachable_sentinel_t>) assert(bound >= 0);
    }

    friend repeat_view;

  public:
    using iterator_concept = std::random_access_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type =
        std::conditional_t<
            std::is_integral_v<index_type>,
            index_type,
            internal::iota_diff_t<index_type>
        >;

    Iterator() = default;

    constexpr const T& operator*() const noexcept { return *this->_value; }

    constexpr Iterator& operator++() {
        ++this->_crrent;
        return *this;
    }

    constexpr Iterator operator++(int) {
        auto _tmp = *this;
        ++*this;
        return _tmp;
    }

    constexpr Iterator& operator--() {
        if constexpr(!std::same_as<Bound, std::unreachable_sentinel_t>) assert(this->_crrent > 0);
        --this->_crrent;
        return *this;
    }

    constexpr Iterator operator--(int) {
        auto _tmp = *this;
        --*this;
        return _tmp;
    }

    constexpr Iterator& operator+=(difference_type diff) {
        if constexpr(!std::same_as<Bound, std::unreachable_sentinel_t>) assert(this->_crrent + diff >= 0);
        this->_crrent += diff;
        return *this;
    }

    constexpr Iterator& operator-=(difference_type diff) {
        if constexpr(!std::same_as<Bound, std::unreachable_sentinel_t>) assert(this->_crrent - diff >= 0);
        this->_crrent -= diff;
        return *this;
    }

    constexpr const T& operator[](difference_type diff) const noexcept {
        return *(*this + diff);
    }

    friend constexpr bool operator==(const Iterator& lhs, const Iterator& rhs) {
        return lhs._crrent == rhs._crrent;
    }

    friend constexpr auto operator<=>(const Iterator& lhs, const Iterator& rhs) {
        return lhs._crrent <=> rhs._crrent;
    }

    friend constexpr Iterator operator+(Iterator itr, difference_type diff) {
        return itr += diff;
    }

    friend constexpr Iterator operator+(difference_type diff, Iterator itr) {
        return itr + diff;
    }

    friend constexpr Iterator operator-(Iterator itr, difference_type diff) {
        return itr -= diff;
    }

    friend constexpr difference_type operator-(const Iterator& lhs, const Iterator& rhs) {
        return (
            static_cast<difference_type>(lhs._crrent) -
            static_cast<difference_type>(rhs._crrent)
        );
    }
};

namespace views {

namespace internal {


template<typename _Range>
inline constexpr bool is_repeat_view = false;

template <class T, class Bound>
inline constexpr bool is_repeat_view<repeat_view<T, Bound>> = true;

template <class T>
concept can_repeat_view = requires { repeat_view(std::declval<T>()); };

template <class T, class Bound>
concept can_bounded_repeat_view =
    requires { repeat_view(std::declval<T>(), std::declval<Bound>()); };
} // namespace internal


struct Repeat {
    template <class T>
        requires internal::can_repeat_view<T>
    constexpr auto operator() [[nodiscard]] (T&& val) const {
        return repeat_view(std::forward<T>(val));
    }

    template <class T, class Bound>
        requires internal::can_bounded_repeat_view<T, Bound>
    constexpr auto operator()
        [[nodiscard]] (T&& val, Bound bound) const {
        return repeat_view(std::forward<T>(val), bound);
    }
};


inline constexpr Repeat repeat;


namespace internal {


template<class Range>
constexpr auto take_of_repeat_view(Range&& range, std::ranges::range_difference_t<Range> diff) {
    using T = std::remove_cvref_t<Range>;
    static_assert(is_repeat_view<T>);

    if constexpr(std::ranges::sized_range<T>) return views::repeat(*range._value, std::min(std::ranges::distance(range), diff));
    else return views::repeat(*range._value, diff);
}

template<class Range>
constexpr auto drop_of_repeat_view(Range&& range, std::ranges::range_difference_t<Range> diff) {
    using T = std::remove_cvref_t<Range>;
    static_assert(is_repeat_view<T>);

    if constexpr(std::ranges::sized_range<T>) {
        auto size = std::ranges::distance(range);
        return views::repeat(*range._value, size - std::min(size, diff));
    }
    else {
        return range;
    }
}


} // namespace internal

} // namespace views

} // namespace lib
