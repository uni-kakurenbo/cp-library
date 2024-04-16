#pragma once


#include <cstdint>
#include <vector>
#include <iterator>
#include <ranges>
#include <bit>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "numeric/bit.hpp"


namespace uni {


// Thanks to: https://github.com/NyaanNyaan/library/blob/master/data-structure-2d/wavelet-matrix.hpp
struct bit_vector {
    using size_type = std::uint_fast32_t;

  private:
    static constexpr size_type WORDSIZE = 64;
    std::vector<std::uint64_t> _block;
    std::vector<size_type> _count;
    size_type _n, _zeros;

  public:
    bit_vector(const size_type n = 0) noexcept(NO_EXCEPT) { this->init(n); }

    template<std::input_iterator I, std::sentinel_for<I> S>
    bit_vector(I first, S last) noexcept(NO_EXCEPT)
      : bit_vector(std::ranges::distance(first, last))
    {
        size_type pos = 0;
        for(auto itr=first; itr != last; ++pos, ++itr) if(*itr) this->set(pos);
    }

    template<std::ranges::input_range R>
    bit_vector(R&& range) noexcept(NO_EXCEPT) : bit_vector(ALL(range)) {}

    template<class T> bit_vector(const std::initializer_list<T>& init_list) noexcept(NO_EXCEPT)
      : bit_vector(ALL(init_list))
    {}

    inline constexpr size_type size() const noexcept(NO_EXCEPT) { return this->_n; }

    inline constexpr size_type zeros() const noexcept(NO_EXCEPT) { return this->_zeros; }
    inline constexpr size_type ones() const noexcept(NO_EXCEPT) { return this->_n - this->_zeros; }

    inline void set(const size_type k) noexcept(NO_EXCEPT) { this->_block[k / WORDSIZE] |= (1LL << (k % WORDSIZE)); }
    inline bool get(const size_type k) const noexcept(NO_EXCEPT) {
        return 1U & static_cast<std::uint32_t>(this->_block[k / WORDSIZE] >> (k % WORDSIZE));
    }

    __attribute__((optimize("O3", "unroll-loops")))
    inline void init(const size_type n) noexcept(NO_EXCEPT) {
        this->_n = this->_zeros = n;
        this->_block.resize(this->_n / WORDSIZE + 1, 0);
        this->_count.resize(this->_block.size(), 0);
    }

    inline void build() noexcept(NO_EXCEPT) {
        for(auto k = 1UL; k < this->_block.size(); ++k) {
            this->_count[k] = this->_count[k-1] + static_cast<size_type>(std::popcount(this->_block[k-1]));
        }
        this->_zeros = this->rank0(this->_n);
    }


    inline size_type rank1(const size_type k) const noexcept(NO_EXCEPT) {
        return
            this->_count[k / WORDSIZE] +
            static_cast<size_type>(std::popcount(uni::clear_higher_bits(this->_block[k / WORDSIZE], k % WORDSIZE)));
    }

    inline size_type rank0(const size_type k) const noexcept(NO_EXCEPT) { return k - this->rank1(k); }


    template<bool BIT>
    inline size_type rank(const size_type k) const noexcept(NO_EXCEPT) {
        if constexpr(BIT) return this->rank0(k);
        else return this->rank1(k);
    }

    template<bool BIT>
    inline size_type select(const size_type rank) const noexcept(NO_EXCEPT) {
        if constexpr(BIT) {
            if(rank >= this->ones()) return this->_n;
        }
        else {
            if(rank >= this->zeros()) return this->_n;
        }

        size_type index = 0;
        {
            size_type ng = static_cast<size_type>(this->_count.size());
            while(ng - index > 1) {
                size_type mid = (ng + index) / 2;

                size_type cnt = this->_count[mid];
                if constexpr(!BIT) cnt = mid * WORDSIZE - cnt;

                (cnt <= rank ? index : ng) = mid;
            }
        }

        const size_type base = index * WORDSIZE;

        if constexpr(BIT) {
            return base + select64(this->_block[index], rank - this->_count[index]);
        }
        else {
            return base + select64(~this->_block[index], rank - (base - this->_count[index]));
        }
    }


    inline size_type select0(const size_type k) const noexcept(NO_EXCEPT) { return this->select<false>(k); }
    inline size_type select1(const size_type k) const noexcept(NO_EXCEPT) { return this->select<true>(k); }

    struct iterator;

  private:
    using iterator_interface = internal::container_iterator_interface<bool, const bit_vector, iterator>;

  public:
    struct iterator : iterator_interface {
        iterator() noexcept = default;
        iterator(const bit_vector *const ref, const size_type pos) noexcept(NO_EXCEPT) : bit_vector::iterator_interface(ref, static_cast<difference_type>(pos)) {}

        inline bool operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline iterator end() const noexcept(NO_EXCEPT) { return iterator(this, this->size()); }
};


} // namespace uni
