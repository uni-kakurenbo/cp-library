#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>

#include "template.hpp"

#include "develop/dev_assert.hpp"

#include "graph.hpp"


namespace Internal {

template<class T>
struct IGrid {
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

template<class T> struct BasicGrid : virtual IGrid<T> {
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
    BasicGrid(const int _h, const int _w) : _h(_h), _w(_w) {}

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

template<class> struct GridCommonMixin;

} // namespace Internal

template<class T, class Row = std::vector<T>>
struct Grid : std::vector<Row>, Internal::BasicGrid<T>, Internal::GridCommonMixin<T>, virtual Internal::IGrid<T> {
    using std::vector<Row>::vector;

    Grid(int n = 0) : Grid(n, n) {}
    Grid(int h, int w, T val = T{}) : std::vector<Row>(h, Row(w, val)), Internal::BasicGrid<T>(h, w) {}

    inline void assign(const Grid &source) {
        this->resize(source.height(), source.width());
        this->std::vector<Row>::assign(ALL(source));
    }

    inline void assign(const int h, const int w, const T &&val = T{}) override {
        this->Internal::BasicGrid<T>::resize(h, w);
        this->std::vector<Row>::resize(h);
        ITRR(row, *this) row.assign(w, val);
    }

    inline void resize(const int h, const int w) override {
        this->Internal::BasicGrid<T>::resize(h, w);
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
struct UnfoldedGrid : std::vector<T>, Internal::BasicGrid<T>, Internal::GridCommonMixin<T>, virtual Internal::IGrid<T> {
    using std::vector<T>::vector;

    UnfoldedGrid(int n = 0) : UnfoldedGrid(n, n) {}
    UnfoldedGrid(int h, int w, T val = T{}) : std::vector<T>(h*w, val), Internal::BasicGrid<T>(h, w) {}

    inline void assign(const UnfoldedGrid &source) {
        this->resize(source.height(), source.width());
        this->std::vector<T>::assign(ALL(source));
    }

    inline void assign(const int h, const int w, const T &&val = T{}) override {
        this->Internal::BasicGrid<T>::resize(h, w);
        this->std::vector<T>::assign(h*w, val);
    }

    inline void resize(const int h, const int w) override {
        this->Internal::BasicGrid<T>::resize(h, w);
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

template<class T> struct Internal::GridCommonMixin : virtual IGrid<T> {
    template<class U = T>
    void inline read(std::istream *ist = &std::cin) {
        REP(i, this->height()) REP(j, this->width()) {
            U val; *ist >> val;
            (*this)(i, j) = val;
        }
    }
};
