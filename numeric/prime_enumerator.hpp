#pragma once


#include <cmath>
#include <cstdint>
#include <vector>
#include <valarray>

#include "snippet/iterations.hpp"

#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/endian.hpp"

#include "numeric/arithmetic.hpp"
#include "numeric/bit.hpp"


namespace lib {


// Thanks to: https://qiita.com/peria/items/e0ab38f95d16a5f7cc58
template<class T>
struct prime_enumerator {
    using value_type = T;
    using size_type = std::int32_t;
    using impl_type = std::uint32_t;

    impl_type n = 0;

  protected:
    using uint8_t = std::uint8_t;
    using uint16_t = std::uint16_t;
    using int32_t = std::int32_t;
    using uint32_t = std::uint32_t;
    using int64_t = std::int64_t;
    using uint64_t = std::uint64_t;

    mutable size_type _size = -1;

    impl_type _sqrt_n = 0, _sqrt_ni = 0, _quart_n = 0;

    std::valarray<uint8_t> _small;
    std::valarray<uint64_t> _large;
    std::vector<uint32_t> _indecies;

    uint8_t* _flag = nullptr;

    static constexpr impl_type SEGMENT_SIZE = 1'000'000;
    static constexpr impl_type  MOD30[] = { 1, 7, 11, 13, 17, 19, 23, 29 };
    static constexpr uint8_t MASK[][8] = {
        { 0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f },
        { 0xfd, 0xdf, 0xef, 0xfe, 0x7f, 0xf7, 0xfb, 0xbf },
        { 0xfb, 0xef, 0xfe, 0xbf, 0xfd, 0x7f, 0xf7, 0xdf },
        { 0xf7, 0xfe, 0xbf, 0xdf, 0xfb, 0xfd, 0x7f, 0xef },
        { 0xef, 0x7f, 0xfd, 0xfb, 0xdf, 0xbf, 0xfe, 0xf7 },
        { 0xdf, 0xf7, 0x7f, 0xfd, 0xbf, 0xfe, 0xef, 0xfb },
        { 0xbf, 0xfb, 0xf7, 0x7f, 0xfe, 0xef, 0xdf, 0xfd },
        { 0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe },
    };
    static constexpr impl_type C0[][8] = {
        { 0, 0, 0, 0, 0, 0, 0, 1 }, { 1, 1, 1, 0, 1, 1, 1, 1 },
        { 2, 2, 0, 2, 0, 2, 2, 1 }, { 3, 1, 1, 2, 1, 1, 3, 1 },
        { 3, 3, 1, 2, 1, 3, 3, 1 }, { 4, 2, 2, 2, 2, 2, 4, 1 },
        { 5, 3, 1, 4, 1, 3, 5, 1 }, { 6, 4, 2, 4, 2, 4, 6, 1 },
    };
    static constexpr uint32_t C1[] = { 6, 4, 2, 4, 2, 4, 6, 2 };

    void _init(const value_type n, const impl_type size) noexcept(NO_EXCEPT) {
        const size_type blocks = size >> 3;
        this->_large.resize(blocks + 2, ~0UL);
        this->_flag = reinterpret_cast<uint8_t*>(&(this->_large[0]));

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

        this->_large[blocks] <<= 64 - ((size & 7) << 3);
        this->_large[blocks] >>= 64 - ((size & 7) << 3);
        this->_large[blocks + 1] = 0;
    }

    void _gen_small() noexcept(NO_EXCEPT) {
        const uint32_t quart_ni = this->_quart_n / 30 + 1;

        this->_small.resize(this->_sqrt_ni, 0xff);
        this->_small[0] = 0xfe;

        for(uint32_t i=0; i<quart_ni; ++i) {
            for(uint8_t flags = this->_small[i]; flags; flags &= flags - 1) {
                const int ibit = lowest_bit_pos(flags);
                const uint32_t m = prime_enumerator::MOD30[ibit];
                const uint32_t pm = 30 * i + 2 * m;
                uint32_t j = i * pm + (m * m) / 30;
                uint32_t k = ibit;
                while(j < this->_sqrt_ni) {
                    this->_small[j] &= prime_enumerator::MASK[ibit][k];
                    j += i * prime_enumerator::C1[k] + prime_enumerator::C0[ibit][k];
                    k = (k + 1) & 7;
                }
            }
        }

        for(impl_type i = 0; i < this->_sqrt_ni; ++i) {
            for(uint8_t flags = this->_small[i]; flags; flags &= flags - 1) {
                int ibit = lowest_bit_pos(flags);
                const int32_t m = prime_enumerator::MOD30[ibit];
                impl_type j = i * (30 * i + 2 * m) + (m * m) / 30;
                this->_indecies.emplace_back(((j + prime_enumerator::SEGMENT_SIZE) << 3) | ibit);
            }
        }
    }

    void _gen_core(uint8_t *const flags, const impl_type size) noexcept(NO_EXCEPT) {
        auto p_index = this->_indecies.begin();
        for(impl_type i=0; i<this->_sqrt_ni; ++i) {
            for(uint8_t primes = this->_small[i]; primes; primes &= primes - 1) {
                const int ibit = lowest_bit_pos(primes);
                impl_type j = (*p_index >> 3) - prime_enumerator::SEGMENT_SIZE;
                impl_type k = *p_index & 7;
                while(j < size) {
                    flags[j] &= prime_enumerator::MASK[ibit][k];
                    j += i * prime_enumerator::C1[k] + prime_enumerator::C0[ibit][k];
                    k = (k + 1) & 7;
                }
                *p_index = ((j << 3) | k);
                ++p_index;
            }
        }
    }

  protected:
    using iterator_interface = internal::bidirectional_iterator_interface<const value_type>;

  public:
    struct iterator;
    using const_iterator = iterator;

    prime_enumerator(const value_type n) noexcept(NO_EXCEPT) : n(n) {
        assert(n >= 0);
        assert(internal::discern_endian() == internal::endian::little);

        this->_sqrt_n = static_cast<uint32_t>(sqrt_ceil(this->n + 1));
        this->_sqrt_ni = this->_sqrt_n / 30 + 1;
        this->_quart_n = static_cast<uint32_t>(sqrt_ceil(this->_sqrt_n));

        this->_gen_small();

        impl_type size = this->n / 30 + 1;
        this->_init(this->n + 1, size);

        for(uint8_t* seg = this->_flag; ; seg += prime_enumerator::SEGMENT_SIZE) {
            if(size < prime_enumerator::SEGMENT_SIZE) {
                this->_gen_core(seg, size);
                break;
            };
            this->_gen_core(seg, prime_enumerator::SEGMENT_SIZE);
            size -= prime_enumerator::SEGMENT_SIZE;
        }
    }

    size_type size() const noexcept(NO_EXCEPT) {
        if(this->_size < 0) {
            this->_size = (this->n >= 2) + (this->n >= 3) + (this->n >= 5);
            for(const uint64_t f : this->_large) this->_size += popcount(f);
        }
        return this->_size;
    }

    inline bool is_prime(const T v) const noexcept(NO_EXCEPT) {
        assert(0 <= v and v <= this->n);

        if(v == 0 or v == 1) return false;
        if(v == 2 or v == 3 or v == 5) return true;
        if((v & 1) == 0 or v % 3 == 0 or v % 5 == 0) return false;

        REP(i, 8) {
            if(prime_enumerator::MOD30[i] == v % 30) {
                return static_cast<bool>((this->_flag[v / 30] >> i) & 1);
            }
        }

        assert(false);
    }

    inline iterator begin() const noexcept(NO_EXCEPT) { return iterator(this->n, &this->_large, 0); }
    inline iterator end() const noexcept(NO_EXCEPT) { return iterator(this->n, &this->_large, -1); }

    inline auto rbegin() noexcept(NO_EXCEPT) { return std::make_reverse_iterator(this->end()); }
    inline auto rend() noexcept(NO_EXCEPT) { return std::make_reverse_iterator(this->begin()); }

    struct iterator : virtual iterator_interface {
        using reference = value_type;

      protected:
        const value_type n = 0;
        const std::valarray<uint64_t> *const _flags = nullptr;
        size_type _block = -1, _flag_size;
        int _bit = 0;

        inline value_type _value() const noexcept(NO_EXCEPT) {
            if(this->_bit < 0) return std::numeric_limits<value_type>::max();
            if(this->_block < 0) {
                if(this->_bit == 0) return 2;
                if(this->_bit == 1) return 3;
                if(this->_bit == 2) return 5;
            }
            return (
                value_type{30} * ((this->_block << 3) + (this->_bit >> 3)) +
                prime_enumerator::MOD30[this->_bit & 7]
            );
        }

      public:
        iterator() {}
        iterator(const value_type n, const std::valarray<uint64_t> *const flags, const int bit) noexcept(NO_EXCEPT)
          : n(n), _flags(flags), _flag_size(static_cast<size_type>(flags->size())), _bit(bit) {
            if(bit < 0) {
                this->_block = static_cast<size_type>(flags->size()) - 1;
            }
        }

        friend inline bool operator==(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT) { return *lhs == *rhs; };
        friend inline bool operator!=(const iterator& lhs, const iterator& rhs) noexcept(NO_EXCEPT) { return *lhs != *rhs; };

        inline value_type operator*() const noexcept(NO_EXCEPT) {
            const value_type res = this->_value();
            return res > this->n ? -1 : res;
        }

        inline iterator& operator++() noexcept(NO_EXCEPT) {
            if(this->_value() > this->n) return *this;

            if(this->_block < 0) {
                this->_bit++;
                if(this->_bit > 2) this->_block = 0, this->_bit = 1;
                return *this;
            }

            int next;
            while(true) {
                const uint64_t mask = this->_bit >= 63 ? 0UL : ~((1UL << (this->_bit + 1)) - 1);
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
                if(this->_bit > 0) this->_bit--;
                return *this;
            }

            int prev = -1;
            while(true) {
                if(0 < this->_bit) {
                    const uint64_t mask = this->_bit >= 64 ? ~0UL : ((1UL << this->_bit) - 1);
                    prev = highest_bit_pos(this->_flags->operator[](this->_block) & mask);
                }
                if(prev >= 0) break;
                this->_block--;
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
    };
};


} // namespace lib
