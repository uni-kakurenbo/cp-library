#pragma once


#include <cassert>
#include <tuple>

#include "internal/dev_env.hpp"
#include "internal/iterator.hpp"
#include "internal/types.hpp"

#include "numeric/arithmetic.hpp"
#include "numeric/bit.hpp"


namespace uni {


template<std::unsigned_integral T>
struct subset_enumerator {
    using value_type = T;
    using size_type = internal::size_t;

  private:
    T _n = 0;

  protected:
    using iterator_interface = internal::forward_iterator<const value_type>;

  public:
    // Enumerate tuple of (q, l, r), which means (floor/ceil)(_n/k) == q (l <= k <= r).
    subset_enumerator(const T n) noexcept(NO_EXCEPT) : _n(n) { assert(n >= 0); }

    struct iterator;
    using const_iterator = iterator;

    inline iterator begin() noexcept(NO_EXCEPT) { return iterator(this->_n, this->_n); }
    inline iterator end() noexcept(NO_EXCEPT) { return iterator(this->_n, 0, true); }

    inline size_type size() noexcept(NO_EXCEPT) { return 1 << std::popcount(this->_n); }

    struct iterator : virtual iterator_interface {
        using value_type = subset_enumerator::value_type;
        using reference = value_type;

      protected:
        T _n = 0, _v = 0;
        bool _end = false;

      public:
        iterator() noexcept = default;
        iterator(const T n, const T v, const bool end = false) noexcept(NO_EXCEPT) : _n(n), _v(v), _end(end) {};


        friend inline bool operator==(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT) {
            if(rhs._end) return lhs._end;
            if(lhs._end) return false;

            return lhs._v == rhs._v;
        };

        friend inline auto operator<=>(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT) {
            if(rhs._end) {
                if(lhs._end) return std::partial_ordering::equivalent;
                return std::partial_ordering::less;
            }
            if(lhs._end) {
                return std::partial_ordering::greater;
            }

            return comapre_as_bitset(rhs._v, lhs._v);
        };


        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->_v; }

        inline iterator& operator++() noexcept(NO_EXCEPT) {
            if(this->_v == 0) {
                this->_end = true;
            }
            else {
                this->_v = (this->_v - 1) & this->_n;
            }

            return *this;
        }

        inline iterator operator++(int) noexcept(NO_EXCEPT) { const auto res = *this; ++res; return res; }
    };

};


} // namespace uni


namespace std::ranges {

template<class T>
inline constexpr bool enable_borrowed_range<uni::subset_enumerator<T>> = true;

} // namespace std::ranges
