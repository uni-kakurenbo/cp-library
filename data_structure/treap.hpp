#pragma once


#include <cassert>
#include <utility>
#include <type_traits>
#include <initializer_list>
#include <random>
#include <concepts>
#include <ranges>


#include "snippet/aliases.hpp"
#include "utility/functional.hpp"

#include "internal/dev_env.hpp"
#include "internal/uncopyable.hpp"
#include "internal/dummy.hpp"
#include "internal/types.hpp"
#include "internal/concepts.hpp"

#include "global/constants.hpp"

#include "random/xorshift.hpp"


#include "debugger/debug.hpp"


namespace lib {

namespace internal {


// Thanks to: https://github.com/xuzijian629/library2/blob/master/treap/implicit_treap.cpp
template<class Derived, std::integral SizeType, class DataType, tree_indexing_policy INDEXING_POLICY>
struct interface : private uncopyable {
    using size_type = SizeType;

    struct node_type;
    using node_pointer = std::add_pointer_t<node_type>;

  private:
    using derived = Derived;

    static constexpr i64 XORSHIFT_ID = -(1L << 62);
    static inline xorshift<XORSHIFT_ID> rand;

    using priority_type = xorshift<>::result_type;


    static void pushup(const node_pointer tree) noexcept(NO_EXCEPT) {
        if(!tree) return;
        tree->size = interface::size(tree->left) + tree->length + interface::size(tree->right);
        derived::pushup(tree);
    }

  public:
    // size of subtree whoes root is the node.
    inline static size_type size(const node_pointer tree) noexcept(NO_EXCEPT) { return tree ? tree->size : 0; }

    struct node_type {
        using data_type = DataType;

        priority_type priority;

        std::add_pointer_t<node_type> left = nullptr, right = nullptr;

        size_type length, size;
        [[no_unique_address]] data_type data;


        node_type(const data_type& _data, const size_type _size) noexcept(NO_EXCEPT)
            requires (INDEXING_POLICY == tree_indexing_policy::sorted)
            : priority(interface::rand()), length(_size), size(_size), data(_data)
        {}

        node_type(const data_type& _data, const size_type _size) noexcept(NO_EXCEPT)
            requires (INDEXING_POLICY == tree_indexing_policy::implicit_key)
            : priority(interface::rand()), length(_size), size(_size), data(_data)
        {}

        ~node_type() {
            delete this->left;
            delete this->right;
        }
    };


    static void split(const node_pointer tree, const size_type pos, node_pointer& left, node_pointer& right) noexcept(NO_EXCEPT) {
        if(!tree) {
            if(pos > 0) {
                left = new node_type({}, pos);
                right = nullptr;
            }
            else if(pos < 0) {
                right = new node_type({}, -pos);
                left = nullptr;
            }
            else {
                left = right = nullptr;
            }
            return;
        }

        derived::pushdown(tree);

        if constexpr(INDEXING_POLICY == tree_indexing_policy::implicit_key) {
            const size_type lower_bound = interface::size(tree->left);
            const size_type upper_bound = interface::size(tree) - interface::size(tree->right);

            if(
                pos <= lower_bound
            ) {
                interface::split(tree->left, pos, left, tree->left), right = std::move(tree);
                interface::pushup(right);
            }
            else if(
                pos >= upper_bound
            ) {
                interface::split(tree->right, pos - upper_bound, tree->right, right), left = std::move(tree);
                interface::pushup(left);
            }
            else {
                tree->length = pos - lower_bound;
                interface::merge(tree->right, new node_type(tree->data, upper_bound - pos), tree->right);

                interface::split(tree->right, 0, tree->right, right), left = std::move(tree);
                interface::pushup(left);
            }
        }
        else {
            if(pos <= tree->data) {
                interface::split(tree->left, pos, left, tree->left), right = std::move(tree);
                interface::pushup(right);
            }
            else {
                interface::split(tree->right, pos, tree->right, right), left = std::move(tree);
                interface::pushup(left);
            }
        }
    }


    static void merge(node_pointer& tree, const node_pointer left, const node_pointer right) noexcept(NO_EXCEPT) {
        derived::pushdown(left);
        derived::pushdown(right);

        if(!(left && right)) {
            tree = left ? left : right;
        }
        else if(left->priority > right->priority) {
            interface::merge(left->right, left->right, right), tree = std::move(left);
        }
        else {
            interface::merge(right->left, left, right->left), tree = std::move(right);
        }

        interface::pushup(tree);
    }
};


} // namespace internal


template<std::integral SizeType = i64>
struct treap_context {
    template<class derived, tree_indexing_policy INDEXING_POLICY, class DataType = internal::dummy>
    using interface = internal::interface<derived, SizeType, DataType, INDEXING_POLICY>;
};


} // namespace lib
