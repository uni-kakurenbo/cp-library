#pragma once


#include "internal/types.hpp"

#include <vector>
#include <cstdint>

#include <immintrin.h>


namespace lib {


// Thanks to: https://github.com/NyaanNyaan/library/blob/master/data-structure-2d/wavelet-matrix.hpp
struct succinct_bit_vector {
    using size_type = internal::size_t;

  private:
    using uint32 = std::uint32_t;
    using int64 = std::int64_t;
    using uint64 = std::uint64_t;

    static constexpr uint32 w = 64;
    std::vector<uint64> _block;
    std::vector<uint32> _count;
    size_type _n, _zeros;

  public:
    succinct_bit_vector() {}
    succinct_bit_vector(const size_type _n) { this->init(_n); }

    inline size_type size() const { return this->_n; }
    inline size_type zeros() const { return this->_zeros; }

    inline void set(size_type i) { this->_block[i / w] |= 1LL << (i % w); }
    inline bool get(size_type i) const { return uint32(this->_block[i / w] >> (i % w)) & 1U; }

    __attribute__((optimize("O3", "unroll-loops"))) void init(const int _n) {
        this->_n = this->_zeros = _n;
        this->_block.resize(_n / w + 1, 0);
        this->_count.resize(this->_block.size(), 0);
    }

    __attribute__((target("popcnt"))) void build() {
        for(auto i=1UL; i < this->_block.size(); ++i) this->_count[i] = this->_count[i - 1] + _mm_popcnt_u64(this->_block[i - 1]);
        this->_zeros = this->rank0(this->_n);
    }

    __attribute__((target("bmi2,popcnt"))) inline size_type rank1(size_type i) const {
        return this->_count[i / w] + _mm_popcnt_u64(_bzhi_u64(this->_block[i / w], i % w));
    }

    inline size_type rank0(size_type i) const { return i - this->rank1(i); }
};


} // namespace lib
