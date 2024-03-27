#pragma once


#include <memory>
#include <memory_resource>
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

#include "data_structure/internal/node_handler.hpp"

#include "global/constants.hpp"

#include "random/engine.hpp"


#include "debugger/debug.hpp"


namespace lib {

namespace internal {


// Thanks to: https://github.com/xuzijian629/library2/blob/master/treap/implicit_treap.cpp
template<class Allocator, class Derived, std::integral SizeType, class ValueType, i64 Id>
struct treap_impl : private uncopyable {
    using size_type = SizeType;
    using value_type = ValueType;

    struct node_type;
    using node_handler = typename lib::node_handlers::reusing<Allocator>::template handler<node_type>;

    using allocator_type = typename node_handler::allocator_type;
    using node_pointer = typename node_handler::node_pointer;

  private:
    using derived = Derived;

    inline derived* _derived() noexcept(NO_EXCEPT) {
        return static_cast<derived*>(this);
    }
    inline const derived* _derived() const noexcept(NO_EXCEPT) {
        return static_cast<const derived*>(this);
    }

    [[no_unique_address]] node_handler _node_handler;


    static inline random_engine_32bit _rand;

    using priority_type = random_engine_32bit::result_type;

  protected:
    void pull(const node_pointer tree) const noexcept(NO_EXCEPT) {
        if(tree == node_handler::nil) return;
        tree->size = tree->left->size + tree->length + tree->right->size;
        this->_derived()->pull(tree);
    }


    void rectify(const node_pointer tree) const noexcept(NO_EXCEPT) {
        if(tree->size == 0) return;

        std::vector<priority_type> priorities(tree->size);
        std::ranges::generate(priorities, this->_rand);
        std::ranges::make_heap(priorities);

        std::queue<node_pointer> queue;
        queue.push(tree);

        auto itr = std::ranges::begin(priorities);
        while(!queue.empty()) {
            node_pointer node = queue.front();
            queue.pop();

            node->priority = *(itr++);

            if(node->left != node_handler::nil) queue.push(node->left);
            if(node->right != node_handler::nil) queue.push(node->right);
        }
    }


    node_pointer create_node(const value_type& val, const size_type size) noexcept(NO_EXCEPT) {
        if(size == 0) return node_handler::nil;
        return this->_node_handler.create(val, size);
    }

    void dispose(node_pointer tree) noexcept(NO_EXCEPT) {
        if(this->_node_handler.disposable(tree)) {
            this->dispose(tree->left);
            this->dispose(tree->right);

            this->_node_handler.dispose(tree);
        }
    }


    void _split(const node_pointer tree, const size_type pos, node_pointer& left, node_pointer& right) noexcept(NO_EXCEPT) {
        if(tree == node_handler::nil) {
            left = right = node_handler::nil;
            return;
        }

        this->_derived()->push(tree);

        const size_type lower_bound = tree->left->size;
        const size_type upper_bound = tree->size - tree->right->size;

        if(pos <= lower_bound) {
            this->split(tree->left, pos, left, tree->left), right = std::move(tree);
            this->pull(right);
        }
        else if(pos >= upper_bound) {
            this->split(tree->right, pos - upper_bound, tree->right, right), left = std::move(tree);
            this->pull(left);
        }
        else {
            tree->length = pos - lower_bound;
            this->merge(tree->right, this->create_node(tree->data, upper_bound - pos), tree->right);

            this->split(tree->right, 0, tree->right, right), left = std::move(tree);
            this->pull(left);
        }
    }

  public:
    explicit treap_impl(const allocator_type& allocator= {}) noexcept(NO_EXCEPT) : _node_handler(allocator) {}

    struct node_type {
        priority_type priority = std::numeric_limits<priority_type>::lowest();

        node_pointer left = node_handler::nil, right = node_handler::nil;

        size_type length, size;
        [[no_unique_address]] value_type data;

        node_type() noexcept = default;

        node_type(const value_type& _data, const size_type _size) noexcept(NO_EXCEPT)
            : priority(treap_impl::_rand()), length(_size), size(_size), data(_data)
        {}
    };


    template<bool STRICT = false, bool RETURN_EXISTENCE = false>
    void split(const node_pointer tree, const value_type& val, node_pointer& left, node_pointer& right, bool* exist = nullptr) noexcept(NO_EXCEPT) {
        if(tree == node_handler::nil) {
            left = right = node_handler::nil;
            return;
        }

        this->_derived()->push(tree);

        if constexpr(RETURN_EXISTENCE) *exist |= val == tree->data;

        if(val < tree->data || (!STRICT && val == tree->data)) {
            this->template split<STRICT, RETURN_EXISTENCE>(tree->left, val, left, tree->left, exist), right = std::move(tree);
            this->pull(right);
        }
        else {
            this->template split<STRICT, RETURN_EXISTENCE>(tree->right, val, tree->right, right, exist), left = std::move(tree);
            this->pull(left);
        }
    }


    void split(const node_pointer tree, const size_type pos, node_pointer& left, node_pointer& right) noexcept(NO_EXCEPT) {
        if(pos <= 0) {
            left = node_handler::nil;
            this->merge(right, this->create_node(value_type{}, -pos), std::move(tree));
        }
        else if(tree->size <= pos) {
            right = node_handler::nil;
            this->merge(left, std::move(tree), this->create_node(value_type{}, pos - tree->size));
        }
        else {
            this->_split(std::move(tree), pos, left, right);
        }
    }


    void merge(node_pointer& tree, const node_pointer left, const node_pointer right) noexcept(NO_EXCEPT) {
        this->_derived()->push(left);
        this->_derived()->push(right);

        // debug(left->priority, right->priority);

        if(left == node_handler::nil || right == node_handler::nil) {
            tree = left == node_handler::nil ? right : left;
        }
        else if(left->priority > right->priority) {
            this->merge(left->right, left->right, right), tree = std::move(left);
        }
        else {
            this->merge(right->left, left, right->left), tree = std::move(right);
        }

        this->pull(tree);
    }
};


} // namespace internal


template<std::integral SizeType = i64, class Allocator = std::allocator<SizeType>, i64 Id = -1>
struct treap_context {
    template<class Derived, class ValueType = internal::dummy>
    using interface = internal::treap_impl<Allocator, Derived, SizeType, ValueType, Id>;
};


namespace pmr {


template<std::integral SizeType = i64, i64 Id = -1>
using treap_context = lib::treap_context<SizeType, std::pmr::polymorphic_allocator<SizeType>, Id>;


} // namespace pmr


} // namespace lib
