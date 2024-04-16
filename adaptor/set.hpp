#pragma once


#include <set>
#include <unordered_set>
#include <iterator>
#include <optional>
#include <ranges>
#include <algorithm>
#include <concepts>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "utility/functional.hpp"


namespace uni {

namespace internal {


template<class Set>
struct set_wrapper : Set {
    using Set::Set;
    using value_type = typename Set::value_type;
    using size_type = internal::size_t;

    template<class Key>
    auto remove(Key&& key) noexcept(NO_EXCEPT) { return this->extract(std::forward<Key>(key)); }

    inline auto ssize() const noexcept(NO_EXCEPT) { return std::ranges::ssize(*this); }

    inline auto min_element() const noexcept(NO_EXCEPT) { return this->begin(); }
    inline auto max_element() const noexcept(NO_EXCEPT) { return std::prev(this->end()); }

    inline auto min() const noexcept(NO_EXCEPT) { return *this->begin(); }
    inline auto max() const noexcept(NO_EXCEPT) { return *std::prev(this->end()); }

    inline auto& pop_min() noexcept(NO_EXCEPT) { this->erase(this->begin()); return *this; }
    inline auto& pop_max() noexcept(NO_EXCEPT) { this->erase(std::prev(this->end())); return *this; }


    inline auto kth_smallest_element(const size_type k) const noexcept(NO_EXCEPT) {
        return std::ranges::next(this->begin(), k);
    }

    inline auto kth_largest_element(const size_type k) const noexcept(NO_EXCEPT) {
        return std::ranges::prev(this->end(), k + 1);
    }


    inline auto kth_smallest(const size_type k) const noexcept(NO_EXCEPT) {
        return *std::ranges::next(this->begin(), k);
    }

    inline auto kth_largest(const size_type k) const noexcept(NO_EXCEPT) {
        return *std::ranges::prev(this->end(), k + 1);
    }


    inline auto& pop_kth_smallest(const size_type k) const noexcept(NO_EXCEPT) {
        return this->erase(std::ranges::next(this->begin(), k));
        return *this;
    }

    inline auto& pop_kth_largest(const size_type k) const noexcept(NO_EXCEPT) {
        return this->erase(std::ranges::prev(this->end(), k + 1));
        return *this;
    }


    auto next_element(const typename Set::key_type& key, const size_type _count = 0) const noexcept(NO_EXCEPT) {
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

    auto prev_element(const typename Set::key_type& key, const size_type _count = 0) const noexcept(NO_EXCEPT) {
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


    std::optional<typename Set::value_type> next(const typename Set::key_type& key, size_type count = 0) const noexcept(NO_EXCEPT) {
        auto itr = this->lower_bound(key);
        const auto end = this->end();
        if(itr == end) return {};
        while(count--) if(++itr == end) return {};
        return { *itr };
    }

    std::optional<typename Set::value_type> prev(const typename Set::key_type& key, size_type count = 0) const noexcept(NO_EXCEPT) {
        auto itr = this->upper_bound(key);
        const auto begin = this->begin();
        if(itr-- == begin) return {};
        while(count--) if(itr-- == begin) return {};
        return { *itr };
    }


    template<class Rhs>
    inline set_wrapper& operator|=(Rhs&& rhs) noexcept(NO_EXCEPT) {
        set_wrapper res;
        std::ranges::set_union(*this, std::forward<Rhs>(rhs), std::inserter(res, res.end()));
        this->swap(res);
        return *this;
    }

    template<class Rhs>
    inline set_wrapper& operator&=(Rhs&& rhs) noexcept(NO_EXCEPT) {
        set_wrapper res;
        std::ranges::set_intersection(*this, std::forward<Rhs>(rhs), std::inserter(res, res.end()));
        this->swap(res);
        return *this;
    }

    template<class Rhs>
    inline set_wrapper& operator^=(Rhs&& rhs) noexcept(NO_EXCEPT) {
        set_wrapper res;
        std::ranges::set_symmetric_difference(*this, std::forward<Rhs>(rhs), std::inserter(res, res.end()));
        this->swap(res);
        return *this;
    }

    template<class... Args>
    inline set_wrapper operator|(set_wrapper<Args...> rhs) noexcept(NO_EXCEPT) {
        return rhs |= *this;
    }

    template<class... Args>
    inline set_wrapper operator&(set_wrapper<Args...> rhs) noexcept(NO_EXCEPT) {
        return rhs &= *this;
    }

    template<class... Args>
    inline set_wrapper operator^(set_wrapper<Args...> rhs) noexcept(NO_EXCEPT) {
        return rhs ^= *this;
    }


    template<class... Args>
    inline auto operator<=>(const set_wrapper<Args...>& rhs) const noexcept(NO_EXCEPT) {
        const bool leq = this->size() <= rhs.size() && std::ranges::includes(rhs, *this);
        const bool geq = rhs.size() <= this->size() && std::ranges::includes(*this, rhs);

        if(leq) {
            if(geq) return std::partial_ordering::equivalent;
            return std::partial_ordering::less;
        }

        if(geq) return std::partial_ordering::greater;

        return std::partial_ordering::unordered;
    }
};


} //namespace internal


template<class... Args> using set = internal::set_wrapper<std::set<Args...>>;
template<class... Args> using unordered_set = internal::set_wrapper<std::unordered_set<Args...>>;
template<class... Args> using multiset = internal::set_wrapper<std::multiset<Args...>>;
template<class... Args> using unordered_multiset = internal::set_wrapper<std::unordered_multiset<Args...>>;


} // namespace uni
