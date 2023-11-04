#pragma once


#include <cstdint>
#include <optional>
#include <type_traits>
#include <iterator>
#include <concepts>
#include <ranges>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "constants.hpp"

#include "data_structure/internal/concepts.hpp"
#include "data_structure/fenwick_tree.hpp"
#include "data_structure/range_action/range_add_range_sum.hpp"
#include "data_structure/range_action/range_sum.hpp"


namespace lib {


template<template<class...> class Tree = lib::fenwick_tree, std::integral Size = internal::size_t>
struct set_adapter {
    using size_type = Size;
    using key_type = internal::size_t;
    using value_type = size_type;

  protected:
    set_adapter() noexcept(NO_EXCEPT) {}

    using Impl =
        Tree<
            std::conditional_t<
                internal::available_for<
                    Tree,
                    actions::range_add_range_sum<value_type>
                >,
                actions::range_add_range_sum<value_type>,
                actions::range_sum<value_type>
            >
        >;

    Impl _data;
    size_type _elem = 0;

  public:
    set_adapter(const size_type sup) noexcept(NO_EXCEPT) : _data(sup) {};

    template<std::input_iterator I, std::sentinel_for<I> S>
    set_adapter(const I first, const S last) noexcept(NO_EXCEPT) : _data(*std::ranges::max_element(first, last)+1) {
        valarray<bool> bits(this->_data.size());
        REP(itr, first, last) {
            assert(0 <= *itr && *itr < this->_data.size());
            bits[*itr] = true;
        }
        this->build_from_bits(ALL(bits));
    };

    template<std::ranges::input_range R>
    set_adapter(const R& range) noexcept(NO_EXCEPT) : set_adapter(ALL(range)) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    inline auto& build_from_bits(const I first, const S last) noexcept(NO_EXCEPT) {
        if constexpr(std::sized_sentinel_for<S, I>) {
            assert(std::ranges::distance(first, last) == this->_data.size());
        }
        this->_data.assign(first, last);
        return *this;
    };

    template<std::ranges::input_range R>
    inline auto& build_from_bits(const R& range) noexcept(NO_EXCEPT) {
        return this->build_from_bits(ALL(range));
    }

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_elem; }
    inline bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }

    inline size_type count(const key_type& k) const noexcept(NO_EXCEPT) { return this->_data.get(k).val(); }
    inline bool contains(const key_type& k) const noexcept(NO_EXCEPT) { return this->_data.get(k).val() > 0; }

    inline bool insert(const key_type& k) noexcept(NO_EXCEPT) {
        assert(0 <= k && k < this->_data.size());
        const bool res = !this->_data.get(k).val();
        if(res) this->_data.apply(k, 1), ++this->_elem;
        return res;
    }

    inline bool remove(const key_type& k) noexcept(NO_EXCEPT) {
        assert(0 <= k && k < this->_data.size());
        const bool res = this->_data.get(k).val();
        if(res) this->_data.apply(k, -1), --this->_elem;
        return res;
    }

    inline std::optional<value_type> next(const key_type& k, const size_type count = 0) const noexcept(NO_EXCEPT) {
        const value_type v = this->_data.max_right(k, [count](const auto& p) { return p.val() <= count; });
        if(v == this->_data.size()) return {};
        return { v };
    }
    inline std::optional<value_type> prev(const key_type& k, const size_type count = 0) const noexcept(NO_EXCEPT) {
        const value_type v = this->_data.min_left(k+1, [count](const auto& p) { return p.val() <= count; });
        if(v == 0) return {};
        return { v - 1 };
    }

    inline auto kth_smallest(const size_type k) const noexcept(NO_EXCEPT) { return this->next(0, k); }
    inline auto kth_largest(const size_type k) const noexcept(NO_EXCEPT) { return this->prev(this->_data.size()-1, k); }

    inline value_type min() const noexcept(NO_EXCEPT) { return this->kth_smallest(0); }
    inline value_type max() const noexcept(NO_EXCEPT) { return this->kth_largest(0); }

    inline size_type count_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(0, v).val(); }
    inline size_type count_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(v+1, this->_data.size()).val(); }
    inline size_type count_or_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(0, v+1).val(); }
    inline size_type count_or_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(v, this->_data.size()).val(); }

    template<comparison com = comparison::equal_to>
    inline size_type count(const value_type& v) const noexcept(NO_EXCEPT) {
        if constexpr(com == comparison::eq) return this->count(v);
        if constexpr(com == comparison::under) return this->count_under(v);
        if constexpr(com == comparison::over) return this->count_over(v);
        if constexpr(com == comparison::or_under) return this->count_or_under(v);
        if constexpr(com == comparison::or_over) return this->count_or_over(v);
        assert(false);
    }

    inline const auto& _debug() const noexcept(NO_EXCEPT) { return this->_data; }
};


template<template<class...> class Tree = lib::fenwick_tree, std::integral Size = std::int64_t>
struct multiset_adapter : protected set_adapter<Tree, Size> {
    using size_type = Size;
    using key_type = internal::size_t;
    using value_type = key_type;

  private:
    using base = set_adapter<Tree, Size>;

  public:
    multiset_adapter(const size_type sup) noexcept(NO_EXCEPT) : base(sup) {};

    template<std::input_iterator I, std::sentinel_for<I> S>
    multiset_adapter(const I first, const S last) noexcept(NO_EXCEPT) : base(*std::ranges::max_element(first, last) + 1) {
        vector<size_type> cnts(this->_data.size());
        REP(itr, first, last) {
            assert(0 <= *itr && *itr < this->_data.size());
            cnts[*itr]++;
        }
        this->build_from_histogram(ALL(cnts));
    };

    template<std::ranges::input_range R>
    multiset_adapter(const R& range) noexcept(NO_EXCEPT) : multiset_adapter(ALL(range)) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    inline auto& build_from_histogram(const I first, const S last) noexcept(NO_EXCEPT) {
        if constexpr(std::sized_sentinel_for<S, I>) {
            assert(std::ranges::distance(first, last) == this->_data.size());
        }
        this->_data.assign(first, last);
        return *this;
    };

    template<std::ranges::input_range R>
    inline auto& build_from_histogram(const R& range) noexcept(NO_EXCEPT) {
        return this->build_from_histogram(ALL(range));
    }

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
        const value_type v = this->_data.max_right(k, [count](const auto& p) { return p.val() <= count; });
        if(v == this->_data.size()) return {};
        return { v };
    }
    inline std::optional<value_type> prev(const key_type& k, const size_type count = 0) const noexcept(NO_EXCEPT) {
        const value_type v = this->_data.min_left(k+1, [count](const auto& p) { return p.val() <= count; });
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
