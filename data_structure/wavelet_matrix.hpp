#pragma once


#include <cstdint>
#include <utility>
#include <vector>
#include <algorithm>
#include <array>
#include <iterator>

#include "snippet/iterations.hpp"

#include "internal/dev_assert.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "data_structure/succinct_bit_vector.hpp"


namespace lib {

namespace internal {

namespace wavelet_matrix_lib {


// Thanks to: https://github.com/NyaanNyaan/library/blob/master/data-structure-2d/wavelet-matrix.hpp
template<class T> struct base {
    using size_type = internal::size_t;

  private:
    using int64 = std::int64_t;
    using uint64 = std::uint64_t;

    size_type _n, _depth;
    std::vector<T> a;
    std::vector<succinct_bit_vector> index;

  public:
    base(const size_type _n) : _n(std::max(_n, 1)), a(this->_n) {}
    template<class I> base(const I first, const I last) : _n(std::distance(first, last)), a(first, last) { this->build(); }

    inline size_type size() const { return this->_n; }
    inline size_type depth() const { return this->_depth; }

    __attribute__((optimize("O3"))) void build() {
        this->_depth = std::__lg(std::max<T>(*std::max_element(ALL(this->a)), 1)) + 1;

        this->index.assign(this->_depth, this->_n);
        std::vector<T> cur = this->a, nxt(this->_n);

        for(size_type h = this->_depth - 1; h >= 0; --h) {
            for(size_type i = 0; i < this->_n; ++i) {
                if((cur[i] >> h) & 1) this->index[h].set(i);
            }
            this->index[h].build();

            std::array<decltype(std::begin(nxt)),2> it{ std::begin(nxt), std::next(std::begin(nxt), this->index[h].zeros()) };
            for(size_type i = 0; i < this->_n; ++i) *it[this->index[h].get(i)]++ = cur[i];

            std::swap(cur, nxt);
        }
        return;
    }

  protected:
    void set(const size_type i, const T &x) {
        dev_assert(x >= 0);
        this->a[i] = x;
    }

    inline T get(size_type k) const {
        T res = 0;
        for(size_type h = this->_depth - 1; h >= 0; --h) {
            bool f = this->index[h].get(k);
            res |= f ? T(1) << h : 0;
            k = f ? this->index[h].rank1(k) + this->index[h].zeros() : this->index[h].rank0(k);
        }
        return res;
    }

    inline T kth_smallest(size_type l, size_type r, size_type k) const {
        T res = 0;
        for(size_type h = this->_depth - 1; h >= 0; --h) {
            size_type l0 = this->index[h].rank0(l), r0 = this->index[h].rank0(r);
            if(k < r0 - l0)
                l = l0, r = r0;
            else {
                k -= r0 - l0;
                res |= (T)1 << h;
                l += this->index[h].zeros() - l0;
                r += this->index[h].zeros() - r0;
            }
        }
        return res;
    }

    inline size_type count_less_than(size_type l, size_type r, const T upper) const {
        if(upper >= (T(1) << this->_depth)) return r - l;

        size_type res = 0;
        for(size_type h = this->_depth - 1; h >= 0; --h) {
            bool f = (upper >> h) & 1;
            size_type l0 = this->index[h].rank0(l), r0 = this->index[h].rank0(r);
            if(f) {
                res += r0 - l0;
                l += this->index[h].zeros() - l0;
                r += this->index[h].zeros() - r0;
            } else {
                l = l0;
                r = r0;
            }
        }
        return res;
    }

    inline std::pair<size_type,size_type> succ0(const size_type l, const size_type r, const size_type h) const {
        return std::make_pair(this->index[h].rank0(l), this->index[h].rank0(r));
    }

    inline std::pair<size_type,size_type> succ1(const size_type l, const size_type r, const size_type h) const {
        size_type l0 = this->index[h].rank0(l);
        size_type r0 = this->index[h].rank0(r);
        size_type zeros = this->index[h].zeros();
        return std::make_pair(l + zeros - l0, r + zeros - r0);
    }
};


} // namespace wavelet_matrix_lib

} // namespace internal


template<class T> struct wavelet_matrix : internal::wavelet_matrix_lib::base<T> {
  private:
    using base = typename internal::wavelet_matrix_lib::base<T>;

  public:
    using value_type = T;
    using size_type = typename base::size_type;

  protected:
    inline void _validate_index_in_right_open([[maybe_unused]] const size_type p) const {
        dev_assert(0 <= p and p < this->size());
    }
    inline void _validate_index_in_closed([[maybe_unused]] const size_type p) const {
        dev_assert(0 <= p and p <= this->size());
    }
    inline void _validate_rigth_open_interval([[maybe_unused]] const size_type l, [[maybe_unused]] const size_type r) const {
        dev_assert(0 <= l and l <= r and r <= this->size());
    }

    inline size_type _positivize_index(const size_type p) const {
        return p < 0 ? this->size() + p : p;
    }

  public:
    using base::base;

    inline void set(const size_type i, const T &x) const { this->base::set(i, x); }

    // return this->a[k]
    inline T get(size_type k) const {
        k = this->_positivize_index(k);
        this->_validate_index_in_right_open(k);

        return this->base::get(k);
    }
    inline T operator[](const size_type k) const { return this->get(k); }

    inline std::pair<size_type,size_type> succ0(const size_type l, const size_type r, const size_type h) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);

        return this->base::succ0(l, r, h);
    }
    inline std::pair<size_type,size_type> succ1(const size_type l, const size_type r, const size_type h) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);

        return this->base::succ1(l, r, h);
    }

    // count i s.t. (l <= i < r) && (v[i] < upper)
    inline size_type count_less_than(size_type l, size_type r, const T upper) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);

        return this->base::count_less_than(l, r, upper);
    }

    // k-th (0-indexed) smallest number in this->a[l, r)
    inline T kth_smallest(size_type l, size_type r, const size_type k) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);
        dev_assert(0 <= k and k < r-l);

        return this->base::kth_smallest(l, r, k);
    }
    // k-th (0-indexed) largest number in this->a[l, r)
    inline T kth_largest(const size_type l, const size_type r, const size_type k) const {
        return this->kth_smallest(l, r, r-l-k-1);
    }

    inline size_type count(const size_type l, const size_type r, const T lower, const T upper) const {
        return this->count_less_than(l, r, upper) - this->count_less_than(l, r, lower);
    }
    inline size_type count(const size_type l, const size_type r, const T v) const {
        return this->count_less_than(l, r, v+1) - this->count_less_than(l, r, v);
    }

    // std::max v[i] s.t. (l <= i < r) && (v[i] < upper)
    inline T prev_value(const size_type l, const size_type r, const T upper) const {
        size_type cnt = this->count_less_than(l, r, upper);
        return cnt == 0 ? T(-1) : this->kth_smallest(l, r, cnt - 1);
    }

    // min v[i] s.t. (l <= i < r) && (lower <= v[i])
    inline T next_value(const size_type l, const size_type r, const T lower) {
        size_type cnt = this->count_less_than(l, r, lower);
        return cnt == r - l ? T(-1) : this->kth_smallest(l, r, cnt);
    }


    struct iterator : virtual internal::container_iterator_interface<T,wavelet_matrix> {
        iterator(const wavelet_matrix *const ref, const size_type pos) : internal::container_iterator_interface<T,wavelet_matrix>(ref, pos) {}

        inline value_type operator*() const override { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


} // namespace lib
