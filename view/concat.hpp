#pragma once


#include <cassert>
#include <iterator>
#include <utility>
#include <tuple>
#include <type_traits>
#include <functional>
#include <ranges>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/type_traits.hpp"
#include "internal/ranges.hpp"



namespace uni {

namespace internal {

namespace view_impl {


template<std::ranges::input_range V0, std::ranges::input_range V1>
    requires std::ranges::view<V0> && std::ranges::view<V1>
struct concat_view : std::ranges::view_interface<concat_view<V0, V1>> {
  private:
    V0 _b0;
    V1 _b1;

    template<bool Const> using B0 = internal::maybe_const_t<Const, V0>;
    template<bool Const> using B1 = internal::maybe_const_t<Const, V1>;

    template<bool Const> struct iterator_tag {};

    template<bool Const>
        requires std::ranges::forward_range<B0<Const>> && std::ranges::forward_range<B1<Const>>
    struct iterator_tag<Const> {
      public:
        using iterator_category = uni::internal::most_primitive_iterator_tag<
            typename std::iterator_traits<std::ranges::iterator_t<B0<Const>>>::iterator_category,
            typename std::iterator_traits<std::ranges::iterator_t<B1<Const>>>::iterator_category
        >;
    };

  public:
    template<bool> class iterator;

    constexpr explicit concat_view(V0 v0, V1 v1) noexcept(NO_EXCEPT)
      : _b0(std::move(v0)), _b1(std::move(v1))
    {}

    inline constexpr std::pair<V0, V1> base() const & noexcept(NO_EXCEPT)
        requires std::copy_constructible<V0> && std::copy_constructible<V0>
    {
        return { this->_b0, this->_b1 };
    }

    inline constexpr std::pair<V0,V1> base() && noexcept(NO_EXCEPT) {
        return { std::move(this->_b0), std::move(this->_b1) };
    }

    inline constexpr auto begin() noexcept(NO_EXCEPT)
        requires (!internal::simple_view<V0> && !internal::simple_view<V1>)
    {
        return iterator<false>(this, std::ranges::begin(this->_b0), std::ranges::begin(this->_b1), 0);
    }

    inline constexpr auto begin() const noexcept(NO_EXCEPT)
        requires std::ranges::range<const V0> && std::ranges::range<const V1>
    {
        return iterator<true>(this, std::ranges::begin(this->_b0), std::ranges::begin(this->_b1), 0);
    }

    inline constexpr auto end() noexcept(NO_EXCEPT)
        requires (!internal::simple_view<V0> && !internal::simple_view<V1>)
    {
        if constexpr(std::ranges::common_range<V0> && std::ranges::common_range<V1>) {
            return iterator<false>(this, std::ranges::end(this->_b0), std::ranges::end(this->_b1), 1);
        }
        else {
            return std::default_sentinel;
        }
    }

    inline constexpr auto end() const noexcept(NO_EXCEPT)
        requires std::ranges::range<const V0> && std::ranges::range<const V1>
    {
        if constexpr(std::ranges::common_range<const V0> && std::ranges::common_range<const V1>) {
            return iterator<true>(this, std::ranges::end(this->_b0), std::ranges::end(this->_b1), 1);
        }
        else {
            return std::default_sentinel;
        }
    }

    inline constexpr auto size() noexcept(NO_EXCEPT)
        requires std::ranges::sized_range<V0> && std::ranges::sized_range<V1>
    {
        return static_cast<std::size_t>(std::ranges::distance(this->_b0) + std::ranges::distance(this->_b1));
    }

    inline constexpr auto size() const noexcept(NO_EXCEPT)
        requires std::ranges::sized_range<const V0> && std::ranges::sized_range<const V1>
    {
        return static_cast<std::size_t>(std::ranges::distance(this->_b0) + std::ranges::distance(this->_b1));
    }
};

template<std::ranges::input_range V0, std::ranges::input_range V1>
    requires std::ranges::view<V0> && std::ranges::view<V1>
template<bool Const>
struct concat_view<V0, V1>::iterator : iterator_tag<Const> {
  private:
    using Parent = internal::maybe_const_t<Const, concat_view>;

    using B0 = concat_view::B0<Const>;
    using B1 = concat_view::B1<Const>;

    std::ranges::iterator_t<B0> _c0 = std::ranges::iterator_t<B0>();
    std::ranges::iterator_t<B0> _b0 = std::ranges::iterator_t<B0>();
    std::ranges::sentinel_t<B0> _e0 = std::ranges::sentinel_t<B0>();

    std::ranges::iterator_t<B1> _c1 = std::ranges::iterator_t<B1>();
    std::ranges::iterator_t<B1> _b1 = std::ranges::iterator_t<B1>();
    std::ranges::sentinel_t<B1> _e1 = std::ranges::sentinel_t<B1>();

    int _block = 0;

    constexpr iterator(Parent *const parent, const std::ranges::iterator_t<B0> c0, const std::ranges::iterator_t<B1> c1, const int block) noexcept(NO_EXCEPT)
      : _c0(std::move(c0)), _b0(std::ranges::begin(parent->_b0)), _e0(std::ranges::end(parent->_b0)),
        _c1(std::move(c1)), _b1(std::ranges::begin(parent->_b1)), _e1(std::ranges::end(parent->_b1)),
        _block(block)
    {}

    friend concat_view;

  public:
    using difference_type = std::common_type_t<std::ranges::range_difference_t<B0>, std::ranges::range_difference_t<B1>>;

    using value_type = std::common_type_t<std::ranges::range_value_t<B0>, std::ranges::range_value_t<B1>>;
    using reference_type = std::common_reference_t<std::ranges::range_reference_t<B0>, std::ranges::range_reference_t<B1>>;

    using iterator_concept = most_primitive_iterator_concept<Const, V0, V1>;

    iterator() noexcept(NO_EXCEPT)
        requires std::default_initializable<std::ranges::iterator_t<B0>> &&
                 std::default_initializable<std::ranges::iterator_t<B0>>
    = default;

    constexpr iterator(iterator<!Const> itr) noexcept(NO_EXCEPT)
        requires
            Const &&
            std::convertible_to<std::ranges::iterator_t<V0>, std::ranges::iterator_t<B0>> &&
            std::convertible_to<std::ranges::sentinel_t<V0>, std::ranges::sentinel_t<B0>> &&
            std::convertible_to<std::ranges::iterator_t<V1>, std::ranges::iterator_t<B1>> &&
            std::convertible_to<std::ranges::sentinel_t<V1>, std::ranges::sentinel_t<B1>>
      : _c0(std::move(itr._c0)), _b0(std::move(itr._b0)), _e0(std::move(itr._e0)),
        _c1(std::move(itr._c0)), _b1(std::move(itr._b0)), _e1(std::move(itr._e1)),
        _block(itr._block)
    {}

    inline constexpr std::variant<std::ranges::iterator_t<B0>, std::ranges::iterator_t<B1>>
    base() && noexcept(NO_EXCEPT) {
        if(this->_block == 0) return std::move(this->_c0);
        else return std::move(this->_C1);
    }

    inline constexpr
        std::variant<
            std::reference_wrapper<const std::ranges::iterator_t<B0>>,
            std::reference_wrapper<const std::ranges::iterator_t<B1>>
        >
    base() const & noexcept {
        if(this->_block == 0) return std::move(this->_c0);
        else return std::move(this->_c1);
    }

    inline constexpr reference_type operator*() const noexcept(NO_EXCEPT)
    {

        if(this->_block == 0) return *this->_c0;
        else return *this->_c1;
    }

    inline constexpr iterator& operator++() noexcept(NO_EXCEPT)
    {
        assert(this->_c0 != this->_e0 or this->_c1 != this->_e1);

        if(this->_block == 0) {
            if(++this->_c0 == this->_e0) {
                this->_block = 1;
                assert(this->_c1 == this->_b1);
            }
        }
        else {
            ++this->_c1;
        }

        return *this;
    }

    inline constexpr void operator++(int) noexcept(NO_EXCEPT) { ++*this; }

    inline constexpr iterator operator++(int) noexcept(NO_EXCEPT)
        requires std::ranges::forward_range<B0> && std::ranges::forward_range<B1>
    {
        const auto res = *this; ++*this; return res;
    }

    inline constexpr iterator& operator--() noexcept(NO_EXCEPT)
        requires
            std::ranges::bidirectional_range<B0> && std::ranges::bidirectional_range<B1> &&
            std::bidirectional_iterator<std::ranges::sentinel_t<B0>>
    {
        if(this->_block == 1) {
            if(this->_c1 == this->_b1) {
                this->_block = 0;
                this->_c0 = std::ranges::prev(this->_e0);
            }
            else {
                --this->_c1;
            }
        }
        else {
            --this->_c0;
        }

        return *this;
    }

    inline constexpr iterator operator--(int) noexcept(NO_EXCEPT)
        requires std::ranges::bidirectional_range<B0> && std::ranges::bidirectional_range<B1>
    {
        const auto res = *this; --*this; return res;
    }

    inline constexpr iterator& operator+=(const difference_type diff) noexcept(NO_EXCEPT)
        requires
            std::ranges::random_access_range<B0> && std::ranges::random_access_range<B1>
    {
        if(diff > 0) {
            if(this->_block == 0) {
                const auto missing = std::ranges::advance(this->_c0, diff, this->_e0);
                if(this->_c0 == this->_e0) {
                    this->_block = 1;
                    assert(this->_c1 == this->_b1);
                    std::ranges::advance(this->_c1, missing, this->_e1);
                }
            }
            else {
                std::ranges::advance(this->_c1, diff, this->_e1);
            }
        }
        if(diff < 0) {
            if(this->_block == 1) {
                const auto missing = std::ranges::advance(this->_c1, diff, this->_b1);
                if(missing < 0) {
                    this->_block = 0;
                    assert(this->_c0 == this->_e0);
                    std::ranges::advance(this->_c0, missing, this->_b0);
                }
            }
            else {
                std::ranges::advance(this->_c0, diff, this->_b0);
            }
        }

        return *this;
    }

    inline constexpr iterator& operator-=(const difference_type diff) noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<B0> && std::ranges::random_access_range<B1>
    {
        return *this += -diff;
    }

    inline constexpr decltype(auto) operator[](const difference_type diff) const noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<B0> && std::ranges::random_access_range<B1>
    {
        return *(*this + diff);
    }

    friend inline constexpr bool operator==(const iterator& lhs, std::default_sentinel_t) noexcept(NO_EXCEPT)
    {
        if(lhs._block == 0) return false;
        if(lhs._block == 1) return lhs._c1 == lhs._e1;
        assert(false);
    }

    friend inline constexpr bool operator==(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT)
        requires
            std::equality_comparable<std::ranges::iterator_t<B0>> &&
            std::equality_comparable<std::ranges::iterator_t<B1>>
    {
        if(lhs._block != rhs._block) return false;
        return lhs._block == 0 ? lhs._c0 == rhs._c0 : lhs._c1 == rhs._c1;
    }

    friend inline constexpr auto operator<=>(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<B0> && std::ranges::random_access_range<B1>
    {
        if(lhs._block != rhs._block) return lhs._block <=> rhs._block;
        return lhs._block == 0 ? lhs._c0 <=> rhs._c0 : lhs._c1 <=> rhs._c1;
    }

    friend inline constexpr iterator operator+(const iterator& itr, const difference_type diff) noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<B0> && std::ranges::random_access_range<B1>
    {
        auto res = itr; res += diff; return res;
    }

    friend inline constexpr iterator operator+(const difference_type diff, const iterator& itr) noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<B0> && std::ranges::random_access_range<B1>
    {
        return itr + diff;
    }

    friend inline constexpr iterator operator-(const iterator& itr, const difference_type diff) noexcept(NO_EXCEPT)
        requires std::ranges::random_access_range<B0> && std::ranges::random_access_range<B1>
    {
        auto res = itr; res -= diff; return res;
    }

    friend inline constexpr const difference_type operator-(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT)
        requires
            std::sized_sentinel_for<std::ranges::iterator_t<B0>, std::ranges::iterator_t<B0>> &&
            std::sized_sentinel_for<std::ranges::iterator_t<B1>, std::ranges::iterator_t<B1>>
    {
        if(lhs._block == rhs._block) {
            return lhs._block == 0 ? std::ranges::distance(rhs._c0, lhs._c0) : std::ranges::distance(rhs._c1, lhs._c1);
        }
        if(lhs._block > rhs._block) return std::ranges::distance(rhs._c0, rhs._e0) + std::ranges::distance(lhs._b1, lhs._c1);
        if(lhs._block < rhs._block) return -(rhs - lhs);
        assert(false);
    }

    friend inline constexpr const difference_type operator-(std::default_sentinel_t, const iterator& rhs) noexcept(NO_EXCEPT)
        requires
            std::sized_sentinel_for<std::ranges::sentinel_t<B0>, std::ranges::iterator_t<B0>> &&
            std::sized_sentinel_for<std::ranges::sentinel_t<B1>, std::ranges::iterator_t<B1>>
    {
        if(rhs._block == 0) return std::ranges::distance(rhs._c0, rhs._e0) + std::ranges::distance(rhs._b1, rhs._e1);
        if(rhs._block == 1) return std::ranges::distance(rhs._c1, rhs._e1);
        assert(false);
    }

    friend inline constexpr const difference_type operator-(const iterator& lhs, std::default_sentinel_t rhs) noexcept(NO_EXCEPT)
        requires
            std::sized_sentinel_for<std::ranges::sentinel_t<B0>, std::ranges::iterator_t<B0>> &&
            std::sized_sentinel_for<std::ranges::sentinel_t<B1>, std::ranges::iterator_t<B1>>
    {
        return -(rhs - lhs);
    }

    friend inline constexpr
    std::common_reference_t<
        std::ranges::range_rvalue_reference_t<B0>,
        std::ranges::range_rvalue_reference_t<B1>
    >
    iter_move(const iterator& itr) noexcept(NO_EXCEPT)
    {
        if(itr._block == 0) return std::ranges::iter_move(itr._c0);
        if(itr._block == 1) return std::ranges::iter_move(itr._c1);
        assert(false);
    }

    friend inline constexpr void iter_swap(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT)
        requires
            std::indirectly_swappable<std::ranges::iterator_t<B0>> &&
            std::indirectly_swappable<std::ranges::iterator_t<B1>> &&
            std::indirectly_swappable<std::ranges::iterator_t<B0>, std::ranges::iterator_t<B1>>
    {
        if(lhs._block == 0 && rhs._block == 0) std::ranges::iter_swap(lhs._c0, rhs._c0);
        if(lhs._block == 0 && rhs._block == 1) std::ranges::iter_swap(lhs._c0, rhs._c1);
        if(lhs._block == 1 && rhs._block == 0) std::ranges::iter_swap(lhs._c1, rhs._c0);
        if(lhs._block == 1 && rhs._block == 1) std::ranges::iter_swap(lhs._c1, rhs._c1);
        assert(false);
    }
};


} // namespace view_impl

} // namespace internal


template<class...> struct concat_view;

template<class T>
struct concat_view<T> : std::views::all_t<T> {
    using std::views::all_t<T>::all_t;
};

template<class T0, class T1>
struct concat_view<T0, T1> : internal::view_impl::concat_view<std::views::all_t<T0>, std::views::all_t<T1>> {
    explicit concat_view(T0&& v0, T1&& v1) noexcept(NO_EXCEPT)
      : internal::view_impl::concat_view<std::views::all_t<T0>, std::views::all_t<T1>>(std::forward<T0>(v0), std::forward<T1>(v1))
    {}
};

template<class T0, class T1, class... Ts>
struct concat_view<T0, T1, Ts...> : concat_view<concat_view<T0, T1>, Ts...> {
    explicit concat_view(T0&& v0, T1&& v1, Ts&&... vs) noexcept(NO_EXCEPT)
      : concat_view<concat_view<T0, T1>, Ts...>(
            concat_view<T0, T1>(std::forward<T0>(v0), std::forward<T1>(v1)), std::forward<Ts>(vs)...
        )
    {}
};

namespace views {

namespace internal {


template<class... Ts>
concept can_concat_view = requires { concat_view<Ts...>(std::declval<Ts>()...); };


} // namespace internal


struct Concat {
    template<class... Ts>
        requires (sizeof...(Ts) == 0 || internal::can_concat_view<Ts...>)
    inline constexpr auto operator() [[nodiscard]] (Ts&&... vs) const {
        if constexpr(sizeof...(Ts) == 0) return std::views::empty<std::nullptr_t>;
        else return concat_view<std::views::all_t<Ts>...>(std::forward<Ts>(vs)...);
    }
};


inline constexpr Concat concat;


} // namespace views

} // namespace uni.


namespace std::ranges {


template<class... Views>
inline constexpr bool enable_borrowed_range<uni::concat_view<Views...>> = (enable_borrowed_range<Views> && ...);


}
