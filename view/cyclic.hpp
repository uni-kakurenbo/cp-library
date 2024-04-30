#pragma once

#include <cassert>
#include <ranges>
#include <concepts>
#include <tuple>
#include <iterator>

#include "internal/dev_env.hpp"
#include "internal/ranges.hpp"
#include "internal/type_traits.hpp"

#include "utility/functional.hpp"
#include "numeric/arithmetic.hpp"


namespace uni {


template<std::ranges::input_range View>
    requires std::ranges::view<View>
struct cyclic_view : std::ranges::view_interface<cyclic_view<View>> {
  private:
    View _base;

    template<bool Const> using Base = internal::maybe_const_t<Const, View>;

    template<bool Const> struct iterator_tag {};

    template<bool Const>
        requires std::ranges::forward_range<Base<Const>>
    struct iterator_tag<Const> {
      private:
        static constexpr auto _iterator_category() noexcept {
            using category = typename std::iterator_traits<std::ranges::iterator_t<Base<Const>>>::iterator_category;
            if constexpr(std::derived_from<category, std::random_access_iterator_tag>)
                return std::random_access_iterator_tag{};
            else
                return category{};
        }

      public:
        using iterator_category = decltype(_iterator_category());
    };

  public:
    template<bool> class iterator;

    constexpr explicit cyclic_view(View base) noexcept(NO_EXCEPT) : _base(std::move(base)) {}

    inline constexpr View base() const & noexcept(NO_EXCEPT) requires std::copy_constructible<View>
    {
        return this->_base;
    }

    inline constexpr View base() && noexcept(NO_EXCEPT) { return std::move(this->_base); }

    inline constexpr auto begin() noexcept(NO_EXCEPT)
        requires(!internal::simple_view<View>)
    {
        return iterator<false>(this, std::ranges::begin(this->_base));
    }

    inline constexpr auto begin() const noexcept(NO_EXCEPT)
        requires std::ranges::range<const View>
    {
        return iterator<true>(this, std::ranges::begin(this->_base));
    }

    inline constexpr auto end() noexcept(NO_EXCEPT)
        requires(!internal::simple_view<View>)
    {
        return std::unreachable_sentinel;
    }

    inline constexpr auto end() const noexcept(NO_EXCEPT)
        requires std::ranges::range<const View>
    {
        return std::unreachable_sentinel;
    }
};

template<class Range>
cyclic_view(Range &&) -> cyclic_view<std::views::all_t<Range>>;


template<std::ranges::input_range View>
    requires std::ranges::view<View>
template<bool Const>
struct cyclic_view<View>::iterator : iterator_tag<Const> {
  private:
    using Parent = internal::maybe_const_t<Const, cyclic_view>;
    using Base = cyclic_view::Base<Const>;

    std::ranges::iterator_t<Base> _current = std::ranges::iterator_t<Base>();
    std::ranges::iterator_t<Base> _begin = std::ranges::iterator_t<Base>();
    std::ranges::sentinel_t<Base> _end = std::ranges::sentinel_t<Base>();

    constexpr iterator(Parent *const parent, const std::ranges::iterator_t<Base> current) noexcept(NO_EXCEPT)
      : _current(std::move(current)), _begin(std::ranges::begin(parent->_base)), _end(std::ranges::end(parent->_base))
    {}

    friend cyclic_view;

  public:
    using difference_type = std::ranges::range_difference_t<Base>;
    using value_type = std::ranges::range_value_t<Base>;
    using iterator_concept = typename internal::iterator_concept<Base>;

  private:
    difference_type _index = 0;

  public:
    iterator() noexcept(NO_EXCEPT) requires std::default_initializable<std::ranges::iterator_t<Base>> = default;

    constexpr iterator(iterator<!Const> itr) noexcept(NO_EXCEPT)
        requires
            Const && std::convertible_to<std::ranges::iterator_t<View>, std::ranges::iterator_t<Base>> &&
            std::convertible_to<std::ranges::sentinel_t<View>, std::ranges::sentinel_t<Base>>
      : _current(std::move(itr._current)), _begin(std::move(itr._begin)), _end(std::move(itr._end)), _index(std::move(itr._index))
    {}

    inline constexpr std::ranges::iterator_t<Base> base() && noexcept(NO_EXCEPT) { return std::move(this->_current); }

    inline constexpr const std::ranges::iterator_t<Base> &base() const & noexcept { return this->_current; }

    inline constexpr decltype(auto) operator*() const noexcept(NO_EXCEPT) { return *this->_current; }

    inline constexpr iterator& operator++() noexcept(NO_EXCEPT)
    {
        assert(this->_current != _end);
        ++this->_index;

        ++this->_current;
        if(this->_current == this->_end) this->_current = this->_begin;

        return *this;
    }

    inline constexpr void operator++(int) noexcept(NO_EXCEPT) { ++*this; }

    inline constexpr iterator operator++(int) noexcept(NO_EXCEPT)
        requires std::ranges::forward_range<Base>
    {
        const auto res = *this; ++*this; return res;
    }

    inline constexpr iterator& operator--() noexcept(NO_EXCEPT)
        requires std::ranges::bidirectional_range<Base>
    {
        --this->_index;

        if(this->_current == this->_begin) this->_current = std::ranges::prev(this->_end);
        else --this->_current;

        return *this;
    }

    inline constexpr iterator operator--(int) noexcept(NO_EXCEPT)
        requires std::ranges::bidirectional_range<Base>
    {
        const auto res = *this; --*this; return res;
    }

    inline constexpr iterator& operator+=(const difference_type diff) noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<Base>
    {
        this->_index += diff;

        if(diff > 0) {
            auto missing = std::ranges::advance(this->_current, diff, this->_end);
            if constexpr(std::ranges::sized_range<Base>) missing %= std::ranges::distance(this->_begin, this->_end);
            while(this->_current == this->_end) {
                this->_current = this->_begin;
                missing = std::ranges::advance(this->_current, missing, this->_end);
            }
        }
        else if(diff < 0) {
            auto missing = std::ranges::advance(this->_current, diff, this->_begin);
            if constexpr(std::ranges::sized_range<Base>) missing %= std::ranges::distance(this->_begin, this->_end);
            while(missing < 0) {
                this->_current = this->_end;
                missing = std::ranges::advance(this->_current, missing, this->_begin);
            }
        }

        return *this;
    }

    inline constexpr iterator& operator-=(const difference_type diff) noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<Base>
    {
        return *this += -diff;
    }

    inline constexpr decltype(auto) operator[](const difference_type diff) const noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<Base>
    {
        return *(*this + diff);
    }

    friend inline constexpr bool operator==(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT)
        requires std::equality_comparable<std::ranges::iterator_t<Base>>
    {
        return lhs._index == rhs._index;
    }

    friend inline constexpr auto operator<=>(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<Base> && std::three_way_comparable<std::ranges::iterator_t<Base>>
    {
        return rhs._index <=> lhs._index;
    }

    friend inline constexpr iterator operator+(const iterator& itr, const difference_type diff) noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<Base>
    {
        auto res = itr; res += diff; return res;
    }

    friend inline constexpr iterator operator+(const difference_type diff, const iterator& itr) noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<Base>
    {
        return itr + diff;
    }


    friend inline constexpr iterator operator-(const iterator& itr, const difference_type diff) noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<Base>
    {
        auto res = itr; res -= diff; return res;
    }

    friend inline constexpr const difference_type operator-(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT)
        requires std::sized_sentinel_for<std::ranges::iterator_t<Base>, std::ranges::iterator_t<Base>>
    {
        return lhs._index - rhs._index;
    }

    friend inline constexpr std::ranges::range_rvalue_reference_t<Base> iter_move(const iterator& itr) noexcept(NO_EXCEPT)
    {
        return std::ranges::iter_move(itr._current);
    }

    friend inline constexpr void iter_swap(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT)
        requires std::indirectly_swappable<std::ranges::iterator_t<Base>>
    {
        std::swap(lhs._index, rhs._index);
        std::ranges::iter_swap(lhs._current, rhs._current);
    }
};


namespace views {

namespace internal {


template<class Range>
concept can_cyclic_view = requires { cyclic_view(std::declval<Range>()); };

}

struct Cyclic : adaptor::range_adaptor_closure<Cyclic> {
    template<std::ranges::viewable_range Range>
        requires internal::can_cyclic_view<Range>
    inline constexpr auto operator() [[nodiscard]] (Range &&res) const
    {
        return cyclic_view(std::forward<Range>(res));
    }

    // using adaptor::range_adaptor_closure<Cyclic>::operator();
    // static constexpr int arity = 1;
    // static constexpr bool has_simple_call_op = true;
};

inline constexpr Cyclic cyclic;


} // namespace views

} // namespace uni


namespace std::ranges {


template<class View>
inline constexpr bool enable_borrowed_range<uni::cyclic_view<View>> = enable_borrowed_range<View>;


} // namespace std::ranges
