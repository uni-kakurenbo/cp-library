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
    using iterator_interface = internal::bidirectional_iterator_interface<const value_type>;

  public:
    // Enumerate tuple of (q, l, r), which means (floor/ceil)(_n/k) == q (l <= k <= r).
    subset_enumerator(const T n) noexcept(NO_EXCEPT) : _n(n) { assert(n >= 0); }

    struct iterator;
    using const_iterator = iterator;

    inline auto begin() noexcept(NO_EXCEPT) { return iterator(this->_n, this->_n); }
    inline auto end() noexcept(NO_EXCEPT) { return iterator(this->_n, 0, true); }

    inline auto rbegin() noexcept(NO_EXCEPT) { return std::make_reverse_iterator(this->end()); }
    inline auto rend() noexcept(NO_EXCEPT) { return std::make_reverse_iterator(this->begin()); }

    inline auto size() noexcept(NO_EXCEPT) { return static_cast<size_type>(1) << std::popcount(this->_n); }

    struct iterator : iterator_interface {
      protected:
        value_type _n = 0, _v = 0;
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


        inline auto operator*() const noexcept(NO_EXCEPT) { return this->_v; }

        inline auto& operator++() noexcept(NO_EXCEPT) {
            if(this->_v == 0) {
                this->_end = true;
            }
            else {
                this->_v = (this->_v - 1) & this->_n;
            }

            return *this;
        }

        inline auto& operator--() noexcept(NO_EXCEPT) {
            if(this->_end) {
                this->_end = false;
            }
            else {
                const auto lsb = lowest_bit_pos(this->_n & ~this->_v);
                this->_v = ((this->_v >> lsb) | 1) << lsb;
            }

            return *this;
        }

        inline auto operator++(int) noexcept(NO_EXCEPT) { const auto res = *this; ++res; return res; }
        inline auto operator--(int) noexcept(NO_EXCEPT) { const auto res = *this; --res; return res; }
    };

};


} // namespace uni


namespace std::ranges {

template<class T>
inline constexpr bool enable_borrowed_range<uni::subset_enumerator<T>> = true;

} // namespace std::ranges
