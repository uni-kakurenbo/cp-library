#pragma once


#include <cmath>
#include <cstdint>
#include <vector>
#include <valarray>
#include <ranges>
#include <bit>


#include "internal/dev_env.hpp"

#include "snippet/iterations.hpp"

#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "numeric/arithmetic.hpp"
#include "numeric/bit.hpp"
#include "numeric/limits.hpp"


namespace lib {

namespace internal {

namespace prime_enumerator_impl {


using impl_type = std::uint32_t;
using small_bit_type = std::uint8_t;
using large_bit_type = std::uint64_t;


inline static constexpr impl_type SEGMENT_SIZE = 1'000'000;

inline static constexpr impl_type  MOD30[8] = { 1, 7, 11, 13, 17, 19, 23, 29 };

inline static constexpr small_bit_type MASK[8][8] = {
    { 0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f },
    { 0xfd, 0xdf, 0xef, 0xfe, 0x7f, 0xf7, 0xfb, 0xbf },
    { 0xfb, 0xef, 0xfe, 0xbf, 0xfd, 0x7f, 0xf7, 0xdf },
    { 0xf7, 0xfe, 0xbf, 0xdf, 0xfb, 0xfd, 0x7f, 0xef },
    { 0xef, 0x7f, 0xfd, 0xfb, 0xdf, 0xbf, 0xfe, 0xf7 },
    { 0xdf, 0xf7, 0x7f, 0xfd, 0xbf, 0xfe, 0xef, 0xfb },
    { 0xbf, 0xfb, 0xf7, 0x7f, 0xfe, 0xef, 0xdf, 0xfd },
    { 0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe },
};

inline static constexpr impl_type C1[8] = { 6, 4, 2, 4, 2, 4, 6, 2 };
inline static constexpr impl_type C0[8][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 1 }, { 1, 1, 1, 0, 1, 1, 1, 1 },
    { 2, 2, 0, 2, 0, 2, 2, 1 }, { 3, 1, 1, 2, 1, 1, 3, 1 },
    { 3, 3, 1, 2, 1, 3, 3, 1 }, { 4, 2, 2, 2, 2, 2, 4, 1 },
    { 5, 3, 1, 4, 1, 3, 5, 1 }, { 6, 4, 2, 4, 2, 4, 6, 1 },
};

} // namespace prime_enumerator_impl

} // namespace internal


// Thanks to: https://qiita.com/peria/items/e0ab38f95d16a5f7cc58

template<class T>
struct prime_enumerator : std::ranges::view_interface<prime_enumerator<T>> {
    using value_type = T;
    using size_type = internal::size_t;
    using impl_type = internal::prime_enumerator_impl::impl_type;

    impl_type n = 0;

  protected:
    using small_bit_type = internal::prime_enumerator_impl::small_bit_type;
    using large_bit_type = internal::prime_enumerator_impl::large_bit_type;

    size_type _size;

    impl_type _sqrt_n = 0, _sqrt_ni = 0, _quart_n = 0;

    std::valarray<small_bit_type> _small;
    std::valarray<large_bit_type> _large;
    std::vector<impl_type> _indecies;

    small_bit_type* _flag = nullptr;

    inline void _init(const value_type n, const impl_type size) noexcept(NO_EXCEPT) {
        const size_type blocks = (size + 7) >> 3;
        this->_large.resize(blocks + 1, ~0UL);
        this->_flag = reinterpret_cast<small_bit_type*>(&(this->_large[0]));

        this->_flag[0] = 0xfe;

        value_type r = n % 30;
        if(r <= 1) this->_flag[size - 1] = 0x0;
        else if(r <= 7) this->_flag[size - 1] = 0x1;
        else if(r <= 11) this->_flag[size - 1] = 0x3;
        else if(r <= 13) this->_flag[size - 1] = 0x7;
        else if(r <= 17) this->_flag[size - 1] = 0xf;
        else if(r <= 19) this->_flag[size - 1] = 0x1f;
        else if(r <= 23) this->_flag[size - 1] = 0x3f;
        else if(r <= 29) this->_flag[size - 1] = 0x7f;

        if(n < 30) this->_flag[0] &= 0xfe;

        this->_large[blocks - 1] <<= 64 - ((size & 7) << 3);
        this->_large[blocks - 1] >>= 64 - ((size & 7) << 3);
        this->_large[blocks] = 0;

        // for(auto x : this->_large) debug(std::bitset<64>(x));
    }

    inline void _gen_small() noexcept(NO_EXCEPT) {
        const impl_type quart_ni = this->_quart_n / 30 + 1;

        this->_small.resize(this->_sqrt_ni, 0xff);
        this->_small[0] = 0xfe;

        REP(i, quart_ni) {
            for(small_bit_type flags = this->_small[i]; flags; flags &= flags - 1) {
                const int ibit = lowest_bit_pos(flags);
                const impl_type m = internal::prime_enumerator_impl::MOD30[ibit];
                const impl_type pm = 30 * i + 2 * m;
                for(
                    impl_type j = i * pm + (m * m) / 30, k = ibit;
                    j < this->_sqrt_ni;
                    j += i * internal::prime_enumerator_impl::C1[k] + internal::prime_enumerator_impl::C0[ibit][k], k = (k + 1) & 7
                ) {
                    this->_small[j] &= internal::prime_enumerator_impl::MASK[ibit][k];
                }
            }
        }

        REP(i, this->_sqrt_ni) {
            for(small_bit_type flags = this->_small[i]; flags; flags &= flags - 1) {
                const int ibit = lowest_bit_pos(flags);
                const impl_type m = internal::prime_enumerator_impl::MOD30[ibit];
                impl_type j = i * (30 * i + 2 * m) + (m * m) / 30;
                this->_indecies.emplace_back(((j + internal::prime_enumerator_impl::SEGMENT_SIZE) << 3) | ibit);
            }
        }
    }

    inline void _gen_core(small_bit_type *const flags, const impl_type size) noexcept(NO_EXCEPT) {
        auto p_index = this->_indecies.begin();
        REP(i, this->_sqrt_ni) {
            for(small_bit_type primes = this->_small[i]; primes; primes &= primes - 1) {
                const int ibit = lowest_bit_pos(primes);
                impl_type j = (*p_index >> 3) - internal::prime_enumerator_impl::SEGMENT_SIZE;
                impl_type k = *p_index & 7;
                for(; j < size; j += i * internal::prime_enumerator_impl::C1[k] + internal::prime_enumerator_impl::C0[ibit][k], k = (k + 1) & 7) {
                    flags[j] &= internal::prime_enumerator_impl::MASK[ibit][k];
                }
                *p_index = ((j << 3) | k);
                ++p_index;
            }
        }
    }

    using iterator_interface = internal::bidirectional_iterator_interface<const value_type>;

  public:
    struct iterator;
    using const_iterator = iterator;

    prime_enumerator() noexcept {};

    prime_enumerator(const value_type n) noexcept(NO_EXCEPT) : n(n) {
        assert(n >= 0);
        assert(std::endian::native == std::endian::little);

        this->_sqrt_n = static_cast<impl_type>(sqrt_ceil(this->n + 1));
        this->_sqrt_ni = this->_sqrt_n / 30 + 1;
        this->_quart_n = static_cast<impl_type>(sqrt_ceil(this->_sqrt_n));

        this->_gen_small();

        impl_type size = (this->n + 1) / 30 + 1;
        this->_init(this->n + 1, size);

        for(small_bit_type* seg = this->_flag; ; seg += internal::prime_enumerator_impl::SEGMENT_SIZE) {
            if(size < internal::prime_enumerator_impl::SEGMENT_SIZE) {
                this->_gen_core(seg, size);
                break;
            };
            this->_gen_core(seg, internal::prime_enumerator_impl::SEGMENT_SIZE);
            size -= internal::prime_enumerator_impl::SEGMENT_SIZE;
        }

        this->_size = (this->n >= 2) + (this->n >= 3) + (this->n >= 5);
        for(const large_bit_type f : this->_large) this->_size += std::popcount(f);
    }

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_size; }

    inline size_type count(const T x) const noexcept(NO_EXCEPT) {
        assert(0 <= x and x <= this->n);

        size_type res = (x >= 2) + (x >= 3) + (x >= 5);
        T count = 0;

        constexpr int large_bit_width = std::numeric_limits<large_bit_type>::digits;
        constexpr int large_bit_size = (30 * large_bit_width) >> 3;

        for(const large_bit_type f : this->_large) {
            if(count + large_bit_size > x) {
                REP(i, large_bit_width) {
                    if(count + internal::prime_enumerator_impl::MOD30[i & 7] > x) break;
                    if((f >> i) & 1) res++;
                    if(((i + 1) & 7) == 0) count += 30;
                }
                break;
            }
            res += std::popcount(f);
            count += large_bit_size;
        }

        return res;
    }

    inline size_type count(const T l, const T r) const noexcept(NO_EXCEPT) {
        assert(l <= r);
        return this->count(r) - this->count(l - 1);
    }

    inline bool is_prime(const T v) const noexcept(NO_EXCEPT) {
        assert(0 <= v and v <= this->n);

        if(v == 0 or v == 1) return false;
        if(v == 2 or v == 3 or v == 5) return true;
        if((v & 1) == 0 or v % 3 == 0 or v % 5 == 0) return false;

        REP(i, 8) {
            if(internal::prime_enumerator_impl::MOD30[i] == v % 30) {
                return static_cast<bool>((this->_flag[v / 30] >> i) & 1);
            }
        }

        assert(false);
    }

    inline iterator begin() const noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline iterator end() const noexcept(NO_EXCEPT) { return iterator(this, -1); }

    inline auto rbegin() const noexcept(NO_EXCEPT) { return std::make_reverse_iterator(this->end()); }
    inline auto rend() const noexcept(NO_EXCEPT) { return std::make_reverse_iterator(this->begin()); }

    struct iterator : virtual iterator_interface {
      protected:
        value_type n = 0;
        size_type _index = 0;
        const std::valarray<large_bit_type>* _flags = nullptr;
        size_type _block = -1, _flag_size;
        int _bit = 0;

        inline value_type _value() const noexcept(NO_EXCEPT) {
            if(this->_bit < 0) return numeric_limits<value_type>::arithmetic_infinity();
            if(this->_block < 0) {
                if(this->_bit == 0) return 2;
                if(this->_bit == 1) return 3;
                if(this->_bit == 2) return 5;
            }
            return (
                value_type{30} * ((this->_block << 3) + (this->_bit >> 3)) +
                internal::prime_enumerator_impl::MOD30[this->_bit & 7]
            );
        }

      public:
        iterator() noexcept = default;
        iterator(const prime_enumerator *const super, const int bit) noexcept(NO_EXCEPT)
          : n(super->n), _flags(&super->_large), _flag_size(static_cast<size_type>(super->_large.size())), _bit(bit) {
            if(bit < 0) {
                this->_index = super->size();
                this->_block = static_cast<size_type>(super->_large.size()) - 1;
            }
        }

        inline size_type index() const noexcept(NO_EXCEPT) { return this->_index; }

        inline value_type operator*() const noexcept(NO_EXCEPT) {
            const value_type res = this->_value();
            return (res > this->n ? -1 : res);
        }

        inline iterator& operator++() noexcept(NO_EXCEPT) {
            if(this->_value() > this->n) return *this;
            ++this->_index;

            if(this->_block < 0) {
                ++this->_bit;
                if(this->_bit > 2) this->_block = 0, this->_bit = 1;
                return *this;
            }

            int next;
            while(true) {
                const large_bit_type mask = this->_bit >= 63 ? 0UL : ~((1UL << (this->_bit + 1)) - 1);
                next = lowest_bit_pos(this->_flags->operator[](this->_block) & mask);
                if(next >= 0) break;
                this->_block++;
                this->_bit = -1;
                if(this->_block >= this->_flag_size) break;
            }
            this->_bit = next;

            return *this;
        }

        inline iterator& operator--() noexcept(NO_EXCEPT) {
            if(this->_block < 0) {
                if(this->_bit > 0) --this->_bit, --this->_index;
                return *this;
            }
            --this->_index;

            int prev = -1;
            while(true) {
                if(0 < this->_bit) {
                    const large_bit_type mask = this->_bit >= 64 ? ~0UL : ((1UL << this->_bit) - 1);
                    prev = highest_bit_pos(this->_flags->operator[](this->_block) & mask);
                }
                if(prev >= 0) break;
                --this->_block;
                this->_bit = 64;
                if(this->_block < 0) {
                    this->_bit = (this->n >= 3) + (this->n >= 5);
                    return *this;
                }
            }

            this->_bit = prev;

            return *this;
        }

        inline iterator operator++(int) noexcept(NO_EXCEPT) { const auto res = *this; ++(*this); return res; }
        inline iterator operator--(int) noexcept(NO_EXCEPT) { const auto res = *this; --(*this); return res; }

        friend inline bool operator==(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT) { return lhs._index == rhs._index; }
        friend inline bool operator!=(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT) { return lhs._index != rhs._index; }
        friend inline bool operator<(const iterator& lhs, const iterator& rhs) { return lhs._index < rhs._index; }
        friend inline bool operator>(const iterator& lhs, const iterator& rhs) { return lhs._index > rhs._index; }
        friend inline size_type operator-(const iterator& lhs, const iterator& rhs) { return lhs._index - rhs._index; }
    };
};


} // namespace lib


namespace std::ranges {

template<class T>
inline constexpr bool enable_borrowed_range<lib::prime_enumerator<T>> = true;

} // namespace std::ranges
