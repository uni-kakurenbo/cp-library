#pragma once


#include <cassert>
#include <tuple>

#include "internal/dev_env.hpp"
#include "internal/iterator.hpp"
#include "internal/types.hpp"

#include "numeric/arithmetic.hpp"


namespace uni {


template<class T, bool CEIL = false>
struct quotient_enumerator {
    using value_type = std::tuple<T,T,T>; // (q, l, r)
    using size_type = T;

  private:
    T _n = 0, _n_impl = 0;
    size_type _size = -1;

  protected:
    using iterator_interface = internal::bidirectional_iterator_interface<const value_type>;

  public:
    // Enumerate tuple of (q, l, r), which means (floor/ceil)(_n/k) == q (l <= k <= r).
    quotient_enumerator(const T n) noexcept(NO_EXCEPT) : _n(n), _n_impl(n - CEIL) { assert(n > 0); }

    struct iterator;
    using const_iterator = iterator;

    inline auto begin() noexcept(NO_EXCEPT) { return iterator(this->_n_impl, 1); }
    inline auto end() noexcept(NO_EXCEPT) { return iterator(this->_n_impl, this->_n + 1); }

    inline auto rbegin() noexcept(NO_EXCEPT) { return std::make_reverse_iterator(this->end()); }
    inline auto rend() noexcept(NO_EXCEPT) { return std::make_reverse_iterator(this->begin()); }

    inline auto size() noexcept(NO_EXCEPT) {
        if(this->_size < 0) {
            size_type r = uni::sqrt_floor(this->_n_impl);
            this->_size = 2 * r - (this->_n_impl < r * (r + 1)) + CEIL;
        }
        return this->_size;
    }

    struct iterator : virtual iterator_interface {
        using value_type = quotient_enumerator::value_type;
        using reference = value_type;

      protected:
        T _n_impl = 0;
        T _q = 0, _l = 0, _r = 0;

        void _set_l(const T l) noexcept(NO_EXCEPT) {
            this->_l = l, this->_q = this->_n_impl / l;
            if(this->_q == 0) {
                if(CEIL) {
                    if(l == this->_n_impl + 1) this->_r = l;
                }
                return;
            }
            this->_r = this->_n_impl / this->_q;
        }
        void _set_r(const T r) noexcept(NO_EXCEPT) {
            this->_r = r, this->_q = this->_n_impl / r;
            this->_l = this->_n_impl / (this->_q + 1) + 1;
        }

      public:
        iterator() noexcept = default;
        iterator(const T n, const T l) noexcept(NO_EXCEPT) : _n_impl(n) { this->_set_l(l); }

        friend inline bool operator==(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT) { return lhs._l == rhs._l; }

        inline value_type operator*() const noexcept(NO_EXCEPT) { return { this->_q + CEIL, this->_l, this->_r }; }

        inline auto& operator++() noexcept(NO_EXCEPT) { this->_set_l(this->_r + 1); return *this; }
        inline auto& operator--() noexcept(NO_EXCEPT) { this->_set_r(this->_l - 1); return *this; }

        inline auto operator++(int) noexcept(NO_EXCEPT) { const auto res = *this; this->_set_l(this->_r + 1); return res; }
        inline auto operator--(int) noexcept(NO_EXCEPT) { const auto res = *this; this->_set_r(this->_l - 1); return res; }
    };

};


} // namespace uni


namespace std::ranges {

template<class T>
inline constexpr bool enable_borrowed_range<uni::quotient_enumerator<T>> = true;

} // namespace std::ranges
