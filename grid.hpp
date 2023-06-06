#pragma once

#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <iterator>
#include <initializer_list>
#include <utility>

#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

namespace lib {

namespace internal {

namespace grid_impl {

template<class T>
struct interface {
    // virtual void assign(const size_t, const size_t, const T&) noexcept(NO_EXCEPT) = 0;

    // virtual void resize(const size_t, const size_t) noexcept(NO_EXCEPT) = 0;

    // virtual size_t height() const noexcept(NO_EXCEPT) = 0;
    // virtual size_t width() const noexcept(NO_EXCEPT) = 0;

    // virtual size_t id(const size_t, const size_t) const noexcept(NO_EXCEPT) = 0;

    // virtual T& operator()(const size_t, const size_t) noexcept(NO_EXCEPT) = 0;
    // virtual const T& operator()(const size_t, const size_t) const noexcept(NO_EXCEPT) = 0;
};

template<class T> struct container_base : virtual interface<T> {
  private:
    size_t _h, _w;

  protected:
    inline void _validate_index(__attribute__ ((unused)) const size_t i, __attribute__ ((unused)) const size_t j) const noexcept(NO_EXCEPT) {
        assert(0 <= i and i < this->height());
        assert(0 <= j and j < this->width());
    }

    inline size_t _positivize_row_index(const size_t x) const noexcept(NO_EXCEPT) {
        return x < 0 ? this->height() + x : x;
    }
    inline size_t _positivize_col_index(const size_t x) const noexcept(NO_EXCEPT) {
        return x < 0 ? this->width() + x : x;
    }

  public:
    container_base() noexcept(NO_EXCEPT) = default;
    container_base(const size_t h, const size_t w) noexcept(NO_EXCEPT) : _h(h), _w(w) {}

    virtual void resize(const size_t h, const size_t w) noexcept(NO_EXCEPT) /*override*/ {
        this->_h = h, this->_w = w;
    }

    inline size_t height() const noexcept(NO_EXCEPT) /*override*/ { return this->_h; }
    inline size_t width() const noexcept(NO_EXCEPT) /*override*/ { return this->_w; }

    inline size_t id(const size_t i, const size_t j) const noexcept(NO_EXCEPT) /*override*/ {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return _i * this->width() + _j;
    }
};

template<class T, class Row = std::vector<T>, class base = std::vector<Row>>
struct container : base, container_base<T>, virtual interface<T> {

    container(const size_t n = 0) noexcept(NO_EXCEPT) : container(n, n) {}
    container(const size_t h, const size_t w, const T &val = T{}) noexcept(NO_EXCEPT) : base(h, Row(w, val)), container_base<T>(h, w) {}

    container(const std::initializer_list<Row> init_list) noexcept(NO_EXCEPT) : base(init_list) {
        const size_t rows = std::distance(ALL(init_list));
        const size_t first_cols = init_list.begin()->size();

        if constexpr (DEV_ENV) { ITR(init_row, init_list) assert((size_t)init_row.size() == first_cols); }

        this->container_base<T>::resize(rows, first_cols);
    }

    inline void assign(const container &source) noexcept(NO_EXCEPT) {
        this->resize(source.height(), source.width());
        this->base::assign(ALL(source));
    }

    inline void assign(const size_t h, const size_t w, const T &val = T{}) noexcept(NO_EXCEPT) /*override*/ {
        this->container_base<T>::resize(h, w);
        this->base::resize(h);
        ITRR(row, *this) row.assign(w, val);
    }

    inline void resize(const size_t h, const size_t w) noexcept(NO_EXCEPT) /*override*/ {
        this->container_base<T>::resize(h, w);
        this->base::resize(h);
        ITRR(row, *this) row.resize(w);
    }

    inline T& operator()(const size_t i, const size_t j) noexcept(NO_EXCEPT) /*override*/ {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }

    inline const T& operator()(const size_t i, const size_t j) const noexcept(NO_EXCEPT) /*override*/ {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }
};

template<class T, class base = std::vector<T>>
struct unfolded_container : base, container_base<T>, virtual interface<T> {

    unfolded_container(size_t n = 0) noexcept(NO_EXCEPT) : unfolded_container(n, n) {}
    unfolded_container(const size_t h, const size_t w, const T &val = T{}) noexcept(NO_EXCEPT) : base(h*w, val), container_base<T>(h, w) {}

    unfolded_container(std::initializer_list<std::initializer_list<T>> init_list) noexcept(NO_EXCEPT) {
        const size_t rows = std::distance(init_list.begin(), init_list.end());
        const size_t first_cols = init_list.begin()->size();

        this->resize(rows, first_cols);

        for(auto index=0, itr=init_list.begin(), itr_end=init_list.end(); itr!=itr_end; ++itr) {
            assert((size_t)itr->size() == first_cols);
            for(auto v=itr->begin(), v_end=itr->end(); v!=v_end; ++v) (*this)[index++] = *v;
        }
    }

    inline void assign(const unfolded_container &source) noexcept(NO_EXCEPT) {
        this->resize(source.height(), source.width());
        this->base::assign(ALL(source));
    }

    inline void assign(const size_t h, const size_t w, const T &val = T{}) noexcept(NO_EXCEPT) /*override*/ {
        this->container_base<T>::resize(h, w);
        this->base::assign(h*w, val);
    }

    inline void resize(const size_t h, const size_t w) noexcept(NO_EXCEPT) /*override*/ {
        this->container_base<T>::resize(h, w);
        this->base::resize(h*w);
    }

    inline T& operator()(const size_t i, const size_t j) noexcept(NO_EXCEPT) /*override*/ {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        return (*this)[this->id(_i, _j)];
    }

    inline const T& operator()(const size_t i, const size_t j) const noexcept(NO_EXCEPT) /*override*/ {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        return (*this)[this->id(_i, _j)];
    }
};

}  // namespace grid_impl

template<class T, class container> struct grid_core : container, virtual grid_impl::interface<T> {
    using container::container;

    enum class invert_direction { vertical, horizontal };
    enum class rotate_direction { counter_clockwise, clockwise };

    template<class U = T, class Stream = std::istream>
    void inline read(Stream *const ist = &std::cin) noexcept(NO_EXCEPT) {
        REP(i, this->height()) REP(j, this->width()) {
            U val; *ist >> val;
            (*this)(i, j) = val;
        }
    }

    template<invert_direction DIRECTION = invert_direction::vertical>
    inline grid_core& invert() noexcept(NO_EXCEPT) {
        grid_core res(this->height(), this->width());
        REP(i, this->height()) REP(j, this->width()) {
            if constexpr (DIRECTION == invert_direction::vertical) {
                res(i,j) = (*this)(this->height()-i-1,j);
            }
            else {
                res(i,j) = (*this)(i, this->width()-j-1);
            }
        }
        this->assign(res);
        return *this;
    }

    template<rotate_direction DIRECTION = rotate_direction::clockwise>
    inline grid_core& rotate(const size_t k) noexcept(NO_EXCEPT) {
        grid_core res = *this;
        REP(i, k) { res = res.rotate<DIRECTION>(); }
        this->assign(res);
        return *this;
    }

    template<rotate_direction DIRECTION = rotate_direction::clockwise>
    inline grid_core& rotate() noexcept(NO_EXCEPT) {
        grid_core res(this->width(), this->height());
        REP(i, this->width()) REP(j, this->height()) {
            if constexpr (DIRECTION == rotate_direction::clockwise) {
                res(i,j) = (*this)(this->height()-j-1,i);
            }
            else {
                res(i,j) = (*this)(j,this->width()-i-1);
            }
        }
        this->assign(res);
        return *this;
    }

    inline grid_core& transpose() noexcept(NO_EXCEPT) {
        grid_core res(this->width(), this->height());
        REP(i, this->width()) REP(j, this->height()) {
            res(i,j) = (*this)(j,i);
        }
        this->assign(res);
        return *this;
    }
};

} // namespace internal

template<class T, class Row = std::vector<T>, class base = std::vector<Row>>
using grid = internal::grid_core<T,internal::grid_impl::container<T,Row,base>>;

template<class T, class base = std::vector<T>>
using unfolded_grid = internal::grid_core<T,internal::grid_impl::unfolded_container<T,base>>;

} // namespace lib
