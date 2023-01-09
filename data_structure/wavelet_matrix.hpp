#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <algorithm>
#include <array>
#include <iterator>
#include <optional>
#include <limits>

#include <atcoder/internal_bit>

#include "snippet/iterations.hpp"
#include "internal/dev_assert.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "data_structure/succinct_bit_vector.hpp"
#include "constants.hpp"


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
    std::vector<succinct_bit_vector> _index;
    std::vector<size_type> _first_ones;
    std::vector<std::vector<T>> _sum;

    T _max = 0;

  public:
    template<class I> base(const I first, const I last) { this->build(first, last); }

    inline size_type size() const { return this->_n; }
    inline size_type bits() const { return this->_bits; }

    template<class I> __attribute__((optimize("O3"))) void build(const I first, const I last) {
        this->_n = std::distance(first, last);
        this->_max = first == last ? 0 : *std::max_element(first, last) + 1;
        this->_bits = atcoder::internal::ceil_pow2(this->_max);

        this->_index.assign(this->_bits, this->_n);
        this->_first_ones.assign(this->_bits, this->_n);

        std::vector<T> depth(first, last), nxt(this->_n);

        this->_sum.assign(this->_bits+1, std::vector<T>(this->_n+1));

        {
            size_type i = 0;
            for(auto itr=first; itr!=last; ++i, ++itr) {
                this->_sum[this->_bits][i+1] = this->_sum[this->_bits][i] + *itr;
            }
        }

        for(size_type h = this->_bits - 1; h >= 0; --h) {
            std::vector<size_type> vals;

            for(size_type i = 0; i < this->_n; ++i) {
                if((depth[i] >> h) & 1) this->_index[h].set(i);
            }
            this->_index[h].build();
            this->_first_ones[h] = this->_index[h].zeros();

            std::array<typename std::vector<T>::iterator,2> itrs{ std::begin(nxt), std::next(std::begin(nxt), this->_index[h].zeros()) };
            for(size_type i=0; i<this->_n; ++i) *itrs[this->_index[h].get(i)]++ = depth[i];

            REP(i, nxt.size()) this->_sum[h][i+1] = this->_sum[h][i] + nxt[i];

            std::swap(depth, nxt);
        }
    }

  protected:
    T sum(const size_type l, const size_type r, const T& x, const T& y, const T& cur, const size_type bit) const {
        if(l == r) return 0;

        if(bit == -1) {
            if(x <= cur and cur < y) return cur * (r - l);
            return 0;
        }

        const T& nxt = (T{1} << bit) | cur;
        const T& ones = ((T{1} << bit) - 1) | nxt;

        if(ones < x or y <= cur) return 0;

        if(x <= cur and ones < y) return this->_sum[bit+1][r] - this->_sum[bit+1][l];

        const size_type l0 = this->_index[bit].rank0(l), r0 = this->_index[bit].rank0(r);
        const size_type l1 = l - l0, r1 = r - r0;

        return this->sum(l0, r0, x, y, cur, bit - 1) + this->sum(this->_first_ones[bit]+l1, this->_first_ones[bit]+r1, x, y, nxt, bit - 1);
    }
    inline T sum(const size_type l, const size_type r, const size_type x, const size_type y) const {
        return this->sum(l, r, x, y, 0, this->_bits-1);
    }

    inline T get(size_type k) const {
        T res = 0;
        for(size_type h = this->_bits - 1; h >= 0; --h) {
            bool f = this->_index[h].get(k);
            res |= f ? T(1) << h : 0;
            k = f ? this->_index[h].rank1(k) + this->_index[h].zeros() : this->_index[h].rank0(k);
        }
        return res;
    }

    inline T kth_smallest(size_type l, size_type r, size_type k) const {
        T res = 0;
        for(size_type h = this->_bits - 1; h >= 0; --h) {
            size_type l0 = this->_index[h].rank0(l), r0 = this->_index[h].rank0(r);
            if(k < r0 - l0)
                l = l0, r = r0;
            else {
                k -= r0 - l0;
                res |= (T)1 << h;
                l += this->_index[h].zeros() - l0;
                r += this->_index[h].zeros() - r0;
            }
        }
        return res;
    }

    inline size_type count_under(size_type l, size_type r, const T& y) const {
        if(y >= (T(1) << this->_bits)) return r - l;

        size_type res = 0;
        for(size_type h = this->_bits - 1; h >= 0; --h) {
            bool f = (y >> h) & 1;
            size_type l0 = this->_index[h].rank0(l), r0 = this->_index[h].rank0(r);
            if(f) {
                res += r0 - l0;
                l += this->_index[h].zeros() - l0;
                r += this->_index[h].zeros() - r0;
            } else {
                l = l0;
                r = r0;
            }
        }
        return res;
    }

    inline std::pair<size_type,size_type> succ0(const size_type l, const size_type r, const size_type h) const {
        return std::make_pair(this->_index[h].rank0(l), this->_index[h].rank0(r));
    }

    inline std::pair<size_type,size_type> succ1(const size_type l, const size_type r, const size_type h) const {
        size_type l0 = this->_index[h].rank0(l);
        size_type r0 = this->_index[h].rank0(r);
        size_type vals = this->_index[h].zeros();
        return std::make_pair(l + vals - l0, r + vals - r0);
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

    inline void set(const size_type i, const T& x) const { this->base::set(i, x); }

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

    // sum of c s.t. x <= c < y in a[l, r)
    T sum(size_type l, size_type r, const T& x, const T& y) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);

        return this->base::sum(l, r, x, y);
    }

    // sum of a[l, r)
    inline T sum(const size_type l, const size_type r) const {
        return this->sum(l, r, -1, std::numeric_limits<T>::max());
    }
    // sum of a
    inline T sum() const { return this->sum(0, this->size()); }

    // sum of a[i] for l <= i < r and l <= a[i] < r;
    inline std::pair<size_type,size_type> succ0(size_type l, size_type r, const size_type h) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);

        return this->base::succ0(l, r, h);
    }
    inline std::pair<size_type,size_type> succ1(size_type l, size_type r, const size_type h) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);

        return this->base::succ1(l, r, h);
    }

    // count i s.t. (l <= i < r) and (v[i] < y)
    inline size_type count_under(size_type l, size_type r, const T& y) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);

        return this->base::count_under(l, r, y);
    }
    // count i s.t. v[i] < y
    inline size_type count_under(const T& y) const {
        return this->base::count_under(0, this->size(), y);
    }

    // count i s.t. (l <= i < r) and (v[i] >= x)
    inline size_type count_or_over(size_type l, size_type r, const T& x) const {
        return r - l - this->count_under(l, r, x);
    }
    // count i s.t. v[i] >= x
    inline size_type count_or_over(const T& x) const {
        return this->size() - this->count_under(x);
    }

    // count i s.t. (l <= i < r) and (v[i] > x)
    inline size_type count_over(size_type l, size_type r, const T& x) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);

        return this->base::count_or_over(l, r, x+1);
    }
    // count i s.t. v[i] > x
    inline size_type count_over(const T& x) const {
        return this->base::count_over(0, this->size(), x);
    }
    // count i s.t. (l <= i < r) and (v[i] <= y)
    inline size_type count_or_under(size_type l, size_type r, const T& y) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);

        return this->base::count_under(l, r, y+1);
    }
    // count i s.t. v[i] <= y
    inline size_type count_or_under(const T& y) const {
        return this->base::count_or_under(0, this->size(), y);
    }

    // count i s.t. (l <= i < r) and (x <= v[i] < y)
    inline size_type count(const size_type l, const size_type r, const T& x, const T& y) const {
        return this->count_under(l, r, y) - this->count_under(l, r, x);
    }
    // count i s.t. x <= v[i] < y
    inline size_type count(const T& x, const T& y) const {
        return this->count_under(y) - this->count_under(x);
    }

    // count i s.t. (l <= i < r) and v[i] == v
    inline size_type count_equal_to(const size_type l, const size_type r, const T& v) const {
        return this->count_under(l, r, v+1) - this->count_under(l, r, v);
    }
    // count i s.t. v[i] == v
    inline size_type count_equal_to(const T& v) const {
        return this->count_under(v+1) - this->count_under(v);
    }

    template<conditions cond>
    inline size_type count(const size_type l, const size_type r, const T& v) const {
        if constexpr(cond == conditions::eq) return this->count_equal_to(l, r, v);
        if constexpr(cond == conditions::under) return this->count_under(l, r, v);
        if constexpr(cond == conditions::over) return this->count_over(l, r, v);
        if constexpr(cond == conditions::or_under) return this->count_or_under(l, r, v);
        if constexpr(cond == conditions::or_over) return this->count_or_over(l, r, v);
        assert(false);
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

    // std::max v[i] s.t. (l <= i < r) and (v[i] < y)
    inline std::optional<T> prev_value(const size_type l, const size_type r, const T& y) const {
        size_type cnt = this->count_under(l, r, y);
        return cnt == 0 ? std::optional<T>{} : std::optional<T>(this->kth_smallest(l, r, cnt - 1));
    }
    // std::max v[i] s.t. v[i] < y
    inline std::optional<T> prev_value(const T& y) const {
        size_type cnt = this->count_under(y);
        return cnt == 0 ? std::optional<T>{} : std::optional<T>(this->kth_smallest(cnt - 1));
    }

    // min v[i] s.t. (l <= i < r) and (x <= v[i])
    inline std::optional<T> next_value(const size_type l, const size_type r, const T& x) {
        size_type cnt = this->count_under(l, r, x);
        return cnt == r - l ? std::optional<T>{} : std::optional<T>(this->kth_smallest(l, r, cnt));
    }
    // min v[i] s.t. x <= v[i]
    inline std::optional<T> next_value(const T& x) {
        size_type cnt = this->count_under(x);
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
