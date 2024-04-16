#pragma once


#include <unordered_set>
#include <iterator>
#include <cstdint>


#include "internal/dev_env.hpp"

#include "hash/general_hasher.hpp"


namespace uni {


template<class T, int hasher_id = -1, template<class...> class Set = std::unordered_set>
struct set_hasher : protected Set<T> {
  private:
    using base = Set<T>;

  public:
    using hash_type = std::uint64_t;
    using size_type = typename base::size_type;

  protected:
    hash_type _hash = 0;

    static inline hash_type id(const T& v) noexcept(NO_EXCEPT) {
        return hash64(v);
    }

  public:
    set_hasher() noexcept(NO_EXCEPT) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    set_hasher(I first, S last) noexcept(NO_EXCEPT) {
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


} // namespace uni
