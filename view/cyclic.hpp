// #pragma once

// #include <cassert>
// #include <ranges>
// #include <concepts>
// #include <tuple>
// #include <iterator>

// #include "internal/dev_env.hpp"
// #include "internal/ranges.hpp"
// #include "internal/type_traits.hpp"

// #include "utility/functional.hpp"

// #include "numeric/arithmetic.hpp"


// namespace lib {


// template<std::ranges::input_range View>
//     requires std::ranges::view<View>
// struct cyclic_view : std::ranges::view_interface<stride_view<View>> {
//   private:
//     View _base;

//     template<bool Const> using Base = internal::maybe_const_t<Const, View>;

//     template<bool Const> struct iterator_tag {};

//     template<bool Const>
//         requires std::ranges::forward_range<Base<Const>>
//     struct iterator_tag<Const> {
//       private:
//         inline static constexpr auto _iterator_category() noexcept {
//             using category = typename std::iterator_traits<std::ranges::iterator_t<Base<Const>>>::iterator_category;
//             if constexpr(std::derived_from<category, std::random_access_iterator_tag>)
//                 return std::random_access_iterator_tag{};
//             else
//                 return category{};
//         }

//       public:
//         using iterator_category = decltype(_iterator_category());
//     };

//   public:
//     template<bool> class iterator;

//     constexpr explicit cyclic_view(View base) noexcept(NO_EXCEPT) : _base(std::move(base)) {}

//     inline constexpr View base() const & noexcept(NO_EXCEPT) requires std::copy_constructible<View>
//     {
//         return this->_base;
//     }

//     inline constexpr View base() && noexcept(NO_EXCEPT) { return std::move(this->_base); }

//     inline constexpr auto begin() noexcept(NO_EXCEPT)
//         requires(!internal::simple_view<View>)
//     {
//         return iterator<false>(this, std::ranges::begin(this->_base));
//     }

//     inline constexpr auto begin() const noexcept(NO_EXCEPT)
//         requires std::ranges::range<const View>
//     {
//         return iterator<true>(this, std::ranges::begin(this->_base));
//     }

//     inline constexpr auto end() noexcept(NO_EXCEPT)
//         requires(!internal::simple_view<View>)
//     {
//         if constexpr(std::ranges::common_range<View> && std::ranges::sized_range<View> &&
//                      std::ranges::forward_range<View>) {
//             const auto missing = (this->_stride - std::ranges::distance(this->_base) % this->_stride) % this->_stride;
//             return iterator<false>(this, std::ranges::end(this->_base), missing);
//         } else if constexpr(std::ranges::common_range<View> && !std::ranges::bidirectional_range<View>)
//             return iterator<false>(this, std::ranges::end(this->_base));
//         else
//             return std::default_sentinel;
//     }

//     inline constexpr auto end() const noexcept(NO_EXCEPT)
//         requires std::ranges::range<const View>
//     {
//         if constexpr(std::ranges::common_range<const View> && std::ranges::sized_range<const View> && std::ranges::forward_range<const View>)
//         {
//             const auto missing = (this->_stride - std::ranges::distance(this->_base) % this->_stride) % this->_stride;
//             return iterator<true>(this, std::ranges::end(this->_base), missing);
//         }
//         else if constexpr(std::ranges::common_range<const View> && !std::ranges::bidirectional_range<const View>)
//             return iterator<true>(this, std::ranges::end(this->_base));
//         else
//             return std::default_sentinel;
//     }

//     inline constexpr auto size() noexcept(NO_EXCEPT)
//         requires std::ranges::sized_range<View>
//     {
//         return to_unsigned(div_ceil(std::ranges::distance(this->_base), this->_stride));
//     }

//     inline constexpr auto size() const noexcept(NO_EXCEPT)
//         requires std::ranges::sized_range<const View>
//     {
//         return to_unsigned(div_ceil(std::ranges::distance(this->_base), this->_stride));
//     }
// };

// template<class Range>
// stride_view(Range &&, std::ranges::range_difference_t<Range>) -> stride_view<std::views::all_t<Range>>;

// template<std::ranges::input_range View>
//     requires std::ranges::view<View>
// template<bool Const>
// struct stride_view<View>::iterator : iterator_tag<Const> {
//   private:
//     using Parent = internal::maybe_const_t<Const, stride_view>;
//     using Base = stride_view::Base<Const>;

//     std::ranges::iterator_t<Base> _current = std::ranges::iterator_t<Base>();
//     std::ranges::sentinel_t<Base> _end = std::ranges::sentinel_t<Base>();
//     std::ranges::range_difference_t<Base> _stride = 0;
//     std::ranges::range_difference_t<Base> _missing = 0;

//     constexpr iterator(Parent *const parent, const std::ranges::iterator_t<Base> current, const std::ranges::range_difference_t<Base> missing = 0) noexcept(NO_EXCEPT)
//       : _current(std::move(current)), _end(std::ranges::end(parent->_base)), _stride(parent->_stride), _missing(missing)
//     {}

//     friend stride_view;

//   public:
//     using difference_type = std::ranges::range_difference_t<Base>;
//     using value_type = std::ranges::range_value_t<Base>;
//     using iterator_concept = typename internal::iterator_concept<Base>;

//     iterator() noexcept(NO_EXCEPT) requires std::default_initializable<std::ranges::iterator_t<Base>> = default;

//     constexpr iterator(iterator<!Const> itr) noexcept(NO_EXCEPT)
//         requires
//             Const && std::convertible_to<std::ranges::iterator_t<View>, std::ranges::iterator_t<Base>> &&
//             std::convertible_to<std::ranges::sentinel_t<View>, std::ranges::sentinel_t<Base>>
//       : _current(std::move(itr._current)),_end(std::move(itr._end)), _stride(itr._stride), _missing(itr._missing)
//     {}

//     inline constexpr std::ranges::iterator_t<Base> base() && noexcept(NO_EXCEPT) { return std::move(this->_current); }

//     inline constexpr const std::ranges::iterator_t<Base> &base() const & noexcept {
//         return this->_current;
//     }

//     inline constexpr decltype(auto) operator*() const noexcept(NO_EXCEPT)
//     {
//         return *this->_current;
//     }

//     inline constexpr iterator &operator++() noexcept(NO_EXCEPT)
//     {
//         assert(this->_current != _end);
//         this->_missing = std::ranges::advance(this->_current, this->_stride, this->_end);
//         return *this;
//     }

//     inline constexpr void operator++(int) noexcept(NO_EXCEPT) { ++*this; }

//     inline constexpr iterator operator++(int) noexcept(NO_EXCEPT)
//         requires std::ranges::forward_range<Base>
//     {
//         const auto res = *this; ++*this; return res;
//     }

//     inline constexpr iterator &operator--() noexcept(NO_EXCEPT)
//         requires std::ranges::bidirectional_range<Base>
//     {
//         std::ranges::advance(this->_current, this->_missing - this->_stride);
//         this->_missing = 0;
//         return *this;
//     }

//     inline constexpr iterator operator--(int) noexcept(NO_EXCEPT)
//         requires std::ranges::bidirectional_range<Base>
//     {
//         const auto res = *this; --*this; return res;
//     }

//     inline constexpr iterator &operator+=(const difference_type diff) noexcept(NO_EXCEPT)
//         requires std::ranges::random_access_range<Base>
//     {
//         if(diff > 0) {
//             assert(std::ranges::distance(this->_current, this->_end) > this->_stride * (diff - 1));
//             this->_missing = std::ranges::advance(this->_current, this->_stride * diff, this->_end);
//         } else if(diff < 0) {
//             std::ranges::advance(this->_current, this->_stride * diff + this->_missing);
//             this->_missing = 0;
//         }
//         return *this;
//     }

//     inline constexpr iterator &operator-=(const difference_type diff) noexcept(NO_EXCEPT)
//         requires std::ranges::random_access_range<Base>
//     {
//         return *this += -diff;
//     }

//     inline constexpr decltype(auto) operator[](const difference_type diff) const noexcept(NO_EXCEPT)
//         requires std::ranges::random_access_range<Base>
//     {
//         return *(*this + diff);
//     }

//     friend inline constexpr bool operator==(const iterator &lhs, std::default_sentinel_t) noexcept(NO_EXCEPT)
//     {
//         return lhs._current == lhs._end;
//     }

//     friend inline constexpr bool operator==(const iterator &lhs, const iterator &rhs) noexcept(NO_EXCEPT)
//         requires std::equality_comparable<std::ranges::iterator_t<Base>>
//     {
//         return lhs._current == rhs._current;
//     }

//     friend inline constexpr bool operator<(const iterator &lhs, const iterator &rhs) noexcept(NO_EXCEPT)
//         requires std::ranges::random_access_range<Base>
//     {
//         return lhs._current < rhs._current;
//     }

//     friend inline constexpr bool operator>(const iterator &lhs, const iterator &rhs) noexcept(NO_EXCEPT)
//         requires std::ranges::random_access_range<Base>
//     {
//         return rhs._current < lhs._current;
//     }

//     friend inline constexpr bool operator<=(const iterator &lhs, const iterator &rhs) noexcept(NO_EXCEPT)
//         requires std::ranges::random_access_range<Base>
//     {
//         return !(rhs._current < lhs._current);
//     }

//     friend inline constexpr bool operator>=(const iterator &lhs, const iterator &rhs) noexcept(NO_EXCEPT)
//         requires std::ranges::random_access_range<Base>
//     {
//         return !(lhs._current < rhs._current);
//     }

//     friend inline constexpr auto operator<=>(const iterator &lhs, const iterator &rhs) noexcept(NO_EXCEPT)
//         requires std::ranges::random_access_range<Base> && std::three_way_comparable<std::ranges::iterator_t<Base>>
//     {
//         return lhs._current <=> rhs._current;
//     }

//     friend inline constexpr iterator operator+(const iterator &itr, const difference_type diff) noexcept(NO_EXCEPT)
//         requires std::ranges::random_access_range<Base>
//     {
//         auto res = itr; res += diff; return res;
//     }

//     friend inline constexpr iterator operator+(const difference_type diff, const iterator &itr) noexcept(NO_EXCEPT)
//         requires std::ranges::random_access_range<Base>
//     {
//         return itr + diff;
//     }

//     friend inline constexpr iterator operator-(const iterator &itr, const difference_type diff) noexcept(NO_EXCEPT)
//         requires std::ranges::random_access_range<Base>
//     {
//         auto res = itr; res -= diff; return res;
//     }

//     friend inline constexpr const difference_type operator-(const iterator &lhs, const iterator &rhs) noexcept(NO_EXCEPT)
//         requires std::sized_sentinel_for<std::ranges::iterator_t<Base>, std::ranges::iterator_t<Base>>
//     {
//         const auto diff = lhs._current - rhs._current;
//         if constexpr(std::ranges::forward_range<Base>)
//             return (diff + lhs._missing - rhs._missing) / lhs._stride;
//         else if(diff < 0)
//             return -div_ceil(-diff, lhs._stride);
//         else
//             return div_ceil(diff, lhs._stride);
//     }

//     friend inline constexpr const difference_type operator-(std::default_sentinel_t rhs, const iterator &lhs) noexcept(NO_EXCEPT)
//         requires std::sized_sentinel_for<std::ranges::sentinel_t<Base>, std::ranges::iterator_t<Base>>
//     {
//         return div_ceil(lhs._end - lhs._current, lhs._stride);
//     }

//     friend inline constexpr const difference_type operator-(const iterator &lhs, std::default_sentinel_t rhs) noexcept(NO_EXCEPT)
//         requires std::sized_sentinel_for<std::ranges::sentinel_t<Base>, std::ranges::iterator_t<Base>>
//     {
//         return -(rhs - lhs);
//     }

//     friend inline constexpr std::ranges::range_rvalue_reference_t<Base> iter_move(const iterator &itr) noexcept(NO_EXCEPT)
//     {
//         return std::ranges::iter_move(itr._current);
//     }

//     friend inline constexpr void iter_swap(const iterator &lhs, const iterator &rhs) noexcept(NO_EXCEPT)
//         requires std::indirectly_swappable<std::ranges::iterator_t<Base>>
//     {
//         std::ranges::iter_swap(lhs._current, rhs._current);
//     }
// };

// namespace views {

// namespace internal {

// template<class Range, typename T>
// concept can_stride_view = requires { stride_view(std::declval<Range>(), std::declval<T>()); };

// }

// struct Stride : adapter::range_adapter<Stride> {
//     template<std::ranges::viewable_range Range, class T = std::ranges::range_difference_t<Range>>
//         requires internal::can_stride_view<Range, T>
//     inline constexpr auto operator() [[nodiscard]] (Range &&res, std::type_identity_t<T> diff) const
//     {
//         return stride_view(std::forward<Range>(res), diff);
//     }

//     using adapter::range_adapter<Stride>::operator();
//     inline static constexpr int arity = 2;
//     inline static constexpr bool has_simple_call_op = true;
// };

// inline constexpr Stride stride;


// } // namespace views

// } // namespace lib


// namespace std::ranges {


// template<class View>
// inline constexpr bool enable_borrowed_range<lib::stride_view<View>> = enable_borrowed_range<View>;


// } // namespace std::ranges
