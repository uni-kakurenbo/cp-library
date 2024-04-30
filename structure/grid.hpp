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
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "global/constants.hpp"

#include "adaptor/vector.hpp"
#include "adaptor/valarray.hpp"


namespace uni {

namespace internal {

namespace grid_impl {


template<class T>
struct container_base {
    using value_type = T;
    using size_type = internal::size_t;

  private:
    size_type _h, _w;

  protected:
    inline void _validate_index(__attribute__ ((unused)) const size_type i, __attribute__ ((unused)) const size_type j) const noexcept(NO_EXCEPT) {
        assert(0 <= i and i < this->height());
        assert(0 <= j and j < this->width());
    }

    inline size_type _positivize_row_index(const size_type x) const noexcept(NO_EXCEPT) {
        return x < 0 ? this->height() + x : x;
    }
    inline size_type _positivize_col_index(const size_type x) const noexcept(NO_EXCEPT) {
        return x < 0 ? this->width() + x : x;
    }

  public:
    container_base() = default;
    container_base(const size_type h, const size_type w) noexcept(NO_EXCEPT) : _h(h), _w(w) {}

    void resize(const size_type h, const size_type w) noexcept(NO_EXCEPT) {
        this->_h = h, this->_w = w;
    }

    inline size_type height() const noexcept(NO_EXCEPT) { return this->_h; }
    inline size_type width() const noexcept(NO_EXCEPT) { return this->_w; }

    inline size_type id(const size_type i, const size_type j) const noexcept(NO_EXCEPT) {
        const size_type _i = this->_positivize_row_index(i);
        const size_type _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return _i * this->width() + _j;
    }

    inline size_type id(const std::pair<size_type,size_type>& p) const noexcept(NO_EXCEPT) {
        return this->id(p.first, p.second);
    }

    inline std::pair<size_type,size_type> pos(const size_type id) const noexcept(NO_EXCEPT) {
        return { id / this->width(), id % this->width() };
    }
};

template<class Base>
struct regular_container : Base, container_base<typename Base::value_type::value_type> {
    using row_type = typename Base::value_type;
    using value_type = typename row_type::value_type;

    using size_type = internal::size_t;

    explicit regular_container(const size_type n = 0) noexcept(NO_EXCEPT) : regular_container(n, n) {}

    regular_container(const size_type h, const size_type w, const value_type &val = value_type()) noexcept(NO_EXCEPT)
      : Base(h, row_type(w, val)), container_base<value_type>(h, w)
    {}

    regular_container(const std::initializer_list<row_type> init_list) noexcept(NO_EXCEPT) : Base(init_list) {
        const auto rows = std::ranges::ssize(init_list);
        const auto first_cols = init_list.begin()->size();

        if constexpr (DEV_ENV) { ITR(init_row, init_list) assert((size_type)init_row.size() == first_cols); }

        this->container_base<value_type>::resize(rows, first_cols);
    }

    inline void assign(const regular_container &source) noexcept(NO_EXCEPT) {
        this->resize(source.height(), source.width());
        this->Base::assign(ALL(source));
    }

    inline void assign(const size_type h, const size_type w, const value_type &val = value_type()) noexcept(NO_EXCEPT) {
        this->container_base<value_type>::resize(h, w);
        this->Base::resize(h);
        ITRR(row, *this) row.assign(w, val);
    }

    inline void resize(const size_type h, const size_type w) noexcept(NO_EXCEPT) {
        this->container_base<value_type>::resize(h, w);
        this->Base::resize(h);
        ITRR(row, *this) row.resize(w);
    }

    inline auto& operator()(const size_type i, const size_type j) noexcept(NO_EXCEPT) {
        const size_type _i = this->_positivize_row_index(i);
        const size_type _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }

    inline const auto& operator()(const size_type i, const size_type j) const noexcept(NO_EXCEPT) {
        const size_type _i = this->_positivize_row_index(i);
        const size_type _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }

    inline auto& operator()(const std::pair<size_type,size_type>& p) noexcept(NO_EXCEPT) {
        return this->operator()(p.first, p.second);
    }

    inline const auto& operator()(const std::pair<size_type,size_type>& p) const noexcept(NO_EXCEPT) {
        return this->operator()(p.first, p.second);
    }
};

template<class Base>
struct unfolded_container : Base, container_base<typename Base::value_type> {
    using value_type = typename Base::value_type;
    using size_type = internal::size_t;

    explicit unfolded_container(size_type n = 0) noexcept(NO_EXCEPT) : unfolded_container(n, n) {}

    unfolded_container(const size_type h, const size_type w, const value_type &val = value_type()) noexcept(NO_EXCEPT)
      : Base(h * w, val), container_base<value_type>(h, w)
    {}

    unfolded_container(std::initializer_list<std::initializer_list<value_type>> init_list) noexcept(NO_EXCEPT) {
        const auto rows = std::ranges::ssize(init_list);
        const auto first_cols = std::ranges::ssize(std::ranges::begin(init_list));

        this->resize(rows, first_cols);

        {
            size_type index = 0;
            for(
                    auto itr = std::ranges::begin(init_list), itr_end = std::ranges::end(init_list);
                    itr!=itr_end;
                    ++itr
            )
            {
                assert((size_type)itr->size() == first_cols);
                for(auto v=itr->begin(), v_end=itr->end(); v!=v_end; ++v) (*this)[index++] = *v;
            }
        }
    }

    inline void assign(const unfolded_container &source) noexcept(NO_EXCEPT) {
        this->resize(source.height(), source.width());
        this->Base::assign(ALL(source));
    }

    inline void assign(const size_type h, const size_type w, const value_type &val = value_type()) noexcept(NO_EXCEPT) {
        this->container_base<value_type>::resize(h, w);
        this->Base::assign(h * w, val);
    }

    inline void resize(const size_type h, const size_type w) noexcept(NO_EXCEPT) {
        this->container_base<value_type>::resize(h, w);
        this->Base::resize(h * w);
    }

    inline value_type& operator()(const size_type i, const size_type j) noexcept(NO_EXCEPT) {
        const size_type _i = this->_positivize_row_index(i);
        const size_type _j = this->_positivize_col_index(j);
        return this->operator[](this->id(_i, _j));
    }

    inline value_type& operator()(const std::pair<size_type,size_type>& p) noexcept(NO_EXCEPT) {
        return this->operator()(this->id(p.first, p.second));
    }

    inline const value_type& operator()(const std::pair<size_type,size_type>& p) const noexcept(NO_EXCEPT) {
        return this->operator()(this->id(p.first, p.second));
    }
};


}  // namespace grid_impl


template<class Container>
struct grid_core : Container {
    using Container::Container;

    using value_type = Container::value_type;
    using size_type = internal::size_t;

    enum class invert_direction { vertical, horizontal };
    enum class rotate_direction { counter_clockwise, clockwise };


    inline bool is_valid(const size_type i, const size_type j) const noexcept(NO_EXCEPT) {
        return 0 <= i and i < this->height() and 0 <= j and j < this->width();
    }

    inline bool is_valid(const std::pair<size_type, size_type>& p) const noexcept(NO_EXCEPT) {
        return this->is_valid(p.first, p.second);
    }


    template<std::input_iterator I, std::sentinel_for<I> S>
    inline auto vicinities(const size_type i, const size_type j, I dirs_first, S dirs_last) const noexcept(NO_EXCEPT) {
        std::vector<std::pair<size_type, size_type>> res;
        REP(itr, dirs_first, dirs_last) {
            const size_type ii = i + itr->first, jj = j + itr->second;
            if(this->is_valid(ii, jj)) res.emplace_back(ii, jj);
        }
        return res;
    }

    template<class I, class C>
    inline auto vicinities(const std::pair<size_type, size_type>& p, const C dirs) const noexcept(NO_EXCEPT) {
        return this->vicinities(p.first, p.second, ALL(dirs));
    }

    inline auto vicinities4(const size_type i, const size_type j) const noexcept(NO_EXCEPT) { return this->vicinities(i, j, ALL(DIRS4)); }
    inline auto vicinities4(const std::pair<size_type, size_type>& p) const noexcept(NO_EXCEPT) { return this->vicinities(p.first, p.second, ALL(DIRS4)); }

    inline auto vicinities8(const size_type i, const size_type j) const noexcept(NO_EXCEPT) { return this->vicinities(i, j, ALL(DIRS8)); }
    inline auto vicinities8(const std::pair<size_type, size_type>& p) const noexcept(NO_EXCEPT) { return this->vicinities(p.first, p.second, ALL(DIRS8)); }


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
    inline grid_core& rotate(const size_type k) noexcept(NO_EXCEPT) {
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


template<class T, class row_type = vector<T>, class Base = vector<row_type>>
using grid = internal::grid_core<internal::grid_impl::regular_container<Base>>;

template<class T, class row_type = valarray<T>, class Base = vector<row_type>>
using valgrid = internal::grid_core<internal::grid_impl::regular_container<Base>>;

template<class T, class Base = vector<T>>
using unfolded_grid = internal::grid_core<internal::grid_impl::unfolded_container<Base>>;

template<class T, class Base = valarray<T>>
using unfolded_valgrid = internal::grid_core<internal::grid_impl::unfolded_container<Base>>;


} // namespace uni
