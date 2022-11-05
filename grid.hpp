#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <iterator>
#include <initializer_list>
#include <utility>

#include "template.hpp"

#include "internal/develop/dev_assert.hpp"
#include "internal/develop/dev_env.hpp"
#include "internal/types.hpp"

namespace Internal {

namespace GridLib {

template<class T>
struct Interface {
    virtual void assign(const Size, const Size, const T&&) = 0;

    virtual void resize(const Size, const Size) = 0;

    virtual Size height() const = 0;
    virtual Size width() const = 0;

    virtual Size id(const Size, const Size) const = 0;

    virtual T& operator()(const Size, const Size) = 0;
    virtual const T& operator()(const Size, const Size) const = 0;
};

template<class T> struct ContainerBase : virtual Interface<T> {
  private:
    Size _h, _w;

  protected:
    inline void _validate_index(__attribute__ ((unused)) const Size i, __attribute__ ((unused)) const Size j) const {
        dev_assert(0 <= i and i < this->height());
        dev_assert(0 <= j and j < this->width());
    }

    inline Size _positivize_row_index(const Size x) const {
        return x < 0 ? this->height() + x : x;
    }
    inline Size _positivize_col_index(const Size x) const {
        return x < 0 ? this->width() + x : x;
    }

  public:
    ContainerBase() = default;
    ContainerBase(const Size _h, const Size _w) : _h(_h), _w(_w) {}

    virtual void resize(const Size h, const Size w) override {
        this->_h = h, this->_w = w;
    }

    inline Size height() const override { return this->_h; }
    inline Size width() const override { return this->_w; }

    inline Size id(const Size i, const Size j) const override {
        const Size _i = this->_positivize_row_index(i);
        const Size _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return _i * this->width() + _j;
    }
};

template<class T, class Row = std::vector<T>, class Base = std::vector<Row>>
struct Container : Base, ContainerBase<T>, virtual Interface<T> {

    Container(const Size n = 0) : Container(n, n) {}
    Container(const Size h, const Size w, const T &&val = T{}) : Base(h, Row(w, std::forward<const T>(val))), ContainerBase<T>(h, w) {}

    Container(const std::initializer_list<Row> init_list) : Base(init_list) {
        const Size rows = std::distance(ALL(init_list));
        const Size first_cols = init_list.begin()->size();

        if constexpr (DEV_ENV) { ITR(init_row, init_list) dev_assert((Size)init_row.size() == first_cols); }

        this->ContainerBase<T>::resize(rows, first_cols);
    }

    inline void assign(const Container &source) {
        this->resize(source.height(), source.width());
        this->Base::assign(ALL(source));
    }

    inline void assign(const Size h, const Size w, const T &&val = T{}) override {
        this->ContainerBase<T>::resize(h, w);
        this->Base::resize(h);
        ITRR(row, *this) row.assign(w, std::forward<const T>(val));
    }

    inline void resize(const Size h, const Size w) override {
        this->ContainerBase<T>::resize(h, w);
        this->Base::resize(h);
        ITRR(row, *this) row.resize(w);
    }

    inline T& operator()(const Size i, const Size j) override {
        const Size _i = this->_positivize_row_index(i);
        const Size _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }

    inline const T& operator()(const Size i, const Size j) const override {
        const Size _i = this->_positivize_row_index(i);
        const Size _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }
};

template<class T, class Base = std::vector<T>>
struct UnfoldedContainer : Base, ContainerBase<T>, virtual Interface<T> {

    UnfoldedContainer(Size n = 0) : UnfoldedContainer(n, n) {}
    UnfoldedContainer(const Size h, const Size w, const T &&val = T{}) : Base(h*w, std::forward<const T>(val)), ContainerBase<T>(h, w) {}

    UnfoldedContainer(std::initializer_list<std::initializer_list<T>> init_list) {
        const Size rows = std::distance(init_list.begin(), init_list.end());
        const Size first_cols = init_list.begin()->size();

        this->resize(rows, first_cols);

        for(auto index=0, itr=init_list.begin(), itr_end=init_list.end(); itr!=itr_end; ++itr) {
            dev_assert((Size)itr->size() == first_cols);
            for(auto v=itr->begin(), v_end=itr->end(); v!=v_end; ++v) (*this)[index++] = *v;
        }
    }

    inline void assign(const UnfoldedContainer &source) {
        this->resize(source.height(), source.width());
        this->Base::assign(ALL(source));
    }

    inline void assign(const Size h, const Size w, const T &&val = T{}) override {
        this->ContainerBase<T>::resize(h, w);
        this->Base::assign(h*w, std::forward<const T>(val));
    }

    inline void resize(const Size h, const Size w) override {
        this->ContainerBase<T>::resize(h, w);
        this->Base::resize(h*w);
    }

    inline T& operator()(const Size i, const Size j) override {
        const Size _i = this->_positivize_row_index(i);
        const Size _j = this->_positivize_col_index(j);
        return (*this)[this->id(_i, _j)];
    }

    inline const T& operator()(const Size i, const Size j) const override {
        const Size _i = this->_positivize_row_index(i);
        const Size _j = this->_positivize_col_index(j);
        return (*this)[this->id(_i, _j)];
    }
};

}  // namespace GridLib

template<class T, class Container> struct GridCore : Container, virtual GridLib::Interface<T> {
    using Container::Container;

    enum class InvertDirection { Vertical, Horizontal };
    enum class RotateDirection { CounterClockwise, Clockwise };

    template<class U = T>
    void inline read(std::istream *ist = &std::cin) {
        REP(i, this->height()) REP(j, this->width()) {
            U val; *ist >> val;
            (*this)(i, j) = val;
        }
    }

    template<InvertDirection DIRECT = InvertDirection::Vertical>
    inline GridCore& invert() {
        GridCore res(this->height(), this->width());
        REP(i, this->height()) REP(j, this->width()) {
            if constexpr (DIRECT == InvertDirection::Vertical) {
                res(i,j) = (*this)(this->height()-i-1,j);
            }
            else {
                res(i,j) = (*this)(i, this->width()-j-1);
            }
        }
        this->assign(res);
        return *this;
    }

    template<RotateDirection DIRECT = RotateDirection::Clockwise>
    inline GridCore& rotate(const Size k) {
        GridCore res = *this;
        REP(i, k) { res = res.rotate<DIRECT>(); }
        this->assign(res);
        return *this;
    }

    template<RotateDirection DIRECT = RotateDirection::Clockwise>
    inline GridCore& rotate() {
        GridCore res(this->width(), this->height());
        REP(i, this->width()) REP(j, this->height()) {
            if constexpr (DIRECT == RotateDirection::Clockwise) {
                res(i,j) = (*this)(this->height()-j-1,i);
            }
            else {
                res(i,j) = (*this)(j,this->width()-i-1);
            }
        }
        this->assign(res);
        return *this;
    }

    inline GridCore& transpose() {
        GridCore res(this->width(), this->height());
        REP(i, this->width()) REP(j, this->height()) {
            res(i,j) = (*this)(j,i);
        }
        this->assign(res);
        return *this;
    }
};

} // namespace Internal

template<class T, class Row = std::vector<T>, class Base = std::vector<Row>>
using Grid = Internal::GridCore<T,Internal::GridLib::Container<T,Row,Base>>;

template<class T, class Base = std::vector<T>>
using UnfoldedGrid = Internal::GridCore<T,Internal::GridLib::UnfoldedContainer<T,Base>>;
