#pragma once


#include <random>
#include <unordered_set>
#include <unordered_map>
#include <iterator>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace lib {


template<
    class T,
    std::uint64_t MOD = 0x1fffffffffffffff,
    int hasher_id = -1,
    class random_engine = std::mt19937_64,
    template<class...> class map = std::unordered_map
>
struct multiset_hasher {
  private:
    using uint128_t = internal::uint128_t;

  public:
    using hash_type = std::uint64_t;
    using size_type = std::uint64_t;

    static constexpr hash_type mod = MOD;

  protected:
    static random_engine rand;
    static map<T,hash_type> _ids;

    static inline hash_type _id(const T& v) noexcept(DEV_ENV) {
        if(multiset_hasher::_ids.count(v)) return multiset_hasher::_ids[v];
        return multiset_hasher::_ids[v] = multiset_hasher::rand() % mod;
    }

    static constexpr hash_type mask(const size_type a) noexcept(DEV_ENV) { return (1ULL << a) - 1; }

    static constexpr hash_type mul(hash_type a, hash_type b) noexcept(DEV_ENV) {
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

    inline void _add_hash(const hash_type h, const hash_type count) noexcept(DEV_ENV) {
        this->_hash += multiset_hasher::mul(h, count);
        if(this->_hash >= multiset_hasher::mod) this->_hash -= multiset_hasher::mod;
    }
    inline void _remove_hash(const hash_type h, const hash_type count) noexcept(DEV_ENV) {
        auto hash = static_cast<std::make_signed_t<hash_type>>(this->_hash);
        hash -= multiset_hasher::mul(h, count);
        if(hash < 0) hash += multiset_hasher::mod;
        this->_hash = hash;
    }

  public:
    multiset_hasher() noexcept(DEV_ENV) {
        static_assert(mod < std::numeric_limits<std::make_signed_t<hash_type>>::max());
    }

    template<class I> multiset_hasher(const I first, const I last) noexcept(DEV_ENV) : multiset_hasher() {
        for(auto itr=first; itr != last; ++itr) this->insert(*itr);
    }

    template<class U>
    multiset_hasher(const std::initializer_list<U>& init_list) noexcept(DEV_ENV) : multiset_hasher(std::begin(init_list), std::end(init_list)) {}

    inline size_type empty() const noexcept(DEV_ENV) { return this->get() == 0; }
    inline void clear() const noexcept(DEV_ENV) { this->_hash = 0; }

    // return: whether inserted newly
    inline void insert(const T& v, size_type count = 1) noexcept(DEV_ENV) {
        this->_add_hash(multiset_hasher::_id(v), count);
    }

    // return: iterator of next element erased
    inline void erase(const T& v, const size_type count = 1) noexcept(DEV_ENV) {
        this->_remove_hash(multiset_hasher::_id(v), count);
    }

    inline hash_type get() const noexcept(DEV_ENV) { return this->_hash; }
    inline hash_type operator()() const noexcept(DEV_ENV) { return this->_hash; }

    inline bool operator==(const multiset_hasher& other) const noexcept(DEV_ENV) { return this->_hash == other._hash; }
    inline bool operator!=(const multiset_hasher& other) const noexcept(DEV_ENV) { return this->_hash != other._hash; }
};

template<class T, std::uint64_t mod, int id, class E, template<class...> class M>
E multiset_hasher<T,mod,id,E,M>::rand(std::random_device{}());

template<class T, std::uint64_t mod, int id, class E, template<class...> class M>
M<T,typename multiset_hasher<T,mod,id,E,M>::hash_type> multiset_hasher<T,mod,id,E,M>::_ids;


} // namespace lib
