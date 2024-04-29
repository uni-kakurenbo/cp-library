#pragma once


#include <compare>
#include <iterator>


#include "snippet/aliases.hpp"
#include "internal/dummy.hpp"

#include "action/base.hpp"
#include "data_structure/internal/basic_tree_concept.hpp"

#include "debugger/debug.hpp"




namespace uni {

namespace internal {

namespace dynamic_tree_impl {

namespace internal {



template<class T>
consteval auto to_val() {
    if constexpr(actions::internal::operatable_action<T>) return typename T::operand{};
    else return T{};
}

template<class T>
consteval auto to_acc() {
    if constexpr(actions::internal::operatable_action<T>) return typename T::operand{};
    else return dummy{};
}

template<class T>
consteval auto to_lazy() {
    if constexpr(actions::internal::effective_action<T>) return typename T::operation{};
    else return dummy{};
}


template<class T, bool LEAF_ONLY, bool MAY_BE_LAZY = true>
struct data_type {
    using val_t = decltype(to_val<T>());
    using acc_t = decltype(to_acc<T>());
    using lazy_t = decltype(to_lazy<T>());

    val_t val;
    [[no_unique_address]] acc_t acc;
    [[no_unique_address]] std::conditional_t<MAY_BE_LAZY, lazy_t, dummy> lazy;

    bool rev = false;

    data_type() noexcept = default;
    data_type(const val_t& _val) noexcept(NO_EXCEPT) : val(_val) {}

    auto _debug() const { return this->val; }


    friend bool operator==(const data_type& lhs, const data_type& rhs) noexcept(NO_EXCEPT) {
        return lhs.val == rhs.val;
    }

    friend auto operator<=>(const data_type& lhs, const data_type& rhs) noexcept(NO_EXCEPT) {
        return lhs.val <=> rhs.val;
    }
};



template<class ActionOrValue, class Derived, class Context>
struct basic_core
  : Context::substance<Derived, internal::data_type<ActionOrValue, Context::LEAF_ONLY>>
{
    using data_type = internal::data_type<ActionOrValue, Context::LEAF_ONLY>;

  private:
    using base = typename Context::substance<Derived, data_type>;
    static_assert(basic_tree<base>);

  public:
    using base::base;

    using node_handler = typename base::node_handler;
    using node_pointer = typename base::node_pointer;

    using size_type = typename base::size_type;

    using operand = data_type::val_t;
    using operation = data_type::lazy_t;


    inline operand val(const node_pointer& node) const noexcept(NO_EXCEPT) {
        if constexpr(Context::LEAF_ONLY) {
            if(node->is_leaf()) return node->size * node->data.val;
            return node->data.val;
        }
        else {
            return node->data.acc;
        }
    }


    using base::split;
    using base::merge;


    inline void split(const node_pointer tree, const size_type l, const size_type r, node_pointer& t0, node_pointer& t1, node_pointer& t2) noexcept(NO_EXCEPT) {
        // See: https://twitter.com/KakurenboUni/status/1784576244321018209
        this->split(tree, l, t0, t1);
        this->split(t1, r - l, t1, t2);
    }

    inline void split(
        const node_pointer tree,
        const size_type l, const size_type m, const size_type r,
        node_pointer& t0, node_pointer& t1, node_pointer& t2, node_pointer& t3
    ) noexcept(NO_EXCEPT) {
        // See: https://twitter.com/KakurenboUni/status/1784576244321018209
        this->split(tree, l, m, t0, t1, t2);
        this->split(t2, r - m, t2, t3);
    }

    inline void merge(node_pointer& tree, node_pointer t0, const node_pointer t1, const node_pointer t2) noexcept(NO_EXCEPT) {
        this->merge(t0, t0, t1);
        this->merge(tree, t0, t2);
    }



    void erase(node_pointer& tree, const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        assert(l <= r);
        node_pointer t0, t1, t2;

        this->split(tree, l, r, t0, t1, t2);
        this->dispose(t1);
        this->merge(tree, t0, t2);
    }


    auto pop(node_pointer& tree, const size_type pos, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(0 <= count);

        if(count == 0) return operand{};

        node_pointer t0, t1, t2;

        this->split(tree, pos, pos + count, t0, t1, t2);

        const auto res = this->val(t1);

        this->dispose(t1);
        this->merge(tree, t0, t2);

        return res;
    }


    operand get(node_pointer tree, const size_type pos) noexcept(NO_EXCEPT) {
        if(tree == node_handler::nil || pos < 0 || pos >= tree->size) return {};

        this->base::push(tree);

        const auto lower_bound = tree->left->size;
        const auto upper_bound = tree->size - tree->right->size;

        if(pos < lower_bound) {
            return this->get(tree->left, pos);
        }
        else if(pos >= upper_bound) {
            return this->get(tree->right, pos - upper_bound);
        }
        else {
            return tree->data.val;
        }
    }



    template<std::forward_iterator I>
        requires std::output_iterator<I, operand>
    void enumerate(node_pointer tree, I& itr) noexcept(NO_EXCEPT) {
        if(tree == node_handler::nil) return;

        this->base::push(tree);

        this->enumerate(tree->left, itr);

        if constexpr(Context::LEAF_ONLY) {
            if(tree->is_leaf()) {
                REP(tree->size) *(itr++) = tree->data.val;
            }
        }
        else {
            REP(tree->length) *(itr++) = tree->data.val;
        }

        this->enumerate(tree->right, itr);
    }

    operand fold(node_pointer& tree, size_type l, size_type r) noexcept(NO_EXCEPT) {
        assert(l <= r);
        if(l == r) return operand{};

        node_pointer t0, t1, t2;

        this->split(tree, l, r, t0, t1, t2);

        const operand res = this->val(t1);

        this->merge(tree, t0, t1, t2);

        return res;
    }
};


} // namespace internal

} // namespace dynamic_tree_impl

} // namespace internal

} // namespace uni
