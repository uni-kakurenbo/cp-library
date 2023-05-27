#pragma once


#include <cassert>
#include <tuple>

#include "internal/dev_env.hpp"
#include "internal/iterator.hpp"
#include "internal/types.hpp"

#include "numeric/arithmetic.hpp"


namespace lib {

template<class T, bool CEIL = false>
struct quotient_enumerator {
    using value_type = std::tuple<T,T,T>; // (q, l, r)
    using size_type = T;

    T n = 0;

  private:
    T _n = 0;
    size_type _size = -1;

  protected:
    using iterator_interface = internal::bidirectional_iterator_interface<const value_type>;

  public:
    // Enumerate tuple of (q, l, r), which means (floor/ceil)(n/k) == q (l <= k <= r).
    quotient_enumerator(const T n) noexcept(DEV_ENV) : n(n), _n(n - CEIL) { assert(n > 0); }

    struct iterator;
    using const_iterator = iterator;

    inline iterator begin() const noexcept(DEV_ENV) { return iterator(this->_n, 1); }
    inline iterator end() const noexcept(DEV_ENV) { return iterator(this->_n, this->n + 1); }

    inline auto rbegin() noexcept(DEV_ENV) { return std::make_reverse_iterator(this->end()); }
    inline auto rend() noexcept(DEV_ENV) { return std::make_reverse_iterator(this->begin()); }

    inline size_type size() noexcept(DEV_ENV) {
        if(this->_size < 0) {
            size_type r = lib::sqrt_floor(this->_n);
            this->_size = 2 * r - (this->_n < r * (r + 1)) + CEIL;
        }
        return this->_size;
    }

    struct iterator : virtual iterator_interface {
        using reference = value_type;

      protected:
        T _n = 0;
        T _q = 0, _l = 0, _r = 0;

        void _set_l(const T l) noexcept(DEV_ENV) {
            this->_l = l, this->_q = this->_n / l;
            if(this->_q == 0) {
                if(CEIL) {
                    if(l == this->_n + 1) this->_r = l;
                }
                return;
            }
            this->_r = this->_n / this->_q;
        }
        void _set_r(const T r) noexcept(DEV_ENV) {
            this->_r = r, this->_q = this->_n / r;
            this->_l = this->_n / (this->_q + 1) + 1;
        }

      public:
        iterator() noexcept(DEV_ENV) {}
        iterator(const T n, const T l) noexcept(DEV_ENV) : _n(n) { this->_set_l(l); }


        friend inline bool operator==(const iterator& lhs, const iterator& rhs) noexcept(DEV_ENV) { return lhs._l == rhs._l; };
        friend inline bool operator!=(const iterator& lhs, const iterator& rhs) noexcept(DEV_ENV) { return lhs._l != rhs._l; };

        inline value_type operator*() const noexcept(DEV_ENV) { return { this->_q + CEIL, this->_l, this->_r }; }

        inline iterator& operator++() noexcept(DEV_ENV) { this->_set_l(this->_r + 1); return *this; }
        inline iterator operator++(int) noexcept(DEV_ENV) { const auto res = *this; this->_set_l(this->_r + 1); return res; }

        inline iterator& operator--() noexcept(DEV_ENV) { this->_set_r(this->_l - 1); return *this; }
        inline iterator operator--(int) noexcept(DEV_ENV) { const auto res = *this; this->_set_r(this->_l - 1); return res; }
    };

};


} // namespace lib
