#pragma once

#include <set>
#include <iterator>
#include <optional>

namespace lib {

namespace internal {


template<class set> struct set_wrapper : set {

  public:
    inline bool contains(const typename set::key_type& key) { return static_cast<bool>(this->count(key)); }
    inline auto remove(const typename set::key_type& key) { return this->erase(this->find(key)); }

    inline auto min_element() { return this->begin(); }
    inline auto max_element() { return this->begin(); }

    inline auto min() { return *this->begin(); }
    inline auto max() { return *this->end(); }

    inline void pop_min() { return this->erase(this->begin()); }
    inline void pop_max() { return this->erase(this->end()); }

    inline auto next_element(const typename set::key_type& key) { return this->lower_bound(key); }
    inline auto prev_element(const typename set::key_type& key) { return std::prev(this->lower_bound(key)); }

    inline std::optional<typename set::value_type> next(const typename set::key_type& key, typename set::size_type count = 0) {
        auto itr = this->lower_bound(key);
        const auto end = this->end();
        if(itr == end) return {};
        while(count--) if(++itr == end) return {};
        return { *itr };
    }

    inline std::optional<typename set::value_type> prev(const typename set::key_type& key, typename set::size_type count = 0) {
        auto itr = this->upper_bound(key);
        const auto begin = this->begin();
        if(itr-- == begin) return {};
        while(count--) if(itr-- == begin) return {};
        return { *itr };
    }
};


} //namespace internal


template<class... Args> using set = internal::set_wrapper<std::set<Args...>>;
template<class... Args> using unordered_set = internal::set_wrapper<std::unordered_set<Args...>>;
template<class... Args> using multiset = internal::set_wrapper<std::multiset<Args...>>;
template<class... Args> using unordered_multiset = internal::set_wrapper<std::unordered_multiset<Args...>>;


} // namespace lib
