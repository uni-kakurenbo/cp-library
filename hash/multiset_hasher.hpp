#pragma once

#include "snippet/iterations.hpp"

#include <random>
#include <unordered_set>
#include <unordered_map>
#include <iterator>


namespace lib {


template<
    class T,
    int hasher_id = -1,
    class random_engine = std::mt19937_64,
    typename random_engine::result_type MOD = 0x1fffffffffffffff,
    template<class...> class multiset = std::unordered_multiset,
    template<class...> class map = std::unordered_map
>
struct multiset_hasher : protected multiset<T> {
  private:
    using base = multiset<T>;

  public:
    using hash_type = typename random_engine::result_type;
    using size_type = typename base::size_type;

    static constexpr hash_type mod = MOD;

  protected:
    static random_engine rand;
    static map<T,hash_type> _ids;

    hash_type _hash = 0;

    static inline hash_type id(const T& v) {
        if(multiset_hasher::_ids.count(v)) return multiset_hasher::_ids[v];
        return multiset_hasher::_ids[v] = multiset_hasher::rand();
    }

    inline void _add_hash(const hash_type h) {
        this->_hash += h;
        if(this->_hash >= multiset_hasher::mod) this->_hash -= multiset_hasher::mod;
    }
    inline void _remove_hash(const hash_type h) {
        this->_hash -= h;
        if(this->_hash < 0) this->_hash += multiset_hasher::mod;
    }

  public:
    multiset_hasher() {}

    template<class I> multiset_hasher(const I first, const I last) {
        for(auto itr=first; itr != last; ++itr) this->_insert(*itr);
    }

    template<class U>
    multiset_hasher(const std::initializer_list<U>& init_list) : multiset_hasher(std::begin(init_list), std::end(init_list)) {}

    inline size_type empty() const { return this->base::empty(); }
    inline size_type size() const { return this->base::size(); }
    inline size_type max_size() const { return this->base::max_size(); }

    inline void clear() const { this->_hash = 0, this->base::clear(); }

    using base::count;
    using base::find;
    using base::equal_range;

    template<class... Args> auto lower_bound(const Args&... args) const { return this->base::lower_bound(args...); }
    template<class... Args> auto upper_bound(const Args&... args) const { return this->base::upper_bound(args...); }

    // return: whether inserted newly
    inline void insert(const T& v, size_type count = 1) {
        if(count == -1) count = this->size();
        REP(count) this->base::insert(v), this->_add_hash(multiset_hasher::id(v));
    }

    // remove: all elements v
    // return: number of erased elements (0 or 1)
    inline size_type erase(const T& v) {
        size_type count = this->base::erase(v);
        REP(count) this->_remove_hash(multiset_hasher::id(v));
        return count;
    }

    // return: number of erased elements (0 or 1)
    inline typename base::iterator erase(const T& v, const size_type count) {
        dev_assert(0 <= count and count <= this->count(v));
        auto itr = this->find(v);
        REP(count) itr = this->base::erase(itr), this->_remove_hash(multiset_hasher::id(v));
        return itr;
    }

    inline hash_type get() const { return this->_hash; }
    inline hash_type operator()() const { return this->_hash; }

    inline bool operator==(const multiset_hasher& other) const { return this->_hash == other._hash; }
    inline bool operator!=(const multiset_hasher& other) const { return this->_hash != other._hash; }
};

template<class T, int id, class E, typename E::result_type mod, template<class...> class S, template<class...> class M>
E multiset_hasher<T,id,E,mod,S,M>::rand(std::random_device{}());

template<class T, int id, class E, typename E::result_type mod, template<class...> class S, template<class...> class M>
M<T,typename multiset_hasher<T,id,E,mod,S,M>::hash_type> multiset_hasher<T,id,E,mod,S,M>::_ids;


} // namespace lib
