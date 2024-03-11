#pragma once


#include <memory>
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

#include "random/engine.hpp"


#include "debugger/debug.hpp"


namespace lib {

namespace internal {


// Thanks to: https://github.com/xuzijian629/library2/blob/master/treap/implicit_treap.cpp
template<class Derived, std::integral SizeType, class DataType, tree_indexing_policy INDEXING_POLICY>
struct treap_impl : private uncopyable {
    using size_type = SizeType;

    struct node_type;
    using node_pointer = std::add_pointer_t<node_type>;

  private:
    using derived = Derived;

    static constexpr i64 XORSHIFT_ID = -(1L << 62);
    static inline random_engine_32bit<XORSHIFT_ID> rand;

    using priority_type = random_engine_32bit<>::result_type;


    static void pushup(const node_pointer tree) noexcept(NO_EXCEPT) {
        if(tree == node_type::nil) return;
        tree->size = tree->left->size + tree->length + tree->right->size;
        derived::pushup(tree);
    }

  public:
    struct node_type {
        using data_type = DataType;

        priority_type priority = std::numeric_limits<priority_type>::lowest();

        std::add_pointer_t<node_type> left = node_type::nil, right = node_type::nil;

        size_type length, size;
        [[no_unique_address]] data_type data;

        node_type() noexcept = default;

        node_type(const data_type& _data, const size_type _size) noexcept(NO_EXCEPT)
            requires (INDEXING_POLICY == tree_indexing_policy::sorted)
            : priority(treap_impl::rand()), length(_size), size(_size), data(_data)
        {}

        node_type(const data_type& _data, const size_type _size) noexcept(NO_EXCEPT)
            requires (INDEXING_POLICY == tree_indexing_policy::implicit_key)
            : priority(treap_impl::rand()), length(_size), size(_size), data(_data)
        {}

        ~node_type() {
            delete this->left;
            delete this->right;
        }


        static inline const node_pointer nil = new node_type{};

        void* operator new(std::size_t n) {
            return malloc(n);
        };

        void operator delete(void* ptr) noexcept {
            if(ptr == node_type::nil) return;
            free(ptr);
        }
    };


    static void split(const node_pointer tree, const size_type pos, node_pointer& left, node_pointer& right) noexcept(NO_EXCEPT) {
        if(tree == node_type::nil) {
            if(pos > 0) {
                left = new node_type({}, pos);
                right = node_type::nil;
            }
            else if(pos < 0) {
                right = new node_type({}, -pos);
                left = node_type::nil;
            }
            else {
                left = right = node_type::nil;
            }
            return;
        }

        derived::pushdown(tree);

        if constexpr(INDEXING_POLICY == tree_indexing_policy::implicit_key) {
            const size_type lower_bound = tree->left->size;
            const size_type upper_bound = tree->size - tree->right->size;

            if(
                pos <= lower_bound
            ) {
                treap_impl::split(tree->left, pos, left, tree->left), right = std::move(tree);
                treap_impl::pushup(right);
            }
            else if(
                pos >= upper_bound
            ) {
                treap_impl::split(tree->right, pos - upper_bound, tree->right, right), left = std::move(tree);
                treap_impl::pushup(left);
            }
            else {
                tree->length = pos - lower_bound;
                treap_impl::merge(tree->right, new node_type(tree->data, upper_bound - pos), tree->right);

                treap_impl::split(tree->right, 0, tree->right, right), left = std::move(tree);
                treap_impl::pushup(left);
            }
        }
        else {
            if(pos <= tree->data) {
                treap_impl::split(tree->left, pos, left, tree->left), right = std::move(tree);
                treap_impl::pushup(right);
            }
            else {
                treap_impl::split(tree->right, pos, tree->right, right), left = std::move(tree);
                treap_impl::pushup(left);
            }
        }
    }


    static void merge(node_pointer& tree, const node_pointer left, const node_pointer right) noexcept(NO_EXCEPT) {
        derived::pushdown(left);
        derived::pushdown(right);

        if(left == node_type::nil || right == node_type::nil) {
            tree = left == node_type::nil ? right : left;
        }
        else if(left->priority > right->priority) {
            treap_impl::merge(left->right, left->right, right), tree = std::move(left);
        }
        else {
            treap_impl::merge(right->left, left, right->left), tree = std::move(right);
        }

        treap_impl::pushup(tree);
    }
};


} // namespace internal


template<std::integral SizeType = i64>
struct treap_context {
    template<class derived, tree_indexing_policy INDEXING_POLICY, class DataType = internal::dummy>
    using interface = internal::treap_impl<derived, SizeType, DataType, INDEXING_POLICY>;
};


} // namespace lib
