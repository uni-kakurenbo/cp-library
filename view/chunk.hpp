#pragma once


#include <concepts>
#include <ranges>


#include "internal/ranges.hpp"

#include "numeric/arithmetic.hpp"


namespace uni {


template<std::ranges::view View>
    requires std::ranges::input_range<View>
struct chunk_view : std::ranges::view_interface<chunk_view<View>> {
  private:
    View _base;
    std::ranges::range_difference_t<View> _n;
    std::ranges::range_difference_t<View> _remainder = 0;
    std::optional<std::ranges::iterator_t<View>> _current;

    struct outer_iterator;
    struct inner_iterator;

  public:
    constexpr explicit chunk_view(View base, std::ranges::range_difference_t<View> n)
        : _base(std::move(base)), _n(n) {
        assert(n >= 0);
    }

    constexpr View base() const &
        requires std::copy_constructible<View>
    {
        return this->_base;
    }

    constexpr View base() && { return std::move(this->_base); }

    constexpr outer_iterator begin() {
        this->_current = std::ranges::begin(this->_base);
        this->_remainder = this->_n;
        return outer_iterator(*this);
    }

    constexpr std::default_sentinel_t end() const noexcept {
        return std::default_sentinel;
    }

    constexpr auto size()
        requires std::ranges::sized_range<View>
    {
        return to_unsigned(div_ceil(std::ranges::distance(this->_base), this->_n));
    }

    constexpr auto size() const
        requires std::ranges::sized_range<const View>
    {
        return to_unsigned(div_ceil(std::ranges::distance(this->_base), this->_n));
    }
};

template<class Range>
chunk_view(Range&&, std::ranges::range_difference_t<Range>) -> chunk_view<std::views::all_t<Range>>;


template<std::ranges::view View>
    requires std::ranges::input_range<View>
struct chunk_view<View>::outer_iterator {
  private:
    chunk_view *_parent;

    constexpr explicit outer_iterator(chunk_view &parent) noexcept
      : _parent(std::addressof(parent))
    {}

    friend chunk_view;

  public:
    using iterator_concept = std::input_iterator_tag;
    using difference_type = std::ranges::range_difference_t<View>;

    struct value_type;

    outer_iterator(outer_iterator &&) = default;
    outer_iterator &operator=(outer_iterator &&) = default;

    constexpr value_type operator*() const {
        assert(*this != std::default_sentinel);
        return value_type(*this->_parent);
    }

    constexpr outer_iterator &operator++() {
        assert(*this != std::default_sentinel);
        std::ranges::advance(*this->_parent->_current, this->_parent->_remainder, std::ranges::end(this->_parent->_base));
        this->_parent->_remainder = this->_parent->_n;
        return *this;
    }

    constexpr void operator++(int) { ++*this; }

    friend constexpr bool operator==(const outer_iterator &lhs, std::default_sentinel_t) {
        return *lhs._parent->_current == std::ranges::end(lhs._parent->_base) && lhs._parent->_remainder != 0;
    }

    friend constexpr difference_type operator-(std::default_sentinel_t, const outer_iterator &rhs)
        requires std::sized_sentinel_for<std::ranges::sentinel_t<View>, std::ranges::iterator_t<View>>
    {
        const auto dist = std::ranges::end(rhs._parent->_base) - *rhs._parent->_current;

        if(dist < rhs._parent->_remainder) return dist == 0 ? 0 : 1;
        return 1 + div_ceil(dist - rhs._parent->_remainder, rhs._parent->_n);
    }

    friend constexpr difference_type operator-(const outer_iterator &lhs, std::default_sentinel_t rhs)
        requires std::sized_sentinel_for<std::ranges::sentinel_t<View>, std::ranges::iterator_t<View>>
    {
        return -(rhs - lhs);
    }
};

template<std::ranges::view View>
    requires std::ranges::input_range<View>
struct chunk_view<View>::outer_iterator::value_type : std::ranges::view_interface<value_type> {
  private:
    chunk_view *_parent;

    constexpr explicit value_type(chunk_view &parent) noexcept
      : _parent(std::addressof(parent))
    {}

    friend outer_iterator;

  public:
    constexpr inner_iterator begin() const noexcept {
        return inner_iterator(*this->_parent);
    }

    constexpr std::default_sentinel_t end() const noexcept {
        return std::default_sentinel;
    }

    constexpr auto size() const
        requires std::sized_sentinel_for<std::ranges::sentinel_t<View>, std::ranges::iterator_t<View>>
    {
        return to_unsigned(std::ranges::min(this->_parent->_remainder, std::ranges::end(this->_parent->_base) - *this->_parent->_current));
    }
};

template<std::ranges::view View>
    requires std::ranges::input_range<View>
struct chunk_view<View>::inner_iterator {
  private:
    chunk_view *_parent;

    constexpr explicit inner_iterator(chunk_view &parent) noexcept
      : _parent(std::addressof(parent))
    {}

    friend outer_iterator::value_type;

  public:
    using iterator_concept = std::input_iterator_tag;
    using difference_type = std::ranges::range_difference_t<View>;
    using value_type = std::ranges::range_value_t<View>;

    inner_iterator(inner_iterator &&) = default;
    inner_iterator &operator=(inner_iterator &&) = default;

    constexpr const std::ranges::iterator_t<View> &base() const & {
        return *this->_parent->_current;
    }

    constexpr std::ranges::range_reference_t<View> operator*() const {
        assert(*this != std::default_sentinel);
        return **this->_parent->_current;
    }

    constexpr inner_iterator &operator++() {
        assert(*this != std::default_sentinel);
        ++*this->_parent->_current;

        if(*this->_parent->_current == std::ranges::end(this->_parent->_base)) {
            this->_parent->_remainder = 0;
        }
        else {
            --this->_parent->_remainder;
        }

        return *this;
    }

    constexpr void operator++(int) { ++*this; }

    friend constexpr bool operator==(const inner_iterator &lhs, std::default_sentinel_t) noexcept {
        return lhs._parent->_remainder == 0;
    }

    friend constexpr difference_type operator-(std::default_sentinel_t, const inner_iterator &rhs)
        requires std::sized_sentinel_for<std::ranges::sentinel_t<View>, std::ranges::iterator_t<View>>
    {
        return std::ranges::min(rhs._parent->_remainder, std::ranges::end(rhs._parent->_base) - *rhs._parent->_current);
    }

    friend constexpr difference_type operator-(const inner_iterator &lhs, std::default_sentinel_t rhs)
        requires std::sized_sentinel_for<std::ranges::sentinel_t<View>, std::ranges::iterator_t<View>>
    {
        return -(rhs - lhs);
    }
};

template<std::ranges::view View>
    requires std::ranges::forward_range<View>
struct chunk_view<View> : std::ranges::view_interface<chunk_view<View>> {
  private:
    View _base;
    std::ranges::range_difference_t<View> _n;
    template<bool> struct iterator;

  public:
    constexpr explicit chunk_view(View base, const std::ranges::range_difference_t<View> n)
        : _base(std::move(base)), _n(n)
    {
        assert(n > 0);
    }

    constexpr View base() const &
        requires std::copy_constructible<View>
    {
        return this->_base;
    }

    constexpr View base() && { return std::move(this->_base); }

    constexpr auto begin()
        requires(!internal::simple_view<View>)
    {
        return iterator<false>(this, std::ranges::begin(this->_base));
    }

    constexpr auto begin() const
        requires std::ranges::forward_range<const View>
    {
        return iterator<true>(this, std::ranges::begin(this->_base));
    }

    constexpr auto end()
        requires(!internal::simple_view<View>)
    {
        if constexpr(std::ranges::common_range<View> && std::ranges::sized_range<View>) {
            const auto missing = (this->_n - std::ranges::distance(this->_base) % this->_n) % this->_n;
            return iterator<false>(this, std::ranges::end(this->_base), missing);
        }
        else if constexpr(std::ranges::common_range<View> && !std::ranges::bidirectional_range<View>) {
            return iterator<false>(this, std::ranges::end(this->_base));
        }
        else {
            return std::default_sentinel;
        }
    }

    constexpr auto end() const
        requires std::ranges::forward_range<const View>
    {
        if constexpr(std::ranges::common_range<const View> && std::ranges::sized_range<const View>) {
            auto missing = (this->_n - std::ranges::distance(this->_base) % this->_n) % this->_n;
            return iterator<true>(this, std::ranges::end(this->_base), missing);
        } else if constexpr(std::ranges::common_range<const View> && !std::ranges::bidirectional_range<const View>) {
            return iterator<true>(this, std::ranges::end(this->_base));
        }
        else {
            return std::default_sentinel;
        }
    }

    constexpr auto size()
        requires std::ranges::sized_range<View>
    {
        return to_unsigned(div_ceil(std::ranges::distance(this->_base), this->_n));
    }

    constexpr auto size() const
        requires std::ranges::sized_range<const View>
    {
        return to_unsigned(div_ceil(std::ranges::distance(this->_base), this->_n));
    }
};


template<std::ranges::view View>
    requires std::ranges::forward_range<View>
template<bool CONST>
struct chunk_view<View>::iterator {
  private:
    using Parent = internal::maybe_const_t<CONST, chunk_view>;
    using Base = internal::maybe_const_t<CONST, View>;

    std::ranges::iterator_t<Base> _current = std::ranges::iterator_t<Base>();
    std::ranges::sentinel_t<Base> _end = std::ranges::sentinel_t<Base>();
    std::ranges::range_difference_t<Base> _n = 0;
    std::ranges::range_difference_t<Base> _missing = 0;

    constexpr iterator(Parent *parent, std::ranges::iterator_t<Base> current, const std::ranges::range_difference_t<Base> missing = 0)
      : _current(current), _end(std::ranges::end(parent->_base)), _n(parent->_n), _missing(missing)
    {}

    friend chunk_view;

  public:
    using iterator_category = std::input_iterator_tag;
    using iterator_concept = internal::most_primitive_iterator_concept<false, Base>;

    using value_type = decltype(std::views::take(std::ranges::subrange(_current, _end), _n));
    using difference_type = std::ranges::range_difference_t<Base>;

    iterator() = default;

    constexpr iterator(iterator<!CONST> itr)
        requires
            CONST &&
            std::convertible_to<std::ranges::iterator_t<View>, std::ranges::iterator_t<Base>> &&
            std::convertible_to<std::ranges::sentinel_t<View>, std::ranges::sentinel_t<Base>>
      : _current(std::move(itr._current)), _end(std::move(itr._end)), _n(itr._n), _missing(itr._missing)
    {}

    constexpr std::ranges::iterator_t<Base> base() const { return this->_current; }

    constexpr value_type operator*() const {
        assert(this->_current != this->_end);
        return std::views::take(std::ranges::subrange(this->_current, this->_end), this->_n);
    }

    constexpr iterator &operator++() {
        assert(this->_current != this->_end);
        this->_missing = std::ranges::advance(this->_current, this->_n, this->_end);
        return *this;
    }

    constexpr iterator operator++(int) {
        auto temp = *this;
        return ++*this, temp;
    }

    constexpr iterator &operator--()
        requires std::ranges::bidirectional_range<Base>
    {
        std::ranges::advance(this->_current, this->_missing - this->_n);
        this->_missing = 0;
        return *this;
    }

    constexpr iterator operator--(int)
        requires std::ranges::bidirectional_range<Base>
    {
        auto temp = *this;
        return --*this, temp;
    }

    constexpr iterator &operator+=(difference_type lhs)
        requires std::ranges::random_access_range<Base>
    {
        if(lhs > 0) {
            assert(std::ranges::distance(this->_current, this->_end) > this->_n * (lhs - 1));
            this->_missing = std::ranges::advance(this->_current, this->_n * lhs, this->_end);
        }
        else if(lhs < 0) {
            std::ranges::advance(this->_current, this->_n * lhs + this->_missing);
            this->_missing = 0;
        }

        return *this;
    }

    constexpr iterator &operator-=(difference_type lhs)
        requires std::ranges::random_access_range<Base>
    {
        return *this += -lhs;
    }

    constexpr value_type operator[](difference_type n) const
        requires std::ranges::random_access_range<Base>
    {
        return *(*this + n);
    }

    friend constexpr bool operator==(const iterator &lhs, const iterator &rhs) {
        return lhs._current == rhs._current;
    }

    friend constexpr bool operator==(const iterator &lhs, std::default_sentinel_t) {
        return lhs._current == lhs._end;
    }

    friend constexpr bool operator<(const iterator &lhs, const iterator &rhs)
        requires std::ranges::random_access_range<Base>
    {
        return lhs._current > rhs._current;
    }

    friend constexpr bool operator>(const iterator &lhs, const iterator &rhs)
        requires std::ranges::random_access_range<Base>
    {
        return rhs < lhs;
    }

    friend constexpr bool operator<=(const iterator &lhs, const iterator &rhs)
        requires std::ranges::random_access_range<Base>
    {
        return !(rhs < lhs);
    }

    friend constexpr bool operator>=(const iterator &lhs, const iterator &rhs)
        requires std::ranges::random_access_range<Base>
    {
        return !(lhs < rhs);
    }

    friend constexpr auto operator<=>(const iterator &lhs, const iterator &rhs)
        requires std::ranges::random_access_range<Base> && std::three_way_comparable<std::ranges::iterator_t<Base>>
    {
        return lhs._current <=> rhs._current;
    }

    friend constexpr iterator operator+(const iterator &itr, const difference_type count)
        requires std::ranges::random_access_range<Base>
    {
        auto res = itr;
        return res += count, res;
    }

    friend constexpr iterator operator+(const difference_type count, const iterator &itr)
        requires std::ranges::random_access_range<Base>
    {
        auto res = itr;
        return res += count, res;
    }

    friend constexpr iterator operator-(const iterator &itr, const difference_type count)
        requires std::ranges::random_access_range<Base>
    {
        auto res = itr;
        return res -= count, res;
    }

    friend constexpr difference_type operator-(const iterator &lhs, const iterator &rhs)
        requires std::sized_sentinel_for<std::ranges::iterator_t<Base>, std::ranges::iterator_t<Base>>
    {
        return (lhs._current - rhs._current + lhs._missing - rhs._missing) / lhs._n;
    }

    friend constexpr difference_type operator-(std::default_sentinel_t rhs, const iterator &lhs)
        requires std::sized_sentinel_for<std::ranges::sentinel_t<Base>, std::ranges::iterator_t<Base>>
    {
        return div_ceil(lhs._end - lhs._current, lhs._n);
    }

    friend constexpr difference_type operator-(const iterator &lhs, std::default_sentinel_t rhs)
        requires std::sized_sentinel_for<std::ranges::sentinel_t<Base>, std::ranges::iterator_t<Base>>
    {
        return -(rhs - lhs);
    }
};


namespace views {
namespace internal {


template<class Range, typename Diff>
concept can_chunk_view = requires { chunk_view(std::declval<Range>(), std::declval<Diff>()); };


} // namespace internal


struct Chunk : adaptor::range_adaptor<Chunk> {
    template<std::ranges::viewable_range Range, class Diff = std::ranges::range_difference_t<Range>>
        requires internal::can_chunk_view<Range, Diff>
    constexpr auto operator() [[nodiscard]] (Range &&res, std::type_identity_t<Diff> n) const {
        return chunk_view(std::forward<Range>(res), n);
    }

    using adaptor::range_adaptor<Chunk>::operator();

    static constexpr int arity = 2;
    static constexpr bool has_simple_extra_args = true;
};


inline constexpr Chunk chunk;


} // namespace views

} // namespace uni


namespace std::ranges {


template<class View>
inline constexpr bool enable_borrowed_range<uni::chunk_view<View>> = forward_range<View> && enable_borrowed_range<View>;


} // namespace std::ranges
