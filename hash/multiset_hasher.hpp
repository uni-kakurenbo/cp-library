#pragma once


#include <iterator>


#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/arithmetic.hpp"

#include "hash/general_hasher.hpp"


namespace uni {


template<class T, std::uint64_t MOD = 0x1fffffffffffffff, int hasher_id = -1>
    requires (MOD < std::numeric_limits<std::make_signed_t<std::uint64_t>>::max())
struct multiset_hasher {
  private:
    using uint128_t = internal::uint128_t;

  public:
    using hash_type = std::uint64_t;
    using size_type = std::uint64_t;

    static constexpr hash_type mod = MOD;

  protected:
    static inline hash_type _id(const T& v) noexcept(NO_EXCEPT) {
        return uni::hash64(v);
    }

    static constexpr hash_type mask(const size_type a) noexcept(NO_EXCEPT) { return (1ULL << a) - 1; }

    static constexpr hash_type mul(hash_type a, hash_type b) noexcept(NO_EXCEPT) {
        #ifdef __SIZEOF_INT128__

        uint128_t res = static_cast<uint128_t>(a) * b;

        #else

        hash_type a31 = a >> 31, b31 = b >> 31;
        a &= multiset_hasher::mask(31);
        b &= multiset_hasher::mask(31);
        hash_type x = a * b31 + b * a31;
        hash_type res = (a31 * b31 << 1) + (x >> 30) + ((x & multiset_hasher::mask(30)) << 31) + a * b;

        #endif

        res = (res >> 61) + (res & multiset_hasher::mod);
        if(res >= multiset_hasher::mod) res -= multiset_hasher::mod;

        return res;
    }

    hash_type _hash = 0;

    inline void _add_hash(const hash_type h, const hash_type count) noexcept(NO_EXCEPT) {
        this->_hash += multiset_hasher::mul(h, count);
        if(this->_hash >= multiset_hasher::mod) this->_hash -= multiset_hasher::mod;
    }
    inline void _remove_hash(const hash_type h, const hash_type count) noexcept(NO_EXCEPT) {
        auto hash = to_signed(this->_hash);
        hash -= multiset_hasher::mul(h, count);
        if(hash < 0) hash += multiset_hasher::mod;
        this->_hash = hash;
    }

  public:
    multiset_hasher() noexcept(NO_EXCEPT) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    multiset_hasher(I first, S last) noexcept(NO_EXCEPT) : multiset_hasher() {
        for(auto itr=first; itr != last; ++itr) this->insert(*itr);
    }

    template<class U>
    multiset_hasher(const std::initializer_list<U>& init_list) noexcept(NO_EXCEPT) : multiset_hasher(std::begin(init_list), std::end(init_list)) {}

    inline size_type empty() const noexcept(NO_EXCEPT) { return this->get() == 0; }
    inline void clear() noexcept(NO_EXCEPT) { this->_hash = 0; }

    // return: whether inserted newly
    inline void insert(const T& v, size_type count = 1) noexcept(NO_EXCEPT) {
        this->_add_hash(multiset_hasher::_id(v), count);
    }

    // return: iterator of next element erased
    inline void erase(const T& v, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_remove_hash(multiset_hasher::_id(v), count);
    }

    inline hash_type get() const noexcept(NO_EXCEPT) { return this->_hash; }
    inline hash_type operator()() const noexcept(NO_EXCEPT) { return this->_hash; }

    inline bool operator==(const multiset_hasher& other) const noexcept(NO_EXCEPT) { return this->_hash == other._hash; }
    inline bool operator!=(const multiset_hasher& other) const noexcept(NO_EXCEPT) { return this->_hash != other._hash; }
};


} // namespace uni
