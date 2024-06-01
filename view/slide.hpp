#pragma once


#include <concepts>
#include <ranges>


#include "internal/ranges.hpp"

#include "numeric/arithmetic.hpp"


namespace uni {


namespace internal {


template<class View>
concept slide_caches_nothing = std::ranges::random_access_range<View> && std::ranges::sized_range<View>;

template<class View>
concept slide_caches_last = !slide_caches_nothing<View> && std::ranges::bidirectional_range<View> && std::ranges::common_range<View>;


template<class View>
concept slide_caches_first = !slide_caches_nothing<View> && !slide_caches_last<View>;

} // namespace internal

template<std::ranges::forward_range View>
    requires std::ranges::view<View>
struct slide_view : std::ranges::view_interface<slide_view<View>> {
  private:
    View _base;
    std::ranges::range_difference_t<View> _n;

    [[no_unique_address]] internal::maybe_present_t<internal::slide_caches_first<View>, internal::cached_position<View>, 0> _cache_begin;
    [[no_unique_address]] internal::maybe_present_t<internal::slide_caches_last<View>, internal::cached_position<View>, 1> _cache_end;

    template<bool> struct iterator;
    struct sentinel;

  public:
    constexpr explicit slide_view(View base, std::ranges::range_difference_t<View> n)
      : _base(std::move(base)), _n(n)
    {
        assert(n > 0);
    }

    constexpr auto begin()
        requires(!(internal::simple_view<View> && internal::slide_caches_nothing<const View>))
    {
        if constexpr(internal::slide_caches_first<View>) {
            std::ranges::iterator_t<View> itr;

            if(this->_cache_begin.has_value()) {
                itr = this->_cache_begin.get(this->_base);
            }
            else {
                itr = std::ranges::next(std::ranges::begin(this->_base), this->_n - 1, std::ranges::end(this->_base));
                this->_cache_begin.set(this->_base, itr);
            }

            return iterator<false>(std::ranges::begin(this->_base), std::move(itr), this->_n);
        }
        else {
            return iterator<false>(std::ranges::begin(this->_base), this->_n);
        }
    }

    constexpr auto begin() const
        requires internal::slide_caches_nothing<const View>
    {
        return iterator<true>(std::ranges::begin(this->_base), this->_n);
    }

    constexpr auto end()
        requires (!(internal::simple_view<View> && internal::slide_caches_nothing<const View>))
    {
        if constexpr(internal::slide_caches_nothing<View>) {
            return iterator<false>(std::ranges::begin(this->_base) + std::ranges::range_difference_t<View>(this->size()), this->_n);
        }
        else if constexpr(internal::slide_caches_last<View>) {
            std::ranges::iterator_t<View> itr;

            if(this->_cache_end.has_value()) {
                itr = this->_cache_end.get(this->_base);
            }
            else {
                itr = std::ranges::prev(std::ranges::end(this->_base), this->_n - 1, std::ranges::begin(this->_base));
                this->_cache_end.set(this->_base, itr);
            }

            return iterator<false>(std::move(itr), this->_n);
        }
        else if constexpr(std::ranges::common_range<View>) {
            return iterator<false>(std::ranges::end(this->_base), std::ranges::end(this->_base), this->_n);
        }
        else {
            return sentinel(std::ranges::end(this->_base));
        }
    }

    constexpr auto end() const
        requires internal::slide_caches_nothing<const View>
    {
        return this->begin() + std::ranges::range_difference_t<const View>(this->size());
    }

    constexpr auto size()
        requires std::ranges::sized_range<View>
    {
        auto size = std::ranges::distance(this->_base) - this->_n + 1;
        if(size < 0) size = 0;

        return to_unsigned(size);
    }

    constexpr auto size() const
        requires std::ranges::sized_range<const View>
    {
        auto size = std::ranges::distance(this->_base) - this->_n + 1;
        if(size < 0) size = 0;

        return to_unsigned(size);
    }
};

template<class Range>
slide_view(Range &&, std::ranges::range_difference_t<Range>) -> slide_view<std::views::all_t<Range>>;


template<std::ranges::forward_range View>
    requires std::ranges::view<View>
template<bool CONST>
struct slide_view<View>::iterator {
  private:
    using Base = internal::maybe_const_t<CONST, View>;

    static constexpr bool LAST_PRESENT = internal::slide_caches_first<Base>;

    std::ranges::iterator_t<Base> _current = std::ranges::iterator_t<Base>();

    [[no_unique_address]] internal::maybe_present_t<LAST_PRESENT, std::ranges::iterator_t<Base>> _last = decltype(_last)();
    std::ranges::range_difference_t<Base> _n = 0;

    constexpr iterator(std::ranges::iterator_t<Base> current, std::ranges::range_difference_t<Base> n)
        requires (!LAST_PRESENT)
      : _current(current), _n(n)
    {}

    constexpr iterator(std::ranges::iterator_t<Base> current, std::ranges::iterator_t<Base> last, std::ranges::range_difference_t<Base> n)
        requires (LAST_PRESENT)
        : _current(current), _last(last), _n(n)
    {}


    friend slide_view;
    friend slide_view::sentinel;

  public:
    using iterator_category = std::input_iterator_tag;
    using iterator_concept = internal::most_primitive_iterator_concept<false, Base>;

    using value_type = decltype(std::views::counted(_current, _n));

    using difference_type = std::ranges::range_difference_t<Base>;

    iterator() = default;

    constexpr iterator(iterator<!CONST> itr)
        requires CONST && std::convertible_to<std::ranges::iterator_t<View>, std::ranges::iterator_t<Base>>
      : _current(std::move(itr._current)), _n(itr._n)
    {}

    constexpr auto operator*() const {
        return std::views::counted(_current, _n);
    }

    constexpr iterator &operator++() {
        ++this->_current;
        if constexpr(LAST_PRESENT) ++this->_last;
        return *this;
    }

    constexpr iterator operator++(int) {
        const auto temp = *this;
        return ++*this, temp;
    }

    constexpr iterator &operator--()
        requires std::ranges::bidirectional_range<Base>
    {
        --this->_current;
        if constexpr(LAST_PRESENT) --this->_last;
        return *this;
    }

    constexpr iterator operator--(int)
        requires std::ranges::bidirectional_range<Base>
    {
        auto temp = *this;
        return --*this, temp;
    }

    constexpr iterator &operator+=(const difference_type rhs)
        requires std::ranges::random_access_range<Base>
    {
        this->_current += rhs;
        if constexpr(LAST_PRESENT) this->_last += rhs;
        return *this;
    }

    constexpr iterator &operator-=(const difference_type rhs)
        requires std::ranges::random_access_range<Base>
    {
        this->_current -= rhs;
        if constexpr(LAST_PRESENT) this->_last -= rhs;
        return *this;
    }

    constexpr auto operator[](const difference_type count) const
        requires std::ranges::random_access_range<Base>
    {
        return std::views::counted(this->_current + count, this->_n);
    }

    friend constexpr bool operator==(const iterator &lhs, const iterator &rhs) {
        if constexpr(LAST_PRESENT) return lhs._last == rhs._last;
        else return lhs._current == rhs._current;
    }

    friend constexpr auto operator<=>(const iterator &lhs, const iterator &rhs)
        requires std::ranges::random_access_range<Base> && std::three_way_comparable<std::ranges::iterator_t<Base>>
    {
        return lhs._current <=> rhs._current;
    }

    friend constexpr iterator operator+(iterator itr, const difference_type count)
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

    friend constexpr iterator operator-(const iterator &itr, difference_type count)
        requires std::ranges::random_access_range<Base>
    {
        auto res = itr;
        return res -= count, res;
    }

    friend constexpr difference_type operator-(const iterator &lhs, const iterator &rhs)
        requires std::sized_sentinel_for<std::ranges::iterator_t<Base>, std::ranges::iterator_t<Base>>
    {
        if constexpr(LAST_PRESENT) return lhs._last - rhs._last;
        else return lhs._current - rhs._current;
    }
};


template<std::ranges::forward_range View>
    requires std::ranges::view<View>
struct slide_view<View>::sentinel {
  private:
    std::ranges::sentinel_t<View> _end = std::ranges::sentinel_t<View>();

    constexpr explicit sentinel(std::ranges::sentinel_t<View> end) : _end(end) {}

    friend slide_view;

  public:
    sentinel() = default;

    friend constexpr bool operator==(const iterator<false> &lhs, const sentinel &rhs) {
        return lhs._last == rhs._end;
    }

    friend constexpr std::ranges::range_difference_t<View>
    operator-(const iterator<false> &lhs, const sentinel &rhs)
        requires std::sized_sentinel_for<std::ranges::sentinel_t<View>, std::ranges::iterator_t<View>>
    {
        return lhs._last - rhs._end;
    }

    friend constexpr std::ranges::range_difference_t<View>
    operator-(const sentinel &rhs, const iterator<false> &lhs)
        requires std::sized_sentinel_for<std::ranges::sentinel_t<View>, std::ranges::iterator_t<View>>
    {
        return rhs._end - lhs._last;
    }
};


namespace views {


namespace internal {


template<class Range, class Diff>
concept can_slide_view = requires { slide_view(std::declval<Range>(), std::declval<Diff>()); };


} // namespace internal



struct Slide : adaptor::range_adaptor<Slide> {
    template<std::ranges::viewable_range Range, class Diff = std::ranges::range_difference_t<Range>>
        requires internal::can_slide_view<Range, Diff>

    constexpr auto operator() [[nodiscard]] (Range &&res, std::type_identity_t<Diff> n) const {
        return slide_view(std::forward<Range>(res), n);
    }

    using adaptor::range_adaptor<Slide>::operator();

    static constexpr int arity = 2;
    static constexpr bool has_simple_extra_args = true;
};


inline constexpr Slide slide;


} // namespace views

} // namespace uni


namespace std::ranges {


template<class View>
inline constexpr bool enable_borrowed_range<uni::slide_view<View>> = enable_borrowed_range<View>;


}
