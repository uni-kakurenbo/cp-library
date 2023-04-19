#pragma once

#include <set>
#include <unordered_set>
#include <iterator>
#include <optional>

#include "internal/types.hpp"
#include "utility/functional.hpp"

namespace lib {

namespace internal {


template<class set> struct set_wrapper : set {
    using set::set;
    using size_type = internal::size_t;

    inline size_type size() const { return this->set::size(); }

    inline bool contains(const typename set::key_type& key) { return static_cast<bool>(this->count(key)); }

    inline std::optional<typename set::iterator> remove(const typename set::key_type& key) {
        const auto itr = this->find(key);
        if(itr == this->set::end()) return {};
        return this->erase(itr);
    }

    inline auto min_element() { return this->begin(); }
    inline auto max_element() { return this->begin(); }

    inline auto min() { return *this->begin(); }
    inline auto max() { return *this->end(); }

    inline void pop_min() { return this->erase(this->begin()); }
    inline void pop_max() { return this->erase(this->end()); }

    inline auto next_element(const typename set::key_type& key, const size_type _count = 0) {
        size_type count = std::abs(_count);
        auto itr = this->lower_bound(key);
        const auto begin = this->begin(), end = this->end();
        if(itr == end) return this->end();
        if(itr == begin) return this->begin();
        while(count--) {
            if(_count < 0) if(itr-- == begin) return this->begin();
            if(_count > 0) if(++itr == end) return this->end();
        }
        return itr;
    }
    inline auto prev_element(const typename set::key_type& key, const size_type _count = 0) {
        size_type count = std::abs(_count);
        auto itr = this->upper_bound(key);
        const auto begin = this->begin(), end = this->end();
        if(itr == end) return this->end();
        if(itr-- == begin) return this->begin();
        while(count--) {
            if(_count < 0) if(itr-- == begin) return this->begin();
            if(_count > 0) if(++itr == end) return this->end();
        }
        return itr;
    }

    inline std::optional<typename set::value_type> next(const typename set::key_type& key, size_type count = 0) {
        auto itr = this->lower_bound(key);
        const auto end = this->end();
        if(itr == end) return {};
        while(count--) if(++itr == end) return {};
        return { *itr };
    }

    inline std::optional<typename set::value_type> prev(const typename set::key_type& key, size_type count = 0) {
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
