#pragma once

#include <cassert>
#include <tuple>

#include "internal/iterator.hpp"
#include "internal/types.hpp"


namespace lib {


template <class T>
struct quotient_enumerator {
    using value_type = std::tuple<T,T,T>; // (q, (l, r))
    using size_type = internal::size_t;

  private:
    T _n = 0;
    size_type _size = -1;

  protected:
    using iterator_interface = internal::bidirectional_iterator_interface<const value_type>;

  public:
    quotient_enumerator(const T n) : _n(n) { assert(n > 0); }

    struct iterator;
    using const_iterator = iterator;

    inline iterator begin() const { return iterator(_n, 1); }
    inline iterator end() const { return iterator(_n, _n+1); }

    inline auto rbegin() { return std::make_reverse_iterator(this->end()); }
    inline auto rend() { return std::make_reverse_iterator(this->begin()); }

    inline size_type size() {
        if(_size >= 0) return _size;
        _size = 0;
        for(T l = 1; l <= _n; l = _n / (_n / l) + 1) _size++;
        return _size;
    }

    struct iterator : virtual iterator_interface {
        using reference = value_type;

      protected:
        T _n = 0;
        T _q = 0, _l = 0, _r = 0;

        void _set_l(const T l) {
            _l = l, _q = _n / l;
            if(_q != 0) _r = _n / _q ;
        }
        void _set_r(const T r) {
            _r = r, _q = _n / r;
            _l = _n / (_q + 1) + 1;
        }

      public:
        iterator() {}
        iterator(const T n, const T l) : _n(n) { this->_set_l(l); }


        friend inline bool operator==(const iterator& lhs, const iterator& rhs) { return lhs._l == rhs._l; };
        friend inline bool operator!=(const iterator& lhs, const iterator& rhs) { return lhs._l != rhs._l; };

        inline value_type operator*() const { return { _q, _l, _r }; }

        inline iterator& operator++(int) { this->_set_l(this->_r + 1); return *this; }
        inline iterator operator++() { const auto res = *this; this->_set_l(this->_r + 1); return res; }

        inline iterator& operator--(int) { this->_set_r(this->_l - 1); return *this; }
        inline iterator operator--() { const auto res = *this; this->_set_r(this->_l - 1); return res; }
    };

};


} // namespace lib
