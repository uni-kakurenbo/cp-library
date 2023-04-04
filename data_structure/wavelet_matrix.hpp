#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <array>
#include <iterator>
#include <optional>
#include <limits>

#include <atcoder/internal_bit>

#include "snippet/iterations.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/range_reference.hpp"
#include "data_structure/bit_vector.hpp"
#include "constants.hpp"
#include "iterable/compression.hpp"


namespace lib {

namespace internal {

namespace wavelet_matrix_impl {


// Thanks to: https://github.com/NyaanNyaan/library/blob/master/data-structure-2d/wavelet-matrix.hpp
template<class T, class dict_type> struct base {
    using size_type = internal::size_t;

  private:
    size_type _n, _bits;
    std::vector<bit_vector> _index;
    std::vector<std::vector<T>> _sum;
    dict_type _first_pos;

    T _max = 0;

  public:
    base() {}
    template<class I> base(const I first, const I last) { this->build(first, last); }
    template<class U> base(const std::initializer_list<U>& init_list) : base(ALL(init_list)) {}

    inline size_type size() const { return this->_n; }
    inline size_type bits() const { return this->_bits; }

    template<class I> __attribute__((optimize("O3"))) void build(const I first, const I last) {
        this->_n = std::distance(first, last);
        this->_max = first == last ? -1 : *std::max_element(first, last);
        this->_bits = atcoder::internal::ceil_pow2(this->_max + 1);

        this->_index.assign(this->_bits, this->_n);

        std::vector<T> bit(first, last), nxt(this->_n);

        this->_sum.assign(this->_bits+1, std::vector<T>(this->_n+1));

        {
            size_type i = 0;
            for(auto itr=first; itr!=last; ++i, ++itr) {
                this->_sum[this->_bits][i+1] = this->_sum[this->_bits][i] + *itr;
            }
        }

        REPD(h, this->_bits) {
            std::vector<size_type> vals;

            for(size_type i = 0; i < this->_n; ++i) {
                if((bit[i] >> h) & 1) this->_index[h].set(i);
            }
            this->_index[h].build();

            std::array<typename std::vector<T>::iterator,2> itrs{ std::begin(nxt), std::next(std::begin(nxt), this->_index[h].zeros()) };
            for(size_type i=0; i<this->_n; ++i) *itrs[this->_index[h].get(i)]++ = bit[i];

            REP(i, this->_n) this->_sum[h][i+1] = this->_sum[h][i] + nxt[i];

            std::swap(bit, nxt);
        }

        REPD(i, this->_n) this->_first_pos[bit[i]] = i;
    }

  protected:
    inline T get(const size_type k) const { return this->_sum[this->_bits][k+1] - this->_sum[this->_bits][k]; }

    size_type select(const T& v, const size_type rank) const {
        if (v > this->_max) return this->_n;
        if (this->_first_pos.count(v) == 0) return this->_n;

        size_type pos = this->_first_pos.at(v) + rank + 1;
        REP(h, this->_bits) {
            const bool bit = (v >> h) & 1;
            if(bit) pos -= this->_index[h].zeros();
            pos = this->_index[h].select(bit, pos);
        }

        return pos - 1;
    }

    inline T kth_smallest(size_type l, size_type r, size_type k) const {
        T val = 0;

        for(size_type h = this->_bits - 1; h >= 0; --h) {
            size_type l0 = this->_index[h].rank0(l), r0 = this->_index[h].rank0(r);
            if(k < r0 - l0) {
                l = l0, r = r0;
            }
            else {
                k -= r0 - l0;
                val |= T{1} << h;
                l += this->_index[h].zeros() - l0;
                r += this->_index[h].zeros() - r0;
            }
        }

        return val;
    }
    inline size_type kth_smallest_index(size_type l, size_type r, size_type k) const {
        T val = 0;

        for(size_type h = this->_bits - 1; h >= 0; --h) {
            size_type l0 = this->_index[h].rank0(l), r0 = this->_index[h].rank0(r);
            if(k < r0 - l0) {
                l = l0, r = r0;
            }
            else {
                k -= r0 - l0;
                val |= T{1} << h;
                l += this->_index[h].zeros() - l0;
                r += this->_index[h].zeros() - r0;
            }
        }

        size_type left = 0;
        REPD(h, this->_bits) {
            const bool bit = (val >> h) & 1;
            left = this->_index[h].rank(bit, left);
            if(bit) left += this->_index[h].zeros();
        }

        return this->select(val, l + k - left);
    }

    inline T kth_largest(const size_type l, const size_type r, const size_type k) const {
        return this->kth_smallest(l, r, r-l-k-1);
    }
    inline size_type kth_largest_index(const size_type l, const size_type r, const size_type k) const {
        return this->kth_smallest_index(l, r, r-l-k-1);
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

    T sum_in_range(const size_type l, const size_type r, const T& x, const T& y, const T& cur, const size_type bit) const {
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

        return this->sum_in_range(l0, r0, x, y, cur, bit - 1) + this->sum_in_range(this->_index[bit].zeros()+l1, this->_index[bit].zeros()+r1, x, y, nxt, bit - 1);
    }
    inline T sum_in_range(const size_type l, const size_type r, const T& x, const T& y) const {
        return this->sum_in_range(l, r, x, y, 0, this->_bits-1);
    }
    inline T sum_under(const size_type l, const size_type r, const T& v) const {
        return this->sum_in_range(l, r, 0, v);
    }
    inline T sum_over(const size_type l, const size_type r, const T& v) const {
        return this->sum_in_range(l, r, v+1, std::numeric_limits<T>::max());
    }
    inline T sum_under_or(const size_type l, const size_type r, const T& v) const {
        return this->sum_in_range(l, r, 0, v+1);
    }
    inline T sum_over_or(const size_type l, const size_type r, const T& v) const {
        return this->sum_in_range(l, r, v, std::numeric_limits<T>::max());
    }
    inline T sum(const size_type l, const size_type r) const {
        return this->_sum[this->_bits][r] - this->_sum[this->_bits][l];
    }

    inline size_type count_under(size_type l, size_type r, const T& y) const {
        if(y >= (T{1} << this->_bits)) return r - l;

        size_type res = 0;
        REPD(h, this->_bits) {
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
    inline size_type count_in_range(const size_type l, const size_type r, const T& x, const T& y) const {
        return this->count_under(l, r, y) - this->count_under(l, r, x);
    }
    inline size_type count_under_or(size_type l, size_type r, const T& v) const {
        return this->count_under(l, r, v+1);
    }
    inline size_type count_over(size_type l, size_type r, const T& v) const {
        return this->count_over_or(l, r, v+1);
    }
    inline size_type count_over_or(size_type l, size_type r, const T& v) const {
        return r - l - this->count_under(l, r, v);
    }
    inline size_type count_equal_to(const size_type l, const size_type r, const T& v) const {
        return this->count_under(l, r, v+1) - this->count_under(l, r, v);
    }

    inline std::optional<T> next(const size_type l, const size_type r, const T& v, const size_type k) const {
        const size_type rank = this->count_under(l, r, v) + k;
        return (rank < 0 or rank >= r - l) ? std::optional<T>{} : std::optional<T>(this->kth_smallest(l, r, rank));
    }
    inline std::optional<T> prev(const size_type l, const size_type r, const T& v, const size_type k) const {
        const size_type rank = this->count_over(l, r, v) + k;
        return (rank < 0 or rank >= r - l) ? std::optional<T>{} : std::optional<T>(this->kth_largest(l, r, rank));
    }
};


} // namespace wavelet_matrix_impl

} // namespace internal


template<class T, class dict_type = std::unordered_map<T,internal::size_t>> struct compressed_wavelet_matrix;

template<class T, class dict_type = std::unordered_map<T,internal::size_t>>
struct wavelet_matrix : internal::wavelet_matrix_impl::base<T,dict_type> {
    using compressed = compressed_wavelet_matrix<T,dict_type>;

  private:
    using base = typename internal::wavelet_matrix_impl::base<T,dict_type>;

  public:
    using value_type = T;
    using size_type = typename base::size_type;

  protected:
    inline size_type _positivize_index(const size_type p) const {
        return p < 0 ? this->size() + p : p;
    }

  public:
    using base::base;

    bool empty() const { return this->size() == 0; }

    inline T get(size_type p) const {
        p = this->_positivize_index(p), assert(0 <= p && p < this->size());
        return this->base::get(p);
    }
    inline T operator[](const size_type p) const { return this->get(p); }

    inline size_type select(const T& v, const size_type p) const { return this->base::select(v, p); }

    struct iterator;
    struct range_reference;

    template<lib::interval rng = lib::interval::right_open>
    inline range_reference range(const size_type l, const size_type r) const {
        if constexpr(rng == lib::interval::right_open) return range_reference(this, l, r);
        if constexpr(rng == lib::interval::left_open) return range_reference(this, l+1, r+1);
        if constexpr(rng == lib::interval::open) return range_reference(this, l+1, r);
        if constexpr(rng == lib::interval::closed) return range_reference(this, l, r+1);
    }
    inline range_reference range() const { return range_reference(this, 0, this->size()); }
    inline range_reference operator()(const size_type l, const size_type r) const { return range_reference(this, l, r); }

    inline range_reference subseq(const size_type p, const size_type c) const { return range_reference(this, p, p+c); }
    inline range_reference subseq(const size_type p) const { return range_reference(this, p, this->size()); }


    struct range_reference : internal::range_reference<const wavelet_matrix> {
        range_reference(const wavelet_matrix *const super, const size_type l, const size_type r)
          : internal::range_reference<const wavelet_matrix>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

        inline T get(const size_type k) const {
            k = this->_super->_positivize_index(k);
            assert(0 <= k and k < this->size());

            return this->_super->get(this->_begin + k);
        }
        inline T operator[](const size_type k) const { return this->get(k); }


        inline T kth_smallest(const size_type k) const {
            assert(0 <= k && k < this->size());
            return this->_super->base::kth_smallest(this->_begin, this->_end, k);
        }
        inline auto kth_smallest_element(const size_type k) const {
            if(k == this->size()) return this->end();
            assert(0 <= k && k < this->size());
            return std::next(this->_super->begin(), this->_super->base::kth_smallest_index(this->_begin, this->_end, k));
        }

        inline T kth_largest(const size_type k) const {
            assert(0 <= k && k < this->size());
            return this->_super->base::kth_largest(this->_begin, this->_end, k);
        }
        inline auto kth_largest_element(const size_type k) const {
            if(k == this->size()) return this->end();
            assert(0 <= k && k < this->size());
            return std::next(this->_super->begin(), this->_super->base::kth_largest_index(this->_begin, this->_end, k));
        }

        inline T min() const { return this->kth_smallest(0); }
        inline T max() const { return this->kth_largest(0); }


        // (r-l)/2 th smallest (0-origin)
        inline T median() const { return this->kth_smallest(this->size() / 2); }

        inline T sum_in_range(const T& x, const T& y) const { return this->_super->base::sum_in_range(this->_begin, this->_end, x, y); }

        inline T sum_under(const T& v) const { return this->_super->base::sum_under(this->_begin, this->_end, v); }
        inline T sum_over(const T& v) const { return this->_super->base::sum_over(this->_begin, this->_end, v); }
        inline T sum_under_or(const T& v) const { return this->_super->base::sum_under_or(this->_begin, this->_end, v); }
        inline T sum_over_or(const T& v) const { return this->_super->base::sum_over_or(this->_begin, this->_end, v); }

        inline T sum(const T& x, const T& y) const { return this->_super->base::sum_in_range(this->_begin, this->_end, x, y); }
        inline T sum() const { return this->_super->base::sum(this->_begin, this->_end); }

        template<comp com>
        inline size_type sum(const T& v) const {
            if constexpr(com == comp::under) return this->sum_under(v);
            if constexpr(com == comp::over) return this->sum_over(v);
            if constexpr(com == comp::under_or) return this->sum_under_or(v);
            if constexpr(com == comp::over_or) return this->sum_over_or(v);
            assert(false);
        }


        inline size_type count_in_range(const T& x, const T& y) const { return this->_super->base::count_in_range(this->_begin, this->_end, x, y); }

        inline size_type count_under(const T& v) const { return this->_super->base::count_under(this->_begin, this->_end, v); }
        inline size_type count_over(const T& v) const { return this->_super->base::count_over(this->_begin, this->_end, v); }
        inline size_type count_under_or(const T& v) const { return this->_super->base::count_under_or(this->_begin, this->_end, v); }
        inline size_type count_over_or(const T& v) const { return this->_super->base::count_over_or(this->_begin, this->_end, v); }

        template<comp com = comp::equal_to>
        inline size_type count(const T& v) const {
            if constexpr(com == comp::eq) return this->_super->count_equal_to(this->_begin, this->_end, v);
            if constexpr(com == comp::under) return this->count_under(v);
            if constexpr(com == comp::over) return this->count_over(v);
            if constexpr(com == comp::under_or) return this->count_under_or(v);
            if constexpr(com == comp::over_or) return this->count_over_or(v);
            assert(false);
        }

        inline auto next_element(const T& v, const size_type k = 0) {
            return this->kth_smallest_element(std::clamp(this->count_under(v) + k, 0, this->size()));
        }
        inline auto prev_element(const T& v, const size_type k = 0) {
            return this->kth_smallest_element(std::clamp(this->count_over(v) - k, 0, this->size()));
        }

        inline std::optional<T> next(const T& v, const size_type k = 0) const { return this->_super->base::next(this->_begin, this->_end, v, k); }
        inline std::optional<T> prev(const T& v, const size_type k = 0) const { return this->_super->base::prev(this->_begin, this->_end, v, k); }
    };

    inline T kth_smallest(const size_type k) const { return this->range().kth_smallest(k); }
    inline T kth_smallest_element(const size_type k) const { return this->range().kth_smallest_element(k); }

    inline T kth_largest(const size_type k) const { return this->range().kth_largest(k); }
    inline T kth_largest_element(const size_type k) const { return this->range().kth_largest_element(k); }

    inline T min() const { return this->range().kth_smallest(0); }
    inline T max() const { return this->range().kth_largest(0); }

    // (size)/2 th smallest (0-origin)
    inline T median() const { return this->range().median(); }

    inline T sum_in_range(const T& x, const T& y) const { return this->range().sum_in_range(x, y); }

    inline T sum_under(const T& v) const { return this->range().sum_under(v); }
    inline T sum_over(const T& v) const { return this->range().sum_over(v); }
    inline T sum_under_or(const T& v) const { return this->range().sum_under_or(v); }
    inline T sum_over_or(const T& v) const { return this->range().sum_over_or(v); }

    inline T sum(const T& x, const T& y) const { return this->range().sum_in_range(x, y); }
    inline T sum() const { return this->range().sum(); }

    template<comp com>
    inline size_type sum(const T& v) const { return this->range().template sum<com>(v); }


    inline size_type count_in_range(const T& x, const T& y) const { return this->range().count_in_range(x, y); }

    inline size_type count_under(const T& v) const { return this->range().count_under(v); }
    inline size_type count_over(const T& v) const { return this->range().count_over(v); }
    inline size_type count_under_or(const T& v) const { return this->range().count_under_or(v); }
    inline size_type count_over_or(const T& v) const { return this->range().count_over_or(v); }

    inline size_type count(const T& x, const T& y) const { return this->range().count(x, y); }

    template<comp com = comp::equal_to>
    inline size_type count(const T& v) const { return this->range().template count<com>(v); }


    inline auto next_element(const T& v) { return this->range().next_element(v); }
    inline auto prev_element(const T& v) { return this->range().prev_element(v); }

    inline std::optional<T> next(const T& v, const size_type k = 0) const { return this->range().next(v, k); }
    inline std::optional<T> prev(const T& v, const size_type k = 0) const { return this->range().prev(v, k); }


  protected:
    using iterator_interface = internal::container_iterator_interface<T,wavelet_matrix>;

  public:
    struct iterator : virtual iterator_interface {
        iterator(const wavelet_matrix *const ref, const size_type pos) : iterator_interface(ref, pos) {}

        inline T operator*() const { return this->ref()->get(this->pos()); }
        inline T operator[](const typename iterator_interface::difference_type count) const { return *(*this + count); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


template<class T, class dict_type>
struct compressed_wavelet_matrix : protected wavelet_matrix<typename compression<T>::size_type,dict_type> {
  protected:
    using core = wavelet_matrix<typename compression<T>::size_type,dict_type>;

    compression<T> compressed;

  public:
    using value_type = typename core::value_type;
    using size_type = typename core::size_type;

    template<class I> compressed_wavelet_matrix(const I first, const I last) { this->build(first, last); }
    template<class I> void build(const I first, const I last) {
        this->compressed = compression<T>(first, last);
        this->core::build(ALL(this->compressed));
    }

    inline T get(const size_type k) const { return this->compressed(this->core::get(k)); }
    inline size_type operator[](const size_type k) const { return this->core::get(k); }


    struct iterator;
    struct range_reference;

    template<lib::interval rng = lib::interval::right_open>
    inline range_reference range(const size_type l, const size_type r) const {
        if constexpr(rng == lib::interval::right_open) return range_reference(this, l, r);
        if constexpr(rng == lib::interval::left_open) return range_reference(this, l+1, r+1);
        if constexpr(rng == lib::interval::open) return range_reference(this, l+1, r);
        if constexpr(rng == lib::interval::closed) return range_reference(this, l, r+1);
    }
    inline range_reference range() const { return range_reference(this, 0, this->size()); }
    inline range_reference operator()(const size_type l, const size_type r) const { return range_reference(this, l, r); }

    inline range_reference subseq(const size_type p, const size_type c) const { return range_reference(this, p, p+c); }
    inline range_reference subseq(const size_type p) const { return range_reference(this, p, this->size()); }


    struct range_reference : internal::range_reference<const compressed_wavelet_matrix> {
        range_reference(const compressed_wavelet_matrix *const super, const size_type l, const size_type r)
          : internal::range_reference<const compressed_wavelet_matrix>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

      private:
        inline auto _range() const { return this->_super->core::range(this->_begin, this->_end); }

      public:
        inline T get(const size_type k) const { return this->_super->compressed(this->_range().get(k)); }
        inline T operator[](const size_type k) const { return this->get(k); }


        inline T kth_smallest(const size_type k) const { return this->_super->compressed(this->_range().kth_smallest(k)); }
        inline auto kth_smallest_element(const size_type k) const {
            return std::next(this->_super->begin(), std::distance(this->_super->core::begin(), this->_range().kth_smallest_element(k)));
        }

        inline T kth_largest(const size_type k) const { return this->_super->compressed(this->_range().kth_largest(k));}
        inline auto kth_largest_element(const size_type k) const {
            return std::next(this->_super->begin(), std::distance(this->_super->core::begin(), this->_range().kth_largest_element(k)));
        }

        inline T min() const { return this->kth_smallest(0); }
        inline T max() const { return this->kth_largest(0); }


        // (r-l)/2 th smallest (0-origin)
        inline T median() const { return this->kth_smallest(this->size() / 2); }


        inline size_type count_in_range(const T& x, const T& y) const {
            return this->_range().count_in_range(this->_super->compressed.rank(x), this->_super->compressed.rank(y));
        }

        inline size_type count_under(const T& v) const { return this->_range().count_under(this->_super->compressed.rank(v)); }
        inline size_type count_over(const T& v) const { return this->_range().count_over(this->_super->compressed.rank2(v)); }
        inline size_type count_under_or(const T& v) const { return this->_range().count_under_or(this->_super->compressed.rank2(v)); }
        inline size_type count_over_or(const T& v) const { return this->_range().count_over_or(this->_super->compressed.rank(v)); }

        template<comp com = comp::equal_to>
        inline size_type count(const T& v) const { return this->_range().template count<com>(this->_super->compressed.rank(v)); }


        inline auto next_element(const T& v, const size_type k = 0) {
            return this->kth_smallest_element(std::clamp(this->_range().count_under(this->_super->compressed.rank(v) + k), 0, this->size()));
        }
        inline auto prev_element(const T& v, const size_type k = 0) {
            return this->kth_largest_element(std::clamp(this->_range().count_over(this->_super->compressed.rank2(v) + k), 0, this->size()));
        }

        inline std::optional<T> next(const T& v, const size_type k = 0) const {
            const std::optional<size_type> res = this->_range().next(this->_super->compressed.rank(v), k);
            if(res.has_value()) return this->_super->compressed(res.value());
            return {};
        }
        inline std::optional<T> prev(const T& v, const size_type k = 0) const {
            const std::optional<size_type> res = this->_range().prev(this->_super->compressed.rank2(v), k);
            if(res.has_value()) return this->_super->compressed(res.value());
            return {};
        }
    };

    inline T kth_smallest(const size_type k) const { return this->range().kth_smallest(k); }
    inline auto kth_smallest_element(const size_type k) const { return this->range().kth_smallest_element(k); }

    inline T kth_largest(const size_type k) const { return this->range().kth_largest(k); }
    inline auto kth_largest_element(const size_type k) const { return this->range().kth_largest_element(k); }

    inline T min() const { return this->range().kth_smallest(0); }
    inline T max() const { return this->range().kth_largest(0); }

    inline T median() const { return this->range().median(); }

    inline size_type count_in_range(const T& x, const T& y) const { return this->range().count_in_range(x, y); }

    inline size_type count_under(const T& v) const { return this->range().count_under(v); }
    inline size_type count_over(const T& v) const { return this->range().count_over(v); }
    inline size_type count_under_or(const T& v) const { return this->range().count_under_or(v); }
    inline size_type count_over_or(const T& v) const { return this->range().count_over_or(v); }

    template<comp com = comp::equal_to> inline size_type count(const T& v) const { return this->range().template count<com>(v); }

    inline auto next_element(const T& v) { return this->range().next_element(v); }
    inline auto prev_element(const T& v) { return this->range().prev_element(v); }

    inline std::optional<T> next(const T& v, const size_type k = 0) const { return this->range().next(v, k); }
    inline std::optional<T> prev(const T& v, const size_type k = 0) const { return this->range().prev(v, k); }


  protected:
    using iterator_interface = internal::container_iterator_interface<T,compressed_wavelet_matrix>;

  public:
    struct iterator : virtual iterator_interface {
        iterator(const compressed_wavelet_matrix *const ref, const size_type pos) : iterator_interface(ref, pos) {}

        inline T operator*() const { return this->ref()->get(this->pos()); }
        inline T operator[](const typename iterator_interface::difference_type count) const { return *(*this + count); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


} // namespace lib
