#pragma once


#include <cassert>
#include <cstdint>
#include <utility>
#include <vector>
#include <valarray>
#include <unordered_map>
#include <algorithm>
#include <array>
#include <iterator>
#include <optional>
#include <limits>
#include <type_traits>
#include <ranges>
#include <concepts>
#include <bit>


#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/range_reference.hpp"

#include "adaptor/gnu/hash_table.hpp"

#include "global/constants.hpp"

#include "iterable/compressed.hpp"

#include "data_structure/bit_vector.hpp"

#include "numeric/bit.hpp"
#include "numeric/arithmetic.hpp"


namespace uni {

namespace internal {

namespace wavelet_matrix_impl {


// Thanks to: https://github.com/NyaanNyaan/library/blob/master/data-structure-2d/wavelet-matrix.hpp
template<std::unsigned_integral T, class MapType>
    requires std::same_as<T, typename MapType::key_type>
struct base {
    using size_type = internal::size_t;
    using impl_type = T;

  private:
    size_type _n;
    int _bits;

    std::vector<bit_vector> _index;
    std::vector<std::vector<impl_type>> _sum;

    MapType _first_pos;

    impl_type _max = 0;

  public:
    base() = default;

    template<std::ranges::input_range R>
    explicit base(R&& range) noexcept(NO_EXCEPT) : base(ALL(range)) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    base(I first, S last) noexcept(NO_EXCEPT) { this->build(first, last); }

    template<std::convertible_to<impl_type> U>
    base(const std::initializer_list<U>& init_list) noexcept(NO_EXCEPT) : base(ALL(init_list)) {}

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n; }
    inline size_type bits() const noexcept(NO_EXCEPT) { return this->_bits; }

    template<std::ranges::input_range R>
    inline void build(R&& range) noexcept(NO_EXCEPT) { this->build(ALL(range)); }

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
        __attribute__((optimize("O3")))
    void build(I first, S last) noexcept(NO_EXCEPT) {
        this->_n = static_cast<size_type>(std::ranges::distance(first, last));
        this->_max = first == last ? -1 : *std::ranges::max_element(first, last);
        this->_bits = std::bit_width(this->_max + 1);

        this->_index.assign(this->_bits, this->_n);

        std::vector<impl_type> bit(first, last), nxt(this->_n);

        this->_sum.assign(this->_bits + 1, std::vector<impl_type>(this->_n + 1));

        {
            size_type i = 0;
            for(auto itr=first; itr!=last; ++i, ++itr) {
                assert(*itr >= 0);
                this->_sum[this->_bits][i + 1] = this->_sum[this->_bits][i] + *itr;
            }
        }

        REPD(h, this->_bits) {
            std::vector<size_type> vals;

            for(size_type i = 0; i < this->_n; ++i) {
                if((bit[i] >> h) & 1) this->_index[h].set(i);
            }
            this->_index[h].build();

            std::array<typename std::vector<impl_type>::iterator, 2> itrs{
                std::ranges::begin(nxt), std::ranges::next(std::ranges::begin(nxt), this->_index[h].zeros())
            };

            REP(i, this->_n) *itrs[this->_index[h].get(i)]++ = bit[i];

            REP(i, this->_n) this->_sum[h][i + 1] = this->_sum[h][i] + nxt[i];

            std::swap(bit, nxt);
        }

        REPD(i, this->_n) this->_first_pos[bit[i]] = static_cast<MapType::mapped_type>(i);
    }

  protected:
    inline auto get(const size_type k) const noexcept(NO_EXCEPT) {
        return this->_sum[this->_bits][k + 1] - this->_sum[this->_bits][k];
    }

    auto select(const impl_type& v, const size_type rank) const noexcept(NO_EXCEPT) {
        if(v > this->_max) return this->_n;
        if(not this->_first_pos.contains(v)) return this->_n;

        size_type pos = this->_first_pos.at(v) + rank;
        REP(h, this->_bits) {
            if(uni::bit(v, h)) pos = this->_index[h].select1(pos - this->_index[h].zeros());
            else pos = this->_index[h].select0(pos);
        }

        return pos;
    }


    auto kth_smallest(size_type *const l, size_type *const r, size_type *const k) const noexcept(NO_EXCEPT) {
        impl_type val = 0;

        for(size_type h = this->_bits - 1; h >= 0; --h) {
            size_type l0 = this->_index[h].rank0(*l), r0 = this->_index[h].rank0(*r);
            if(*k < r0 - l0) {
                *l = l0, *r = r0;
            }
            else {
                *k -= r0 - l0;
                val |= impl_type{1} << h;
                *l += this->_index[h].zeros() - l0;
                *r += this->_index[h].zeros() - r0;
            }
        }

        return val;
    }

    inline auto kth_smallest(size_type l, size_type r, size_type k) const noexcept(NO_EXCEPT) {
        return this->kth_smallest(&l, &r, &k);
    }

    auto kth_smallest_index(size_type l, size_type r, size_type k) const noexcept(NO_EXCEPT) {
       const impl_type val = this->kth_smallest(&l, &r, &k);

        size_type left = 0;
        REPD(h, this->_bits) {
            if(uni::bit(val, h)) left = this->_index[h].rank1(left) + this->_index[h].zeros();
            else  left = this->_index[h].rank0(left);
        }

        return this->select(val, l + k - left);
    }

    inline auto kth_largest(const size_type l, const size_type r, const size_type k) const noexcept(NO_EXCEPT) {
        return this->kth_smallest(l, r, r - l - k - 1);
    }
    inline auto kth_largest_index(const size_type l, const size_type r, const size_type k) const noexcept(NO_EXCEPT) {
        return this->kth_smallest_index(l, r, r - l - k - 1);
    }


    inline auto succ0(const size_type l, const size_type r, const size_type h) const noexcept(NO_EXCEPT) {
        return std::make_pair(this->_index[h].rank0(l), this->_index[h].rank0(r));
    }
    inline auto succ1(const size_type l, const size_type r, const size_type h) const noexcept(NO_EXCEPT) {
        const size_type l0 = this->_index[h].rank0(l);
        const size_type r0 = this->_index[h].rank0(r);
        const size_type vals = this->_index[h].zeros();
        return std::make_pair(l + vals - l0, r + vals - r0);
    }


    impl_type sum_in_range(
        const size_type l, const size_type r,
        const impl_type& x, const impl_type& y,
        const impl_type& cur, const size_type bit
    ) const noexcept(NO_EXCEPT)
    {
        if(l == r) return 0;

        if(bit == -1) {
            if(x <= cur && cur <= y) return cur * (r - l);
            return 0;
        }

        const impl_type nxt = (impl_type{1} << bit) | cur;
        const impl_type ones = ((impl_type{1} << bit) - 1) | nxt;

        if(ones < x || y < cur) return 0;

        if(x <= cur && ones <= y) return this->_sum[bit + 1][r] - this->_sum[bit + 1][l];

        const size_type l0 = this->_index[bit].rank0(l), r0 = this->_index[bit].rank0(r);
        const size_type l1 = l - l0, r1 = r - r0;

        return
            this->sum_in_range(l0, r0, x, y, cur, bit - 1) +
            this->sum_in_range(this->_index[bit].zeros() + l1, this->_index[bit].zeros() + r1, x, y, nxt, bit - 1);
    }

    inline auto sum_in_range(const size_type l, const size_type r, const impl_type& x, const impl_type& y) const noexcept(NO_EXCEPT) {
        return this->sum_in_range(l, r, x, y, 0, this->_bits - 1);
    }
    inline auto sum_under(const size_type l, const size_type r, const impl_type& v) const noexcept(NO_EXCEPT) {
        return this->sum_in_range(l, r, 0, v - 1);
    }
    inline auto sum_over(const size_type l, const size_type r, const impl_type& v) const noexcept(NO_EXCEPT) {
        return this->sum_in_range(l, r, v + 1, std::numeric_limits<impl_type>::max());
    }
    inline auto sum_or_under(const size_type l, const size_type r, const impl_type& v) const noexcept(NO_EXCEPT) {
        return this->sum_in_range(l, r, 0, v);
    }
    inline auto sum_or_over(const size_type l, const size_type r, const impl_type& v) const noexcept(NO_EXCEPT) {
        return this->sum_in_range(l, r, v, std::numeric_limits<impl_type>::max());
    }
    inline auto sum(const size_type l, const size_type r) const noexcept(NO_EXCEPT) {
        return this->_sum[this->_bits][r] - this->_sum[this->_bits][l];
    }

    auto count_under(size_type l, size_type r, const impl_type& y) const noexcept(NO_EXCEPT) {
        if(y >= (impl_type{1} << this->_bits)) return r - l;

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

    inline auto count_or_under(const size_type l, const size_type r, const impl_type& v) const noexcept(NO_EXCEPT) {
        return this->count_under(l, r, v + 1);
    }
    inline auto count_or_over(const size_type l, const size_type r, const impl_type& v) const noexcept(NO_EXCEPT) {
        return r - l - this->count_under(l, r, v);
    }
    inline auto count_over(const size_type l, const size_type r, const impl_type& v) const noexcept(NO_EXCEPT) {
        return this->count_or_over(l, r, v + 1);
    }
    inline auto count_in_range(const size_type l, const size_type r, const impl_type& x, const impl_type& y) const noexcept(NO_EXCEPT) {
        return this->count_or_under(l, r, y) - this->count_under(l, r, x);
    }
    inline auto count_equal_to(const size_type l, const size_type r, const impl_type& v) const noexcept(NO_EXCEPT) {
        return this->count_in_range(l, r, v, v);
    }

    inline std::optional<impl_type> next(const size_type l, const size_type r, const impl_type& v, const size_type k) const noexcept(NO_EXCEPT) {
        const size_type rank = this->count_under(l, r, v) + k;
        if(rank < 0 || rank >= r - l) return {};
        return { this->kth_smallest(l, r, rank) };
    }

    inline std::optional<impl_type> prev(const size_type l, const size_type r, const impl_type& v, const size_type k) const noexcept(NO_EXCEPT) {
        const size_type rank = this->count_over(l, r, v) + k;
        if(rank < 0 || rank >= r - l) return {};
        return this->kth_largest(l, r, rank);
    }
};


} // namespace wavelet_matrix_impl

} // namespace internal


template<std::integral T, template<class...> class MapTemplate = gnu::gp_hash_table>
struct compressed_wavelet_matrix;


template<std::integral T, template<class...> class MapTemplate = gnu::gp_hash_table>
struct wavelet_matrix : internal::wavelet_matrix_impl::base<std::make_unsigned_t<T>, MapTemplate<std::make_unsigned_t<T>, u32>> {
    using value_type = T;
    using impl_type = std::make_unsigned_t<T>;

    using map_type = MapTemplate<impl_type, u32>;
    using size_type = internal::size_t;

    using compressed = compressed_wavelet_matrix<value_type, MapTemplate>;

  private:
    using base = internal::wavelet_matrix_impl::base<impl_type, map_type>;

  public:
  protected:
    inline size_type _positivize_index(const size_type p) const noexcept(NO_EXCEPT) {
        return p < 0 ? this->size() + p : p;
    }

  public:
    using base::base;

    inline bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }

    inline value_type get(size_type p) const noexcept(NO_EXCEPT) {
        p = this->_positivize_index(p), assert(0 <= p && p < this->size());
        return this->base::get(p);
    }
    inline auto operator[](const size_type p) const noexcept(NO_EXCEPT) { return this->get(p); }

    inline auto select(const value_type& v, const size_type p) const noexcept(NO_EXCEPT) {
        return this->base::select(v, p);
    }

    struct iterator;
    struct range_reference;

    template<uni::interval_notation rng = uni::interval_notation::right_open>
    inline auto range(const size_type l, const size_type r) const noexcept(NO_EXCEPT) {
        if constexpr(rng == uni::interval_notation::right_open) return range_reference(this, l, r);
        if constexpr(rng == uni::interval_notation::left_open) return range_reference(this, l + 1, r + 1);
        if constexpr(rng == uni::interval_notation::open) return range_reference(this, l + 1, r);
        if constexpr(rng == uni::interval_notation::closed) return range_reference(this, l, r + 1);
    }
    inline auto range() const noexcept(NO_EXCEPT) { return range_reference(this, 0, this->size()); }
    inline auto operator()(const size_type l, const size_type r) const noexcept(NO_EXCEPT) { return range_reference(this, l, r); }

    inline auto subseq(const size_type p, const size_type c) const noexcept(NO_EXCEPT) { return range_reference(this, p, p+c); }
    inline auto subseq(const size_type p) const noexcept(NO_EXCEPT) { return range_reference(this, p, this->size()); }


    struct range_reference : internal::range_reference<const wavelet_matrix> {
        range_reference(const wavelet_matrix *const super, const size_type l, const size_type r) noexcept(NO_EXCEPT)
          : internal::range_reference<const wavelet_matrix>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

        inline auto get(const size_type k) const noexcept(NO_EXCEPT) {
            k = this->_super->_positivize_index(k);
            assert(0 <= k && k < this->size());

            return this->_super->get(this->_begin + k);
        }
        inline auto operator[](const size_type k) const noexcept(NO_EXCEPT) { return this->get(k); }


        inline value_type kth_smallest(const size_type k) const noexcept(NO_EXCEPT) {
            assert(0 <= k && k < this->size());
            return this->_super->base::kth_smallest(this->_begin, this->_end, k);
        }
        inline auto kth_smallest_element(const size_type k) const noexcept(NO_EXCEPT) {
            if(k == this->size()) return this->end();
            assert(0 <= k && k < this->size());
            return std::ranges::next(this->_super->begin(), this->_super->base::kth_smallest_index(this->_begin, this->_end, k));
        }

        inline value_type kth_largest(const size_type k) const noexcept(NO_EXCEPT) {
            assert(0 <= k && k < this->size());
            return this->_super->base::kth_largest(this->_begin, this->_end, k);
        }
        inline auto kth_largest_element(const size_type k) const noexcept(NO_EXCEPT) {
            if(k == this->size()) return this->end();
            assert(0 <= k && k < this->size());
            return std::ranges::next(this->_super->begin(), this->_super->base::kth_largest_index(this->_begin, this->_end, k));
        }

        inline auto min() const noexcept(NO_EXCEPT) { return this->kth_smallest(0); }
        inline auto max() const noexcept(NO_EXCEPT) { return this->kth_largest(0); }


        // (r-l)/2 th smallest (0-origin)
        inline auto median() const noexcept(NO_EXCEPT) { return this->kth_smallest(this->size() / 2); }

        inline value_type sum_in_range(const value_type& x, const value_type& y) const noexcept(NO_EXCEPT) { return this->_super->base::sum_in_range(this->_begin, this->_end, x, y); }

        inline value_type sum_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_super->base::sum_under(this->_begin, this->_end, v); }
        inline value_type sum_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_super->base::sum_over(this->_begin, this->_end, v); }
        inline value_type sum_or_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_super->base::sum_or_under(this->_begin, this->_end, v); }
        inline value_type sum_or_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_super->base::sum_or_over(this->_begin, this->_end, v); }

        inline value_type sum(const value_type& x, const value_type& y) const noexcept(NO_EXCEPT) { return this->_super->base::sum_in_range(this->_begin, this->_end, x, y); }
        inline value_type sum() const noexcept(NO_EXCEPT) { return this->_super->base::sum(this->_begin, this->_end); }

        template<comparison com>
        inline auto sum(const value_type& v) const noexcept(NO_EXCEPT) {
            if constexpr(com == comparison::under) return this->sum_under(v);
            if constexpr(com == comparison::over) return this->sum_over(v);
            if constexpr(com == comparison::or_under) return this->sum_or_under(v);
            if constexpr(com == comparison::or_over) return this->sum_or_over(v);
            assert(false);
        }


        inline auto count_in_range(const value_type& x, const value_type& y) const noexcept(NO_EXCEPT) {
            return this->_super->base::count_in_range(this->_begin, this->_end, x, y);
        }

        inline auto count_equal_to(const value_type& v) const noexcept(NO_EXCEPT) { return this->_super->base::count_equal_to(this->_begin, this->_end, v); }
        inline auto count_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_super->base::count_under(this->_begin, this->_end, v); }
        inline auto count_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_super->base::count_over(this->_begin, this->_end, v); }
        inline auto count_or_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_super->base::count_or_under(this->_begin, this->_end, v); }
        inline auto count_or_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_super->base::count_or_over(this->_begin, this->_end, v); }

        template<comparison com = comparison::equal_to>
        inline auto count(const value_type& v) const noexcept(NO_EXCEPT) {
            if constexpr(com == comparison::equal_to) return this->count_equal_to(v);
            if constexpr(com == comparison::under) return this->count_under(v);
            if constexpr(com == comparison::over) return this->count_over(v);
            if constexpr(com == comparison::or_under) return this->count_or_under(v);
            if constexpr(com == comparison::or_over) return this->count_or_over(v);
            assert(false);
        }

        inline auto next_element(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) {
            return this->kth_smallest_element(std::clamp(this->count_under(v) + k, size_type{ 0 }, this->size()));
        }
        inline auto prev_element(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) {
            return this->kth_largest_element(std::clamp(this->count_over(v) - k, size_type{ 0 }, this->size()));
        }

        inline std::optional<value_type> next(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) {
            return this->_super->base::next(this->_begin, this->_end, v, k);
        }
        inline std::optional<value_type> prev(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) {
            return this->_super->base::prev(this->_begin, this->_end, v, k);
        }
    };

    inline auto kth_smallest(const size_type k) const noexcept(NO_EXCEPT) { return this->range().kth_smallest(k); }
    inline auto kth_smallest_element(const size_type k) const noexcept(NO_EXCEPT) { return this->range().kth_smallest_element(k); }

    inline auto kth_largest(const size_type k) const noexcept(NO_EXCEPT) { return this->range().kth_largest(k); }
    inline auto kth_largest_element(const size_type k) const noexcept(NO_EXCEPT) { return this->range().kth_largest_element(k); }

    inline auto min() const noexcept(NO_EXCEPT) { return this->range().kth_smallest(0); }
    inline auto max() const noexcept(NO_EXCEPT) { return this->range().kth_largest(0); }

    // (size)/2 th smallest (0-origin)
    inline auto median() const noexcept(NO_EXCEPT) { return this->range().median(); }

    inline auto sum_in_range(const value_type& x, const value_type& y) const noexcept(NO_EXCEPT) { return this->range().sum_in_range(x, y); }

    inline auto sum_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().sum_under(v); }
    inline auto sum_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().sum_over(v); }
    inline auto sum_or_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().sum_or_under(v); }
    inline auto sum_or_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().sum_or_over(v); }

    inline auto sum(const value_type& x, const value_type& y) const noexcept(NO_EXCEPT) { return this->range().sum_in_range(x, y); }
    inline auto sum() const noexcept(NO_EXCEPT) { return this->range().sum(); }

    template<comparison com>
    inline auto sum(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().template sum<com>(v); }


    inline auto count_in_range(const value_type& x, const value_type& y) const noexcept(NO_EXCEPT) { return this->range().count_in_range(x, y); }

    inline auto count_qeual_to(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().count_qeual_to(v); }
    inline auto count_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().count_under(v); }
    inline auto count_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().count_over(v); }
    inline auto count_or_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().count_or_under(v); }
    inline auto count_or_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().count_or_over(v); }

    template<comparison com = comparison::equal_to>
    inline auto count(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().template count<com>(v); }


    inline auto next_element(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().next_element(v); }
    inline auto prev_element(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().prev_element(v); }

    inline auto next(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) { return this->range().next(v, k); }
    inline auto prev(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) { return this->range().prev(v, k); }

    struct iterator;

  protected:
    using iterator_interface = internal::container_iterator_interface<value_type, const wavelet_matrix, const iterator>;

  public:
    struct iterator : iterator_interface {
        iterator() noexcept = default;
        iterator(const wavelet_matrix *const ref, const size_type pos) noexcept(NO_EXCEPT) : iterator_interface(ref, pos) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    };

    inline auto begin() const noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline auto end() const noexcept(NO_EXCEPT) { return iterator(this, this->size()); }
};


template<std::integral T, template<class...> class MapTemplate>
struct compressed_wavelet_matrix : protected wavelet_matrix<u32, MapTemplate> {
    using value_type = T;
    using size_type = internal::size_t;

  protected:
    using core = wavelet_matrix<u32, MapTemplate>;
    using compresser = compressed<value_type, valarray<u32>>;

    compresser _comp;

  public:
    compressed_wavelet_matrix() = default;

    template<std::ranges::input_range R>
    explicit compressed_wavelet_matrix(R&& range) noexcept(NO_EXCEPT) : compressed_wavelet_matrix(ALL(range)) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    compressed_wavelet_matrix(I first, S last) noexcept(NO_EXCEPT) { this->build(first, last); }

    template<std::input_iterator I, std::sentinel_for<I> S>
    inline void build(I first, S last) noexcept(NO_EXCEPT) {
        this->_comp = compresser(first, last);
        this->core::build(ALL(this->_comp));
    }

    inline auto get(const size_type k) const noexcept(NO_EXCEPT) { return this->_comp.value(this->core::get(k)); }
    inline auto operator[](const size_type k) const noexcept(NO_EXCEPT) { return this->_comp.value(this->core::get(k)); }


    struct iterator;
    struct range_reference;

    template<uni::interval_notation rng = uni::interval_notation::right_open>
    inline auto range(const size_type l, const size_type r) const noexcept(NO_EXCEPT) {
        if constexpr(rng == uni::interval_notation::right_open) return range_reference(this, l, r);
        if constexpr(rng == uni::interval_notation::left_open) return range_reference(this, l + 1, r + 1);
        if constexpr(rng == uni::interval_notation::open) return range_reference(this, l + 1, r);
        if constexpr(rng == uni::interval_notation::closed) return range_reference(this, l, r + 1);
    }
    inline auto range() const noexcept(NO_EXCEPT) { return range_reference(this, 0, this->size()); }
    inline auto operator()(const size_type l, const size_type r) const noexcept(NO_EXCEPT) { return range_reference(this, l, r); }

    inline auto subseq(const size_type p, const size_type c) const noexcept(NO_EXCEPT) { return range_reference(this, p, p+c); }
    inline auto subseq(const size_type p) const noexcept(NO_EXCEPT) { return range_reference(this, p, this->size()); }


    struct range_reference : internal::range_reference<const compressed_wavelet_matrix> {
        range_reference(const compressed_wavelet_matrix *const super, const size_type l, const size_type r) noexcept(NO_EXCEPT)
          : internal::range_reference<const compressed_wavelet_matrix>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

      private:
        inline auto _range() const noexcept(NO_EXCEPT) { return this->_super->core::range(this->_begin, this->_end); }

      public:
        inline auto get(const size_type k) const noexcept(NO_EXCEPT) { return this->_super->_comp.value(this->_range().get(k)); }
        inline auto operator[](const size_type k) const noexcept(NO_EXCEPT) { return this->get(k); }


        inline auto kth_smallest(const size_type k) const noexcept(NO_EXCEPT) { return this->_super->_comp.value(this->_range().kth_smallest(k)); }
        inline auto kth_smallest_element(const size_type k) const noexcept(NO_EXCEPT) {
            return std::ranges::next(this->_super->begin(), std::ranges::distance(this->_super->core::begin(), this->_range().kth_smallest_element(k)));
        }

        inline auto kth_largest(const size_type k) const noexcept(NO_EXCEPT) { return this->_super->_comp.value(this->_range().kth_largest(k));}
        inline auto kth_largest_element(const size_type k) const noexcept(NO_EXCEPT) {
            return std::ranges::next(this->_super->begin(), std::ranges::distance(this->_super->core::begin(), this->_range().kth_largest_element(k)));
        }

        inline auto min() const noexcept(NO_EXCEPT) { return this->kth_smallest(0); }
        inline auto max() const noexcept(NO_EXCEPT) { return this->kth_largest(0); }


        // (r-l)/2 th smallest (0-origin)
        inline auto median() const noexcept(NO_EXCEPT) { return this->kth_smallest(this->size() / 2); }


        inline auto count_in_range(const value_type& x, const value_type& y) const noexcept(NO_EXCEPT) {
            return this->_range().count_in_range(this->_super->_comp.rank(x), this->_super->_comp.rank2(y));
        }

        inline size_type count_equal_to(const value_type& v) const noexcept(NO_EXCEPT) {
            const auto p = this->_super->_comp.rank(v);
            const auto q = this->_super->_comp.rank2(v);
            if(p != q) return 0;
            return this->_range().count_equal_to(p);
        }

        inline auto count_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_range().count_under(this->_super->_comp.rank(v)); }
        inline auto count_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_range().count_over(this->_super->_comp.rank2(v)); }
        inline auto count_or_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_range().count_or_under(this->_super->_comp.rank2(v)); }
        inline auto count_or_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_range().count_or_over(this->_super->_comp.rank(v)); }

        template<comparison com = comparison::equal_to>
        inline auto count(const value_type& v) const noexcept(NO_EXCEPT) {
            if constexpr(com == comparison::equal_to) return this->count_equal_to(v);
            if constexpr(com == comparison::under) return this->count_under(v);
            if constexpr(com == comparison::over) return this->count_over(v);
            if constexpr(com == comparison::or_under) return this->count_or_under(v);
            if constexpr(com == comparison::or_over) return this->count_or_over(v);
            assert(false);
        }


        inline auto next_element(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) {
            return this->kth_smallest_element(std::clamp(this->_range().count_under(this->_super->_comp.rank(v) + k), size_type{ 0 }, this->size()));
        }
        inline auto prev_element(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) {
            return this->kth_largest_element(std::clamp(this->_range().count_over(this->_super->_comp.rank2(v) + k), size_type{ 0 }, this->size()));
        }

        inline std::optional<value_type> next(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) {
            const auto res = this->_range().next(this->_super->_comp.rank(v), k);
            if(res.has_value()) return this->_super->_comp.value(res.value());
            return {};
        }
        inline std::optional<value_type> prev(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) {
            const auto res = this->_range().prev(this->_super->_comp.rank2(v), k);
            if(res.has_value()) return this->_super->_comp.value(res.value());
            return {};
        }
    };

    inline auto kth_smallest(const size_type k) const noexcept(NO_EXCEPT) { return this->range().kth_smallest(k); }
    inline auto kth_smallest_element(const size_type k) const noexcept(NO_EXCEPT) { return this->range().kth_smallest_element(k); }

    inline auto kth_largest(const size_type k) const noexcept(NO_EXCEPT) { return this->range().kth_largest(k); }
    inline auto kth_largest_element(const size_type k) const noexcept(NO_EXCEPT) { return this->range().kth_largest_element(k); }

    inline auto min() const noexcept(NO_EXCEPT) { return this->range().kth_smallest(0); }
    inline auto max() const noexcept(NO_EXCEPT) { return this->range().kth_largest(0); }

    inline auto median() const noexcept(NO_EXCEPT) { return this->range().median(); }

    inline auto count_in_range(const value_type& x, const value_type& y) const noexcept(NO_EXCEPT) { return this->range().count_in_range(x, y); }
    inline auto count_equal_to(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().count_equal_to(v); }
    inline auto count_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().count_under(v); }
    inline auto count_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().count_over(v); }
    inline auto count_or_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().count_or_under(v); }
    inline auto count_or_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().count_or_over(v); }

    template<comparison com = comparison::equal_to>
    inline auto count(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().template count<com>(v); }

    inline auto next_element(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().next_element(v); }
    inline auto prev_element(const value_type& v) const noexcept(NO_EXCEPT) { return this->range().prev_element(v); }

    inline auto next(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) { return this->range().next(v, k); }
    inline auto prev(const value_type& v, const size_type k = 0) const noexcept(NO_EXCEPT) { return this->range().prev(v, k); }

    struct iterator;

  protected:
    using iterator_interface = internal::container_iterator_interface<value_type, const compressed_wavelet_matrix, const iterator>;

  public:
    struct iterator : iterator_interface {
        iterator() noexcept = default;
        iterator(const compressed_wavelet_matrix *const ref, const size_type pos) noexcept(NO_EXCEPT) : iterator_interface(ref, pos) {}

        inline auto operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
        inline auto operator[](const typename iterator_interface::difference_type count) const noexcept(NO_EXCEPT) { return *(*this + count); }
    };

    inline auto begin() const noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline auto end() const noexcept(NO_EXCEPT) { return iterator(this, this->size()); }
};


template<std::ranges::input_range R>
explicit wavelet_matrix(R&&) -> wavelet_matrix<std::ranges::range_value_t<R>>;

template<std::input_iterator I, std::sentinel_for<I> S>
explicit wavelet_matrix(I, S) -> wavelet_matrix<std::iter_value_t<I>>;


template<std::ranges::input_range R>
explicit compressed_wavelet_matrix(R&&) -> compressed_wavelet_matrix<std::ranges::range_value_t<R>>;

template<std::input_iterator I, std::sentinel_for<I> S>
explicit compressed_wavelet_matrix(I, S) -> compressed_wavelet_matrix<std::iter_value_t<I>>;


} // namespace uni
