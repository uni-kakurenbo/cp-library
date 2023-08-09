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


template<class Set> struct set_wrapper : Set {
    using Set::Set;
    using size_type = internal::size_t;

    inline size_type size() const noexcept(NO_EXCEPT) { return this->Set::size(); }

    inline bool contains(const typename Set::key_type& key) const noexcept(NO_EXCEPT) { return static_cast<bool>(this->count(key)); }

    inline std::optional<typename Set::iterator> remove(const typename Set::key_type& key) noexcept(NO_EXCEPT) {
        const auto itr = this->Set::find(key);
        if(itr == this->Set::end()) return {};
        return this->erase(itr);
    }

    inline auto min_element() const noexcept(NO_EXCEPT) { return this->begin(); }
    inline auto max_element() const noexcept(NO_EXCEPT) { return std::prev(this->end()); }

    inline auto min() const noexcept(NO_EXCEPT) { return *this->begin(); }
    inline auto max() const noexcept(NO_EXCEPT) { return *std::prev(this->end()); }

    inline auto& pop_min() noexcept(NO_EXCEPT) { this->erase(this->begin()); return *this; }
    inline auto& pop_max() noexcept(NO_EXCEPT) { this->erase(std::prev(this->end())); return *this; }

    inline auto kth_smallest_element(const size_type k) const noexcept(NO_EXCEPT) { return std::next(this->begin(), k); }
    inline auto kth_largest_element(const size_type k) const noexcept(NO_EXCEPT) { return std::prev(this->begin(), k + 1); }

    inline auto kth_smallest(const size_type k) const noexcept(NO_EXCEPT) { return *std::next(this->begin(), k); }
    inline auto kth_largest(const size_type k) const noexcept(NO_EXCEPT) { return *std::prev(this->begin(), k + 1); }

    inline auto& pop_kth_smallest(const size_type k) const noexcept(NO_EXCEPT) { return this->erase(std::next(this->begin(), k)); return *this; }
    inline auto& pop_kth_largest(const size_type k) const noexcept(NO_EXCEPT) { return this->erase(std::prev(this->begin(), k + 1)); return *this; }

    inline auto next_element(const typename Set::key_type& key, const size_type _count = 0) const noexcept(NO_EXCEPT) {
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
    inline auto prev_element(const typename Set::key_type& key, const size_type _count = 0) const noexcept(NO_EXCEPT) {
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

    inline std::optional<typename Set::value_type> next(const typename Set::key_type& key, size_type count = 0) const noexcept(NO_EXCEPT) {
        auto itr = this->lower_bound(key);
        const auto end = this->end();
        if(itr == end) return {};
        while(count--) if(++itr == end) return {};
        return { *itr };
    }

    inline std::optional<typename Set::value_type> prev(const typename Set::key_type& key, size_type count = 0) const noexcept(NO_EXCEPT) {
        auto itr = this->upper_bound(key);
        const auto begin = this->begin();
        if(itr-- == begin) return {};
        while(count--) if(itr-- == begin) return {};
        return { *itr };
    }

    friend inline set_wrapper operator|(set_wrapper s, const set_wrapper t) noexcept(NO_EXCEPT) {
        s.merge(t);
        return s;
    }
};


} //namespace internal


template<class... Args> using set = internal::set_wrapper<std::set<Args...>>;
template<class... Args> using unordered_set = internal::set_wrapper<std::unordered_set<Args...>>;
template<class... Args> using multiset = internal::set_wrapper<std::multiset<Args...>>;
template<class... Args> using unordered_multiset = internal::set_wrapper<std::unordered_multiset<Args...>>;


} // namespace lib
