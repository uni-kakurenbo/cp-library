#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>

#include "template.hpp"

#include "develop/dev_assert.hpp"

#include "graph.hpp"


namespace Internal {

namespace GridLib {

template<class T>
struct Interface {
    inline void _validate_index(__attribute__ ((unused)) const int i, __attribute__ ((unused)) const int j) const {
        dev_assert(0 <= i and i < this->height());
        dev_assert(0 <= j and j < this->width());
    }

    virtual void assign(const int, const int, const T&&) = 0;

    virtual void resize(const int, const int) = 0;

    virtual int height() const = 0;
    virtual int width() const = 0;

    virtual int id(const int, const int) const = 0;

    virtual T& operator()(const int, const int) = 0;
    virtual const T& operator()(const int, const int) const = 0;
};

template<class T> struct ContainerBase : virtual Interface<T> {
  private:
    int _h, _w;

  protected:
    inline int _positivize_row_index(const int x) const {
        return x < 0 ? this->height() + x : x;
    }
    inline int _positivize_col_index(const int x) const {
        return x < 0 ? this->width() + x : x;
    }

  public:
    ContainerBase(const int _h, const int _w) : _h(_h), _w(_w) {}

    virtual void resize(const int h, const int w) override {
        this->_h = h, this->_w = w;
    }

    inline int height() const override { return this->_h; }
    inline int width() const override { return this->_w; }

    inline int id(const int i, const int j) const override {
        const int _i = this->_positivize_row_index(i);
        const int _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return _i * this->width() + _j;
    }
};

template<class T, class Row = std::vector<T>>
struct Container : std::vector<Row>, ContainerBase<T>, virtual Interface<T> {
    using std::vector<Row>::vector;

    Container(int n = 0) : Container(n, n) {}
    Container(int h, int w, T val = T{}) : std::vector<Row>(h, Row(w, val)), ContainerBase<T>(h, w) {}

    template<class G> Container(const G &source) : Container(source.height(), source.width()) {
        this->assign(source);
    }

    template<class G>
    inline void assign(const G &source) {
        this->resize(source.height(), source.width());
        this->std::vector<Row>::assign(source.begin(), source.end());
    }

    inline void assign(const int h, const int w, const T &&val = T{}) override {
        this->ContainerBase<T>::resize(h, w);
        this->std::vector<Row>::resize(h);
        ITRR(row, *this) row.assign(w, val);
    }

    inline void resize(const int h, const int w) override {
        this->ContainerBase<T>::resize(h, w);
        this->std::vector<Row>::resize(h);
        ITRR(row, *this) row.resize(w);
    }

    inline T& operator()(const int i, const int j) override {
        const int _i = this->_positivize_row_index(i);
        const int _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }

    inline const T& operator()(const int i, const int j) const override {
        const int _i = this->_positivize_row_index(i);
        const int _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }
};

template<class T>
struct UnfoldedContainer : std::vector<T>, ContainerBase<T>, virtual Interface<T> {
    using std::vector<T>::vector;

    UnfoldedContainer(int n = 0) : UnfoldedContainer(n, n) {}
    UnfoldedContainer(int h, int w, T val = T{}) : std::vector<T>(h*w, val), ContainerBase<T>(h, w) {}

    template<class G> UnfoldedContainer(const G &source) { this->assign(source); }

    template<class G>
    inline void assign(const G &source) {
        this->resize(source.height(), source.width());
        this->std::vector<T>::assign(source.begin(), source.end());
    }

    inline void assign(const int h, const int w, const T &&val = T{}) override {
        this->ContainerBase<T>::resize(h, w);
        this->std::vector<T>::assign(h*w, val);
    }

    inline void resize(const int h, const int w) override {
        this->ContainerBase<T>::resize(h, w);
        this->std::vector<T>::resize(h*w);
    }

    inline T& operator()(const int i, const int j) override {
        const int _i = this->_positivize_row_index(i);
        const int _j = this->_positivize_col_index(j);
        return (*this)[this->id(_i, _j)];
    }

    inline const T& operator()(const int i, const int j) const override {
        const int _i = this->_positivize_row_index(i);
        const int _j = this->_positivize_col_index(j);
        return (*this)[this->id(_i, _j)];
    }
};

}  // namespace Grid

template<class T, class Container> struct GridCore : Container, virtual GridLib::Interface<T> {
    using Container::Container;

    enum class Direction { Vertical, Horizontal, CCW, CW };

  template<class U = T>
    void inline read(std::istream *ist = &std::cin) {
        REP(i, this->height()) REP(j, this->width()) {
            U val; *ist >> val;
            (*this)(i, j) = val;
        }
    }

    inline GridCore rotated(const int k) const {  // TODO: 方向
        GridCore res = *this;
        REP(i, k) res = res.rotated();
        return res;
    }

    inline GridCore rotated() const {  // TODO: 方向
        GridCore res = this->inverted();
        return res.transposed();
    }

    template<Direction DIRECT = Direction::Vertical>
    inline GridCore inverted() const {
        GridCore res(this->height(), this->width());
        REP(i, this->height()) REP(j, this->width()) {
            if constexpr (DIRECT == Direction::Vertical) res(i,j) = (*this)(this->height()-i-1,j);
            else if (DIRECT == Direction::Horizontal) res(i,j) = (*this)(i, this->height()-j-1);
            // TODO: exception
        }
        return res;
    }

    inline GridCore transposed() const {
        GridCore res(this->width(), this->height());
        REP(i, this->width()) REP(j, this->height()) {
            res(i,j) = (*this)(j,i);
        }
        return res;
    }
};

} // namespace Internal


template<class T, class Row = std::vector<T>>
using Grid = Internal::GridCore<T,Internal::GridLib::Container<T,Row>>;

template<class T>
using UnfoldedGrid = Internal::GridCore<T,Internal::GridLib::UnfoldedContainer<T>>;
