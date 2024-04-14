#pragma once


#include <concepts>
#include <ranges>


#include "internal/type_traits.hpp"
#include "internal/ranges.hpp"


namespace lib {


namespace internal {


template<class Range>
concept range_with_movable_reference =
    std::ranges::input_range<Range> && std::move_constructible<std::ranges::range_reference_t<Range>> &&
    std::move_constructible<std::ranges::range_rvalue_reference_t<Range>>;


} // namespace internal


template<std::ranges::view View>
    requires internal::range_with_movable_reference<View>
struct enumerate_view : public std::ranges::view_interface<enumerate_view<View>> {
  private:
    View _base = View();

    template<bool Const> class iterator;
    template<bool Const> class sentinel;

  public:
    enumerate_view()
        requires std::default_initializable<View>
    = default;

    constexpr explicit enumerate_view(View __base) : _base(std::move(__base)) {}

    constexpr auto begin()
        requires(!internal::simple_view<View>)
    {
        return iterator<false>(std::ranges::begin(this->_base), 0);
    }

    constexpr auto begin() const
        requires internal::range_with_movable_reference<const View>
    {
        return iterator<true>(std::ranges::begin(this->_base), 0);
    }

    constexpr auto end()
        requires(!internal::simple_view<View>)
    {
        if constexpr(std::ranges::common_range<View> && std::ranges::sized_range<View>) {
            return iterator<false>(std::ranges::end(_base), std::ranges::distance(this->_base));
        }
        else {
            return sentinel<false>(std::ranges::end(_base));
        }
    }

    constexpr auto end() const
        requires internal::range_with_movable_reference<const View>
    {
        if constexpr(std::ranges::common_range<const View> && std::ranges::sized_range<const View>) {
            return iterator<true>(std::ranges::end(_base), std::ranges::distance(_base));
        }
        else {
            return sentinel<true>(std::ranges::end(_base));
        }
    }

    constexpr auto size()
        requires std::ranges::sized_range<View>
    {
        return std::ranges::size(_base);
    }

    constexpr auto size() const
        requires std::ranges::sized_range<const View>
    {
        return std::ranges::size(_base);
    }

    constexpr View base() const &
        requires std::copy_constructible<View>
    {
        return _base;
    }

    constexpr View base() && { return std::move(_base); }
};


template<class Range>
enumerate_view(Range&& ) -> enumerate_view<std::views::all_t<Range>>;


template<std::ranges::view View>
    requires internal::range_with_movable_reference<View>
template<bool Const>
class enumerate_view<View>::iterator {
    using Base = internal::maybe_const_t<Const, View>;
    friend enumerate_view;

  public:
    using iterator_category = std::iterator_traits<std::ranges::iterator_t<Base>>::iterator_category;

    using iterator_concept = internal::most_primitive_iterator_concept<Const, View>;
    using difference_type = std::ranges::range_difference_t<Base>;
    using value_type = std::tuple<difference_type, std::ranges::range_value_t<Base>>;

  private:
    using rangeeference_type = std::tuple<difference_type, std::ranges::range_reference_t<Base>>;

    std::ranges::iterator_t<Base> _current = std::ranges::iterator_t<Base>();
    difference_type _pos = 0;

    constexpr explicit iterator(std::ranges::iterator_t<Base> __current, const difference_type __pos)
      : _current(std::move(__current)), _pos(__pos)
    {}

  public:
    iterator()
        requires std::default_initializable<std::ranges::iterator_t<Base>>
    = default;

    constexpr iterator(iterator<!Const> itr)
        requires Const && std::convertible_to<std::ranges::iterator_t<View>, std::ranges::iterator_t<Base>>
      : _current(std::move(itr._current)), _pos(itr._pos)
    {}

    constexpr const std::ranges::iterator_t<Base> &base() const & noexcept {
        return this->_current;
    }

    constexpr std::ranges::iterator_t<Base> base() && { return std::move(this->_current); }

    constexpr difference_type index() const noexcept { return this->_pos; }

    constexpr auto operator*() const {
        return rangeeference_type(this->_pos, *this->_current);
    }

    constexpr iterator& operator++() {
        ++this->_current;
        ++this->_pos;
        return *this;
    }

    constexpr void operator++(int) { ++*this; }

    constexpr iterator operator++(int)
        requires std::ranges::forward_range<Base>
    {
        auto temp = *this;
        ++*this;
        return temp;
    }

    constexpr iterator& operator--()
        requires std::ranges::bidirectional_range<Base>
    {
        --this->_current;
        --this->_pos;
        return *this;
    }

    constexpr iterator operator--(int)
        requires std::ranges::bidirectional_range<Base>
    {
        auto temp = *this;
        --*this;
        return temp;
    }

    constexpr iterator& operator+=(const difference_type diff)
        requires std::ranges::random_access_range<Base>
    {
        this->_current += diff;
        this->_pos += diff;
        return *this;
    }

    constexpr iterator& operator-=(const difference_type diff)
        requires std::ranges::random_access_range<Base>
    {
        this->_current -= diff;
        this->_pos -= diff;
        return *this;
    }

    constexpr auto operator[](const difference_type diff) const
        requires std::ranges::random_access_range<Base>
    {
        return rangeeference_type(this->_pos + diff, this->_current[diff]);
    }

    friend constexpr bool operator==(const iterator& lhs, const iterator& rhs) noexcept {
        return lhs._pos == rhs._pos;
    }

    friend constexpr std::strong_ordering
    operator<=>(const iterator& lhs, const iterator& rhs) noexcept {
        return lhs._pos <=> rhs._pos;
    }

    friend constexpr iterator operator+(iterator lhs, const difference_type rhs)
        requires std::ranges::random_access_range<Base>
    {
        return (lhs += rhs);
    }

    friend constexpr iterator operator+(const difference_type lhs, const iterator& rhs)
        requires std::ranges::random_access_range<Base>
    {
        return rhs += lhs;
    }

    friend constexpr iterator operator-(iterator& lhs, const difference_type rhs)
        requires std::ranges::random_access_range<Base>
    {
        return lhs -= rhs;
    }

    friend constexpr difference_type operator-(const iterator& lhs, const iterator& rhs) {
        return lhs._pos - rhs._pos;
    }

    friend constexpr auto iter_move(const iterator& itr)
        noexcept(
            noexcept (std::ranges::iter_move(itr._current)) &&
            std::is_nothrow_move_constructible_v<std::ranges::range_rvalue_reference_t<Base>>
        )
    {
            return std::tuple<difference_type, std::ranges::range_rvalue_reference_t<Base>>(
                itr._pos, std::ranges::iter_move(itr._current)
            );
    }
};

template<std::ranges::view View>
    requires internal::range_with_movable_reference<View>
template<bool Const>
class enumerate_view<View>::sentinel {
    using Base = internal::maybe_const_t<Const, View>;

    std::ranges::sentinel_t<Base> _end = std::ranges::sentinel_t<Base>();

    constexpr explicit sentinel(std::ranges::sentinel_t<Base> end) : _end(std::move(end)) {}

    friend enumerate_view;

  public:
    sentinel() = default;

    constexpr sentinel(sentinel<!Const> other)
        requires Const && std::convertible_to<std::ranges::sentinel_t<View>, std::ranges::sentinel_t<Base>>
        : _end(std::move(other._end)) {}

    constexpr std::ranges::sentinel_t<Base> base() const { return this->_end; }

    template<bool Const_>
        requires
            std::sentinel_for<
                std::ranges::sentinel_t<Base>, std::ranges::iterator_t<internal::maybe_const_t<Const_, View>>
            >
    friend constexpr bool operator==(const iterator<Const_>& lhs, const sentinel& rhs) {
        return lhs._current == rhs._end;
    }

    template<bool Const_>
        requires
            std::sized_sentinel_for<
                std::ranges::sentinel_t<Base>, std::ranges::iterator_t<internal::maybe_const_t<Const_, View>>
            >
    friend constexpr std::ranges::range_difference_t<internal::maybe_const_t<Const_, View>>
    operator-(const iterator<Const_>& lhs, const sentinel& rhs) {
        return lhs._current - rhs._end;
    }

    template<bool Const_>
        requires std::sized_sentinel_for<
            std::ranges::sentinel_t<Base>, std::ranges::iterator_t<internal::maybe_const_t<Const_, View>>>
    friend constexpr std::ranges::range_difference_t<internal::maybe_const_t<Const_, View>>
    operator-(const sentinel& lhs, const iterator<Const_>& rhs) {
        return lhs._end - rhs._current;
    }
};


namespace views {

namespace internal {


template<class T>
concept can_enumerate_view =
    requires { enumerate_view<std::views::all_t<T>>(std::declval<T>()); };


} // namespace internal


struct Enumerate : adaptor::range_adaptor_closure<Enumerate> {
    template<std::ranges::viewable_range Range>
        requires internal::can_enumerate_view<Range>
    constexpr auto operator() [[nodiscard]] (Range&& range) const {
        return enumerate_view<std::views::all_t<Range>>(std::forward<Range>(range));
    }
};


inline constexpr Enumerate enumerate;


} // namespace views

} // namespace lib


namespace std::ranges {


template<class T>
inline constexpr bool enable_borrowed_range<lib::enumerate_view<T>> = enable_borrowed_range<T>;


} // namespace std
