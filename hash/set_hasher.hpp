#pragma once


#include <random>
#include <unordered_set>
#include <unordered_map>
#include <iterator>

#include "internal/dev_env.hpp"


namespace lib {


template<
    class T,
    int hasher_id = -1,
    class random_engine = std::mt19937_64,
    template<class...> class set = std::unordered_set,
    template<class...> class map = std::unordered_map
>
struct set_hasher : protected set<T> {
  private:
    using base = set<T>;

  public:
    using hash_type = typename random_engine::result_type;
    using size_type = typename base::size_type;

  protected:
    static random_engine rand;
    static map<T,hash_type> _ids;

    hash_type _hash = 0;

    static inline hash_type id(const T& v) noexcept(NO_EXCEPT) {
        auto itr = set_hasher::_ids.find(v);
        if(itr == std::end(set_hasher::_ids)) return set_hasher::_ids[v] = set_hasher::rand();
        return itr->second;
    }

  public:
    set_hasher() {}

    template<class I> set_hasher(const I first, const I last) noexcept(NO_EXCEPT) {
        for(auto itr=first; itr != last; ++itr) this->_insert(*itr);
    }

    template<class U>
    set_hasher(const std::initializer_list<U>& init_list) noexcept(NO_EXCEPT) : set_hasher(std::begin(init_list), std::end(init_list)) {}

    inline size_type empty() const noexcept(NO_EXCEPT) { return this->base::empty(); }
    inline size_type size() const noexcept(NO_EXCEPT) { return this->base::size(); }
    inline size_type max_size() const noexcept(NO_EXCEPT) { return this->base::max_size(); }

    inline void clear() const noexcept(NO_EXCEPT) { this->_hash = 0, this->base::clear(); }

    using base::count;
    using base::find;
    using base::equal_range;

    inline auto begin() const noexcept(NO_EXCEPT) { return this->base::begin(); }
    inline auto end() const noexcept(NO_EXCEPT) { return this->base::end(); }

    template<class... Args> auto lower_bound(const Args&... args) const noexcept(NO_EXCEPT) { return this->base::lower_bound(args...); }
    template<class... Args> auto upper_bound(const Args&... args) const noexcept(NO_EXCEPT) { return this->base::upper_bound(args...); }

    // return: whether inserted newly
    inline bool insert(const T& v) noexcept(NO_EXCEPT) {
        if(this->base::count(v)) return false;
        this->base::insert(v);
        this->_hash ^= set_hasher::id(v);
        return true;
    }

    // return: number of erased elements (0 or 1)
    inline size_type erase(const T& v) noexcept(NO_EXCEPT) {
        if(not this->base::count(v)) return 0;
        this->base::erase(v);
        this->_hash ^= set_hasher::id(v);
        return 1;
    }

    inline hash_type get() const noexcept(NO_EXCEPT) { return this->_hash; }
    inline hash_type operator()() const noexcept(NO_EXCEPT) { return this->_hash; }

    inline bool operator==(const set_hasher& other) const noexcept(NO_EXCEPT) { return this->_hash == other._hash; }
    inline bool operator!=(const set_hasher& other) const noexcept(NO_EXCEPT) { return this->_hash != other._hash; }
};

template<class T, int id, class E, template<class...> class S, template<class...> class M>
E set_hasher<T,id,E,S,M>::rand(std::random_device{}());

template<class T, int id, class E, template<class...> class S, template<class...> class M>
M<T,typename set_hasher<T,id,E,S,M>::hash_type> set_hasher<T,id,E,S,M>::_ids;


} // namespace lib
