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


namespace lib {


// Thanks to: https://github.com/NyaanNyaan/library/blob/master/data-structure-2d/wavelet-matrix.hpp
struct bit_vector {
    using size_type = std::uint_fast32_t;

  private:
    static constexpr size_type w = 64;
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

    template<class T> bit_vector(const std::initializer_list<T>& init_list) noexcept(NO_EXCEPT)
      : bit_vector(std::ranges::begin(init_list), std::ranges::end(init_list))
    {}

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n; }

    inline size_type zeros() const noexcept(NO_EXCEPT) { return this->_zeros; }
    inline size_type ones() const noexcept(NO_EXCEPT) { return this->_n - this->_zeros; }

    inline void set(const size_type k) noexcept(NO_EXCEPT) { this->_block[k / w] |= (1LL << (k % w)); }
    inline bool get(const size_type k) const noexcept(NO_EXCEPT) { return static_cast<std::uint32_t>(this->_block[k / w] >> (k % w)) & 1U; }

    __attribute__((optimize("O3", "unroll-loops")))
    inline void init(const size_type n) noexcept(NO_EXCEPT) {
        this->_n = this->_zeros = n;
        this->_block.resize(this->_n / w + 1, 0);
        this->_count.resize(this->_block.size(), 0);
    }

    inline void build() noexcept(NO_EXCEPT) {
        for(auto k = 1UL; k < this->_block.size(); ++k) {
            this->_count[k] = this->_count[k-1] + static_cast<size_type>(std::popcount(this->_block[k-1]));
        }
        this->_zeros = this->rank0(this->_n);
    }

    inline size_type rank1(const size_type k) const noexcept(NO_EXCEPT) {
        return this->_count[k / w] + static_cast<size_type>(std::popcount(lib::clear_higher_bits(this->_block[k / w], k % w)));
    }
    inline size_type rank0(size_type k) const noexcept(NO_EXCEPT) { return k - this->rank1(k); }

    inline size_type rank(const bool bit, const size_type k) const noexcept(NO_EXCEPT) {
        return bit ? this->rank1(k) : this->rank0(k);
    }

    inline size_type select(const bool bit, const size_type rank) const noexcept(NO_EXCEPT) {
        if (!bit and rank > this->zeros()) { return this->_n + 1; }
        if (bit and rank > this->ones()) { return this->_n + 1; }

        size_type block_pos = 0;
        {
            size_type ng = -1, ok = static_cast<size_type>(this->_count.size());
            while(ok - ng > 1) {
                size_type mid = (ng + ok) / 2;

                size_type cnt = this->_count[mid];
                if(!bit) cnt = mid * w - cnt;

                (cnt >= rank ? ok : ng) = mid;
            }
            block_pos = ok;
        }

        const size_type base_index = (block_pos - 1) * w;
        const size_type count = this->_count[block_pos - 1];
        const std::uint64_t block = this->_block[block_pos - 1];

        size_type ng = -1, ok = w;
        while(ok - ng > 1) {
            const size_type mid = (ok + ng) / 2;
            size_type r = count + static_cast<size_type>(std::popcount(lib::clear_higher_bits(block, mid)));
            if(!bit) r = base_index + mid - r;
            (r >= rank ? ok : ng) = mid;
        }

        return base_index + ok;
    }

    inline size_type select0(const size_type k) const noexcept(NO_EXCEPT) { return this->select(0, k); }
    inline size_type select1(const size_type k) const noexcept(NO_EXCEPT) { return this->select(1, k); }

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


} // namespace lib
