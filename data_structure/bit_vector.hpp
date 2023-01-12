#pragma once


#include <immintrin.h>
#include <cstdint>
#include <vector>
#include <iterator>

#include "internal/types.hpp"
#include "internal/iterator.hpp"


namespace lib {


// Thanks to: https://github.com/NyaanNyaan/library/blob/master/data-structure-2d/wavelet-matrix.hpp
struct bit_vector {
    using size_type = internal::size_t;

  private:
    static constexpr size_type w = 64;
    std::vector<std::uint64_t> _block;
    std::vector<size_type> _count;
    size_type _n, _zeros;

  public:
    bit_vector(const size_type _n = 0) { this->init(_n); }

    template<class I> explicit bit_vector(const I first, const I last) : bit_vector(std::distance(first, last)) {
        size_type pos = 0;
        for(auto itr=first; itr != last; ++pos, ++itr) if(*itr) this->set(pos);
    }
    template<class T> bit_vector(const std::initializer_list<T>& init_list) : bit_vector(std::begin(init_list), std::end(init_list)) {}

    inline size_type size() const { return this->_n; }

    inline size_type zeros() const { return this->_zeros; }
    inline size_type ones() const { return this->_n - this->_zeros; }

    inline void set(const size_type k) { this->_block[k / w] |= 1LL << (k % w); }
    inline bool get(const size_type k) const { return std::uint32_t(this->_block[k / w] >> (k % w)) & 1U; }

    __attribute__((optimize("O3", "unroll-loops"))) void init(const int _n) {
        this->_n = this->_zeros = _n;
        this->_block.resize(_n / w + 1, 0);
        this->_count.resize(this->_block.size(), 0);
    }

    __attribute__((target("popcnt"))) void build() {
        for(auto k = 1UL; k < this->_block.size(); ++k) this->_count[k] = this->_count[k-1] + _mm_popcnt_u64(this->_block[k-1]);
        this->_zeros = this->rank0(this->_n);
    }

    __attribute__((target("bmi2,popcnt"))) inline size_type rank1(const size_type k) const {
        return this->_count[k / w] + _mm_popcnt_u64(_bzhi_u64(this->_block[k / w], k % w));
    }
    inline size_type rank0(size_type k) const { return k - this->rank1(k); }

    inline size_type rank(const bool bit, const size_type k) const {
        return bit ? this->rank1(k) : this->rank0(k);
    }

    __attribute__((target("bmi2,popcnt"))) inline size_type select(const bool bit, const size_type rank) const {
        if (!bit and rank > this->zeros()) { return this->_n + 1; }
        if (bit and rank > this->ones()) { return this->_n + 1; }

        size_type block_pos = 0;
        {
            size_type ng = -1, ok = this->_count.size();
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
            size_type r = this->_count[block_pos - 1] + _mm_popcnt_u64(_bzhi_u64(block, mid));
            if(!bit) r = base_index + mid - r;
            (r >= rank ? ok : ng) = mid;
        }

        return base_index + ok;
    }

    inline size_type select0(const size_type k) const { return this->select(0, k); }
    inline size_type select1(const size_type k) const { return this->select(1, k); }


    struct iterator : virtual internal::container_iterator_interface<bool,bit_vector> {
        iterator(const bit_vector *const ref, const size_type pos) : internal::container_iterator_interface<bool,bit_vector>(ref, pos) {}

        inline bool operator*() const override { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


} // namespace lib
