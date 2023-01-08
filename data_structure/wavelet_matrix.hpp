#pragma once


#include <cstdint>
#include <utility>
#include <vector>
#include <algorithm>
#include <array>
#include <iterator>
#include <optional>

#include <atcoder/internal_bit>

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

    size_type _n, _bits;
    std::vector<succinct_bit_vector> index;

    T _max = 0;

  public:
    template<class I> base(const I first, const I last) { this->build(first, last); }

    inline size_type size() const { return this->_n; }
    inline size_type bits() const { return this->_bits; }

    template<class I> __attribute__((optimize("O3"))) void build(const I first, const I last) {
        this->_n = std::distance(first, last);
        this->_max = first == last ? 0 : *std::max_element(first, last);
        this->_bits = atcoder::internal::ceil_pow2(this->_max);

        this->index.assign(this->_bits, this->_n);
        std::vector<T> cur(first, last), nxt(this->_n);

        for(size_type h = this->_bits - 1; h >= 0; --h) {
            for(size_type i = 0; i < this->_n; ++i) {
                if((cur[i] >> h) & 1) this->index[h].set(i);
            }
            this->index[h].build();

            std::array<typename std::vector<T>::iterator,2> itrs{ std::begin(nxt), std::next(std::begin(nxt), this->index[h].zeros()) };
            for(size_type i = 0; i < this->_n; ++i) *itrs[this->index[h].get(i)]++ = cur[i];

            std::swap(cur, nxt);
        }
    }

  protected:
    inline T get(size_type k) const {
        T res = 0;
        for(size_type h = this->_bits - 1; h >= 0; --h) {
            bool f = this->index[h].get(k);
            res |= f ? T(1) << h : 0;
            k = f ? this->index[h].rank1(k) + this->index[h].zeros() : this->index[h].rank0(k);
        }
        return res;
    }

    inline T kth_smallest(size_type l, size_type r, size_type k) const {
        T res = 0;
        for(size_type h = this->_bits - 1; h >= 0; --h) {
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
        if(upper >= (T(1) << this->_bits)) return r - l;

        size_type res = 0;
        for(size_type h = this->_bits - 1; h >= 0; --h) {
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

    // return a[k]
    inline T get(size_type k) const {
        k = this->_positivize_index(k);
        this->_validate_index_in_right_open(k);

        return this->base::get(k);
    }
    inline T operator[](const size_type k) const { return this->get(k); }


    // min value in a[l, r)
    inline T min(const size_type l, const size_type r) { return this->kth_smallest(l, r, 0); }
    // min value in whole a
    inline T min() { return this->kth_smallest(0, this->size(), 0); }

    // max value in a[l, r)
    inline T max(const size_type l, const size_type r) { return this->kth_largest(l, r, 0); }
    // min value in whole a
    inline T max() { return this->kth_largest(0, this->size(), 0); }

    inline T sum(size_type l, size_type r) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);

        T res = 0; REP(p, l, r) res += this->get(p);
        return res;
    }
    inline T sum() const { return this->sum(0, this->size()); }

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

    // count i s.t. (l <= i < r) and (v[i] < upper)
    inline size_type count_less_than(size_type l, size_type r, const T upper) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);

        return this->base::count_less_than(l, r, upper);
    }
    // count i s.t. v[i] < upper
    inline size_type count_less_than(const T upper) const {
        return this->base::count_less_than(0, this->size(), upper);
    }

    // count i s.t. (l <= i < r) and (v[i] >= lower)
    inline size_type count_not_less_than(size_type l, size_type r, const T lower) const {
        return r - l - this->count_less_than(l, r, lower);
    }
    // count i s.t. v[i] >= lower
    inline size_type count_not_less_than(const T lower) const {
        return this->size() - this->count_less_than(lower);
    }

    // count i s.t. (l <= i < r) and (lower <= v[i] < upper)
    inline size_type count(const size_type l, const size_type r, const T lower, const T upper) const {
        return this->count_less_than(l, r, upper) - this->count_less_than(l, r, lower);
    }
    // count i s.t. lower <= v[i] < upper
    inline size_type count(const T lower, const T upper) const {
        return this->count_less_than(upper) - this->count_less_than(lower);
    }

    // count i s.t. (l <= i < r) and v[i] == v
    inline size_type count(const size_type l, const size_type r, const T v) const {
        return this->count_less_than(l, r, v+1) - this->count_less_than(l, r, v);
    }
    // count i s.t. v[i] == v
    inline size_type count(const T v) const {
        return this->count_less_than(v+1) - this->count_less_than(v);
    }

    // k-th (0-indexed) smallest number in a[l, r)
    inline T kth_smallest(size_type l, size_type r, const size_type k) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);
        dev_assert(0 <= k and k < r-l);

        return this->base::kth_smallest(l, r, k);
    }
    // k-th (0-indexed) smallest number in a
    inline T kth_smallest(const size_type k) const {
        dev_assert(0 <= k and k < this->size());

        return this->base::kth_smallest(0, this->size(), k);
    }

    // k-th (0-indexed) largest number in a[l, r)
    inline T kth_largest(const size_type l, const size_type r, const size_type k) const {
        return this->kth_smallest(l, r, r-l-k-1);
    }
    // k-th (0-indexed) largest number in a
    inline T kth_largest(const size_type k) const {
        return this->kth_smallest(this->size()-k-1);
    }

    // std::max v[i] s.t. (l <= i < r) and (v[i] < upper)
    inline std::optional<T> prev_value(const size_type l, const size_type r, const T upper) const {
        size_type cnt = this->count_less_than(l, r, upper);
        return cnt == 0 ? std::optional<T>{} : std::optional<T>(this->kth_smallest(l, r, cnt - 1));
    }
    // std::max v[i] s.t. v[i] < upper
    inline std::optional<T> prev_value(const T upper) const {
        size_type cnt = this->count_less_than(upper);
        return cnt == 0 ? std::optional<T>{} : std::optional<T>(this->kth_smallest(cnt - 1));
    }

    // min v[i] s.t. (l <= i < r) and (lower <= v[i])
    inline std::optional<T> next_value(const size_type l, const size_type r, const T lower) {
        size_type cnt = this->count_less_than(l, r, lower);
        return cnt == r - l ? std::optional<T>{} : std::optional<T>(this->kth_smallest(l, r, cnt));
    }
    // min v[i] s.t. lower <= v[i]
    inline std::optional<T> next_value(const T lower) {
        size_type cnt = this->count_less_than(lower);
        return cnt == this->size() ? std::optional<T>{} : std::optional<T>(this->kth_smallest(cnt));
    }


    struct iterator : virtual internal::container_iterator_interface<T,wavelet_matrix> {
        iterator(const wavelet_matrix *const ref, const size_type pos) : internal::container_iterator_interface<T,wavelet_matrix>(ref, pos) {}

        inline value_type operator*() const override { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


} // namespace lib
