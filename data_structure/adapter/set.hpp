#pragma once

#include <cstdint>
#include <optional>
#include <type_traits>
#include <iterator>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "constants.hpp"

#include "data_structure/internal/declarations.hpp"
#include "data_structure/range_action/range_add_range_sum.hpp"

namespace lib {


template<template<class> class Tree = lib::fenwick_tree, class Size = internal::size_t>
struct set_adapter {
    using size_type = Size;
    using key_type = internal::size_t;
    using value_type = size_type;

  protected:
    set_adapter() noexcept(NO_EXCEPT) {}

    using Impl = Tree<actions::range_add_range_sum<value_type>>;

    Impl _data;
    size_type _elem = 0;

  public:
    set_adapter(const size_type sup) noexcept(NO_EXCEPT) : _data(sup) {};

    template<class I>
    set_adapter(const I first, const I last) noexcept(NO_EXCEPT) : _data(*std::max_element(first, last)+1) {
        valarray<bool> bits(this->_data.size());
        REP(itr, first, last) {
            assert(0 <= *itr && *itr < this->_data.size());
            bits[*itr] = true;
        }
        this->build_from_bits(ALL(bits));
    };


    template<class I>
    inline auto& build_from_bits(const I first, const I last) noexcept(NO_EXCEPT) {
        assert(std::distance(first, last) == this->_data.size());
        using T = typename std::iterator_traits<I>::value_type;
        static_assert(std::is_same_v<bool,T>, "bit type must be bool");
        static_assert(std::is_same_v<Size,std::common_type_t<Size,T>>, "counter type must be narrower than size_type");
        this->_data.assign(first, last);
        return *this;
    };


    inline size_type size() const noexcept(NO_EXCEPT) { return this->_elem; }
    inline bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }

    inline size_type count(const key_type& k) const noexcept(NO_EXCEPT) { return this->_data.get(k); }
    inline bool contains(const key_type& k) const noexcept(NO_EXCEPT) { return this->_data.get(k) > 0; }

    inline bool insert(const key_type& k) noexcept(NO_EXCEPT) {
        assert(0 <= k && k < this->_data.size());
        const bool res = !this->_data.get(k);
        if(res) this->_data.apply(k, 1), ++this->_elem;
        return res;
    }

    inline bool remove(const key_type& k) noexcept(NO_EXCEPT) {
        assert(0 <= k && k < this->_data.size());
        const bool res = this->_data.get(k);
        if(res) this->_data.apply(k, -1), --this->_elem;
        return res;
    }

    inline std::optional<value_type> next(const key_type& k, const size_type count = 0) const noexcept(NO_EXCEPT) {
        const value_type v = this->_data.max_right(k, [count](const size_type p) { return p <= count; });
        if(v == this->_data.size()) return {};
        return { v };
    }
    inline std::optional<value_type> prev(const key_type& k, const size_type count = 0) const noexcept(NO_EXCEPT) {
        const value_type v = this->_data.min_left(k+1, [count](const size_type p) { return p <= count; });
        if(v == 0) return {};
        return { v - 1 };
    }

    inline auto kth_smallest(const size_type k) const noexcept(NO_EXCEPT) { return this->next(0, k); }
    inline auto kth_largest(const size_type k) const noexcept(NO_EXCEPT) { return this->prev(this->_data.size()-1, k); }

    inline value_type min() const noexcept(NO_EXCEPT) { return this->kth_smallest(0); }
    inline value_type max() const noexcept(NO_EXCEPT) { return this->kth_largest(0); }

    inline size_type count_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(0, v); }
    inline size_type count_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(v+1, this->_data.size()); }
    inline size_type count_or_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(0, v+1); }
    inline size_type count_or_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(v, this->_data.size()); }

    template<comp com = comp::equal_to>
    inline size_type count(const value_type& v) const noexcept(NO_EXCEPT) {
        if constexpr(com == comp::eq) return this->count(v);
        if constexpr(com == comp::under) return this->count_under(v);
        if constexpr(com == comp::over) return this->count_over(v);
        if constexpr(com == comp::or_under) return this->count_or_under(v);
        if constexpr(com == comp::or_over) return this->count_or_over(v);
        assert(false);
    }

    inline const auto& _debug() const noexcept(NO_EXCEPT) { return this->_data; }
};


template<template<class> class Tree = lib::fenwick_tree, class Size = std::int64_t>
struct multiset_adapter : protected set_adapter<Tree,Size> {
    using size_type = Size;
    using key_type = internal::size_t;
    using value_type = key_type;

  private:
    using base = set_adapter<Tree,Size>;

  public:
    multiset_adapter(const size_type sup) noexcept(NO_EXCEPT) : base(sup) {};

    template<class I>
    multiset_adapter(const I first, const I last) noexcept(NO_EXCEPT) : base(*max_element(first, last)+1) {
        vector<size_type> bits(this->_data.size());
        REP(itr, first, last) {
            assert(0 <= *itr && *itr < this->_data.size());
            bits[*itr]++;
        }
        this->build_from_histogram(ALL(bits));
    };

    template<class I>
    inline auto& build_from_histogram(const I first, const I last) noexcept(NO_EXCEPT) {
        assert(std::distance(first, last) == this->_data.size());
        using T = typename std::iterator_traits<I>::value_type;
        static_assert(std::is_integral_v<T>, "counter type must be integal");
        static_assert(std::is_same_v<Size,std::common_type_t<Size,T>>, "counter type must be narrower than size_type");
        this->_data.assign(first, last);
        return *this;
    };

    inline void insert(const key_type& k, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(0 <= k && k < this->_data.size());
        this->_data.apply(k, count);
        this->_elem += count;
    }

    inline void remove(const key_type& k, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(0 <= k && k < this->_data.size());
        this->_data.apply(k, -count);
        this->_elem -= count;
    }



    inline std::optional<value_type> next(const key_type& k, const size_type count = 0) const noexcept(NO_EXCEPT) {
        const value_type v = this->_data.max_right(k, [count](const size_type p) { return p <= count; });
        if(v == this->_data.size()) return {};
        return { v };
    }
    inline std::optional<value_type> prev(const key_type& k, const size_type count = 0) const noexcept(NO_EXCEPT) {
        const value_type v = this->_data.min_left(k+1, [count](const size_type p) { return p <= count; });
        if(v == 0) return {};
        return { v - 1 };
    }

    using base::kth_smallest;
    using base::kth_largest;

    using base::min;
    using base::max;

    using base::count_under;
    using base::count_over;
    using base::count_or_under;
    using base::count_or_over;
    using base::count;

    using base::_debug;

};


} // namespace lib
