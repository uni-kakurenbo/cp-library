#pragma once


#include <set>
#include <unordered_set>
#include <iterator>
#include <optional>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "utility/functional.hpp"


namespace lib {

namespace internal {


template<class set> struct set_wrapper : set {
    using set::set;
    using size_type = internal::size_t;

    inline size_type size() const noexcept(NO_EXCEPT) { return this->set::size(); }

    inline bool contains(const typename set::key_type& key) noexcept(NO_EXCEPT) { return static_cast<bool>(this->count(key)); }

    inline std::optional<typename set::iterator> remove(const typename set::key_type& key) noexcept(NO_EXCEPT) {
        const auto itr = this->find(key);
        if(itr == this->set::end()) return {};
        return this->erase(itr);
    }

    inline auto min_element() noexcept(NO_EXCEPT) { return this->begin(); }
    inline auto max_element() noexcept(NO_EXCEPT) { return this->begin(); }

    inline auto min() noexcept(NO_EXCEPT) { return *this->begin(); }
    inline auto max() noexcept(NO_EXCEPT) { return *this->end(); }

    inline auto pop_min() noexcept(NO_EXCEPT) { this->erase(this->begin()); return *this; }
    inline auto pop_max() noexcept(NO_EXCEPT) { this->erase(this->end()); return *this; }

    inline auto next_element(const typename set::key_type& key, const size_type _count = 0) noexcept(NO_EXCEPT) {
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
    inline auto prev_element(const typename set::key_type& key, const size_type _count = 0) noexcept(NO_EXCEPT) {
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

    inline std::optional<typename set::value_type> next(const typename set::key_type& key, size_type count = 0) noexcept(NO_EXCEPT) {
        auto itr = this->lower_bound(key);
        const auto end = this->end();
        if(itr == end) return {};
        while(count--) if(++itr == end) return {};
        return { *itr };
    }

    inline std::optional<typename set::value_type> prev(const typename set::key_type& key, size_type count = 0) noexcept(NO_EXCEPT) {
        auto itr = this->upper_bound(key);
        const auto begin = this->begin();
        if(itr-- == begin) return {};
        while(count--) if(itr-- == begin) return {};
        return { *itr };
    }


    friend inline set_wrapper operator|(const set_wrapper s, const set_wrapper t) noexcept(NO_EXCEPT) {
        set_wrapper res;
        std::set_union(std::begin(s), std::end(s), std::begin(t), std::end(t), std::inserter(res, std::begin(res)));
        return res;
    }
};

} //namespace internal

template<class... Args> using set = internal::set_wrapper<std::set<Args...>>;
template<class... Args> using unordered_set = internal::set_wrapper<std::unordered_set<Args...>>;
template<class... Args> using multiset = internal::set_wrapper<std::multiset<Args...>>;
template<class... Args> using unordered_multiset = internal::set_wrapper<std::unordered_multiset<Args...>>;


} // namespace lib
