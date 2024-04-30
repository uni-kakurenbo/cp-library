#pragma once


#include <cstdint>
#include <optional>
#include <type_traits>
#include <iterator>
#include <concepts>
#include <ranges>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/concepts.hpp"

#include "global/constants.hpp"

#include "data_structure/segment_tree.hpp"
#include "action/range_set_range_sum.hpp"
#include "action/range_add_range_sum.hpp"
#include "action/range_sum.hpp"
#include "action/helpers.hpp"
#include "algebraic/combined.hpp"
#include "algebraic/addition.hpp"
#include "algebraic/minimum.hpp"


namespace uni {


namespace internal {


template<
    template<class...> class Tree,
    template<class...> class Action
>
    requires actions::internal::action<Action<internal::size_t>>
struct set_adaptor_impl {
    using size_type = internal::size_t;
    using key_type = internal::size_t;
    using value_type = internal::size_t;

  protected:
    using impl_data_type = algebraic::combined<algebraic::addition<value_type>, algebraic::minimum<value_type>>;

    using impl_tree =
        Tree<
            std::conditional_t<
                internal::available_with<
                    Tree,
                    Action<impl_data_type>
                >,
                Action<impl_data_type>,
                actions::range_sum<impl_data_type>
            >
        >;

    impl_tree _data;
    size_type _elem = 0;

  public:
    set_adaptor_impl(const size_type sup) noexcept(NO_EXCEPT) : _data(sup) {};

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_data.fold(); }
    inline bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }

    inline size_type count(const key_type& k) const noexcept(NO_EXCEPT) { return this->_data.get(k).val()->first.val(); }
    inline bool contains(const key_type& k) const noexcept(NO_EXCEPT) { return this->_data.get(k).val()->first.val() > 0; }

    inline value_type mex(const key_type& base = 0) const noexcept(NO_EXCEPT) {
        return this->_data.max_right(base, [](const auto& p) { return p.val()->second.val() > 0; });
    }

    inline std::optional<value_type> next(const key_type& k, const size_type count = 0) const noexcept(NO_EXCEPT) {
        const auto v = this->_data.max_right(k, [count](const auto& p) { return p.val()->first.val() <= count; });
        if(v == this->_data.size()) return {};
        return { v };
    }
    inline std::optional<value_type> prev(const key_type& k, const size_type count = 0) const noexcept(NO_EXCEPT) {
        const auto v = this->_data.min_left(k + 1, [count](const auto& p) { return p.val()->first.val() <= count; });
        if(v == 0) return {};
        return { v - 1 };
    }

    inline auto kth_smallest(const size_type k) const noexcept(NO_EXCEPT) { return this->next(0, k); }
    inline auto kth_largest(const size_type k) const noexcept(NO_EXCEPT) { return this->prev(this->_data.size()-1, k); }

    inline value_type min() const noexcept(NO_EXCEPT) { return this->kth_smallest(0); }
    inline value_type max() const noexcept(NO_EXCEPT) { return this->kth_largest(0); }

    inline size_type count_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(0, v).val()->first.val(); }
    inline size_type count_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(v+1, this->_data.size()).val()->first.val(); }
    inline size_type count_or_under(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(0, v+1).val()->first.val(); }
    inline size_type count_or_over(const value_type& v) const noexcept(NO_EXCEPT) { return this->_data.fold(v, this->_data.size()).val()->first.val(); }

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


};


template<template<class...> class Tree = uni::segment_tree>
struct set_adaptor : internal::set_adaptor_impl<Tree, actions::range_set_range_sum> {
    using size_type = internal::size_t;
    using key_type = internal::size_t;
    using value_type = size_type;

  protected:
    using Base = internal::set_adaptor_impl<Tree, actions::range_set_range_sum>;
    using impl_data_type = typename Base::impl_data_type;

  public:
    set_adaptor() noexcept(NO_EXCEPT) = default;

    set_adaptor(const size_type sup) noexcept(NO_EXCEPT) : Base(sup) {};

    template<std::input_iterator I, std::sentinel_for<I> S>
    set_adaptor(I first, S last) noexcept(NO_EXCEPT) : set_adaptor(*std::ranges::max_element(first, last) + 1) {
        valarray<bool> bits(this->_data.size());
        REP(itr, first, last) {
            assert(0 <= *itr && *itr < this->_data.size());
            bits[*itr] = true;
        }
        this->build_from_bits(ALL(bits));
    };

    template<std::ranges::input_range R>
    set_adaptor(R&& range) noexcept(NO_EXCEPT) : set_adaptor(ALL(range)) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    inline auto& build_from_bits(I first, S last) noexcept(NO_EXCEPT) {
        if constexpr(std::sized_sentinel_for<S, I>) {
            assert(std::ranges::distance(first, last) == this->_data.size());
        }
        this->_data.assign(first, last);
        return *this;
    };

    template<std::ranges::input_range R>
    inline auto& build_from_bits(R&& range) noexcept(NO_EXCEPT) {
        return this->build_from_bits(ALL(range));
    }

    inline bool insert(const key_type& k) noexcept(NO_EXCEPT) {
        assert(0 <= k && k < this->_data.size());
        const bool res = !this->_data.get(k).val()->first.val();
        if(res) this->_data.set(k, impl_data_type{ 1 });
        return res;
    }

    inline bool remove(const key_type& k) noexcept(NO_EXCEPT) {
        assert(0 <= k && k < this->_data.size());
        const bool res = this->_data.get(k).val()->first.val();
        if(res) this->_data.set(k, impl_data_type{ 0 });
        return res;
    }
};


template<template<class...> class Tree = uni::segment_tree, std::integral Size = std::int64_t>
struct multiset_adaptor : internal::set_adaptor_impl<Tree, actions::range_add_range_sum> {
    using size_type = Size;
    using key_type = internal::size_t;
    using value_type = key_type;

  private:
    using Base = internal::set_adaptor_impl<Tree, actions::range_add_range_sum>;
    using impl_data_type = typename Base::impl_data_type;


  public:
    multiset_adaptor(const size_type sup) noexcept(NO_EXCEPT) : Base(sup) {};

    template<std::input_iterator I, std::sentinel_for<I> S>
    multiset_adaptor(I first, S last) noexcept(NO_EXCEPT) : Base(*std::ranges::max_element(first, last) + 1) {
        vector<size_type> cnts(this->_data.size());
        REP(itr, first, last) {
            assert(0 <= *itr && *itr < this->_data.size());
            cnts[*itr]++;
        }
        this->build_from_histogram(ALL(cnts));
    };

    template<std::ranges::input_range R>
    multiset_adaptor(R&& range) noexcept(NO_EXCEPT) : multiset_adaptor(ALL(range)) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    inline auto& build_from_histogram(I first, S last) noexcept(NO_EXCEPT) {
        if constexpr(std::sized_sentinel_for<S, I>) {
            assert(std::ranges::distance(first, last) == this->_data.size());
        }
        this->_data.assign(first, last);
        return *this;
    };

    template<std::ranges::input_range R>
    inline auto& build_from_histogram(R&& range) noexcept(NO_EXCEPT) {
        return this->build_from_histogram(ALL(range));
    }


    inline void insert(const key_type& k, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(0 <= k && k < this->_data.size());
        assert(0 <= count);
        const auto cur = this->_data.get(k);
        const auto num = cur.val()->first.val();
        this->_data.set(k, impl_data_type{ num + count });
        this->_elem += count;
    }

    inline void remove(const key_type& k, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(0 <= k && k < this->_data.size());
        const auto cur = this->_data.get(k);
        const auto num = cur.val()->first.val();
        assert(0 <= count && count <= num);
        this->_data.set(k, impl_data_type{ num - count });
    }
};


} // namespace uni
