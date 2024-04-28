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


namespace uni {

namespace internal {


// Thanks to: https://github.com/xuzijian629/library2/blob/master/treap/implicit_treap.cpp
template<class NodeHandler, class Derived, std::integral SizeType, class ValueType>
struct red_black_tree_impl {
    using size_type = SizeType;
    using rank_type = int;
    using value_type = ValueType;

    enum class node_colors : std::int8_t { RED, BLACK };

    struct node_type;
    using node_handler = typename NodeHandler::handler<node_type>;

    using allocator_type = typename node_handler::allocator_type;
    using node_pointer = typename node_handler::node_pointer;

    struct node_type {
        node_pointer left = node_handler::nil, right = node_handler::nil;

        node_colors color = node_colors::BLACK;
        size_type size = 0;
        rank_type rank = 0;

        inline constexpr bool is_leaf() const noexcept(NO_EXCEPT) {
            return this->left == node_handler::nil && this->right == node_handler::nil;
        }

        [[no_unique_address]] value_type data;

        node_type() noexcept = default;

        node_type(const value_type& _data, const size_type _size) noexcept(NO_EXCEPT)
            : size(_size), data(_data)
        {}

        node_type(const node_colors _color, const node_pointer& _left, const node_pointer& _right) noexcept(NO_EXCEPT)
          : left(_left), right(_right), color(_color)
        {}
    };

  private:
    using derived = Derived;

    inline auto* _derived() noexcept(NO_EXCEPT) {
        return static_cast<derived*>(this);
    }
    inline const auto* _derived() const noexcept(NO_EXCEPT) {
        return static_cast<const derived*>(this);
    }

    [[no_unique_address]] node_handler _node_handler;

  public:
    void pull(const node_pointer& tree) const noexcept(NO_EXCEPT) {
        if(tree == node_handler::nil) return;
        if(tree->is_leaf()) return;
        tree->size = tree->left->size + tree->right->size;
        tree->rank = tree->left->rank + (tree->left->color == node_colors::BLACK);
        this->_derived()->pull(tree);
    }

    void push(node_pointer& tree) noexcept(NO_EXCEPT) {
        if(tree == node_handler::nil) return;
        this->clone(tree);
        this->_derived()->push(tree);
        this->pull(tree);
    }

    inline void clone(node_pointer& tree) noexcept(NO_EXCEPT) {
        tree = this->_node_handler.clone(tree);
    }


    node_pointer create(const value_type& val, const size_type size) noexcept(NO_EXCEPT) {
        if(size == 0) return node_handler::nil;
        return this->_node_handler.create(val, size);
    }

    node_pointer create(const node_colors color, node_pointer left, node_pointer right) noexcept(NO_EXCEPT) {
        this->push(left);
        this->push(right);
        return this->_node_handler.create(color, std::move(left), std::move(right));
    }


    void dispose(const node_pointer& tree) noexcept(NO_EXCEPT) {
        if(this->_node_handler.disposable(tree)) {
            this->dispose(tree->left);
            this->dispose(tree->right);

            this->_node_handler.dispose(tree);
        }
    }

  private:
    void _as_root(node_pointer& node) {
        this->clone(node);
        if(node->color == node_colors::RED) node->color = node_colors::BLACK;
        this->push(node);
    }

    void _merge(node_pointer& tree, node_pointer left, node_pointer right) noexcept(NO_EXCEPT) {
        if(left->rank < right->rank) {
            this->push(right);
            this->_merge(tree, left, right->left);

            if(right->color == node_colors::BLACK && tree->color == node_colors::RED && tree->left->color == node_colors::RED) {
                tree->color = node_colors::BLACK;

                if(right->right->color == node_colors::BLACK) {
                    this->clone(right);
                    right->color = node_colors::RED;
                    right->left = tree->right;
                    this->pull(right);

                    this->clone(tree);
                    tree->right = std::move(right);
                }
                else {
                    this->clone(right->right);
                    right->right->color = node_colors::BLACK;

                    this->clone(right);
                    right->left = tree;
                    right->color = node_colors::RED;

                    tree = std::move(right);
                }
            }
            else {
                this->clone(right);
                right->left = tree;

                tree = std::move(right);
            }
        }
        else if(left->rank > right->rank) {
            this->push(left);
            this->_merge(tree, left->right, right);

            if(left->color == node_colors::BLACK && tree->color == node_colors::RED && tree->right->color == node_colors::RED) {
                tree->color = node_colors::BLACK;

                if(left->left->color == node_colors::BLACK) {
                    this->clone(left);
                    left->color = node_colors::RED;
                    left->right = tree->left;
                    this->pull(left);

                    this->clone(tree);
                    tree->left = std::move(left);
                }
                else {
                    this->clone(left->left);
                    left->left->color = node_colors::BLACK;

                    this->clone(left);
                    left->right = tree;
                    left->color = node_colors::RED;

                    tree = std::move(left);
                }
            }
            else {
                this->clone(left);
                left->right = tree;

                tree = std::move(left);
            }
        }
        else {
            tree = this->create(node_colors::RED, left, right);
        }

        this->pull(tree);
    }

    void _split(node_pointer tree, const size_type pos, node_pointer& left, node_pointer& right) {
        if(tree == node_handler::nil) {
            left = right = node_handler::nil;
            return;
        }

        this->push(tree);

        if(tree->is_leaf()) {
            left = this->create(tree->data, pos);
            right = this->create(tree->data, tree->size - pos);
            return;
        }

        auto l = std::move(tree->left), r = std::move(tree->right);
        this->_node_handler.dispose(tree);

        if(pos < l->size) {
            this->_split(std::move(l), pos, left, right);
            this->_as_root(r);
            this->merge(right, right, std::move(r));
            this->_as_root(left);
            return;
        }
        if(pos > l->size) {
            this->_split(std::move(r), pos - l->size, left, right);
            this->_as_root(l);
            this->merge(left, std::move(l), left);
            this->_as_root(right);
            return;
        }

        left = std::move(l), right = std::move(r);
        this->_as_root(left), this->_as_root(right);
    }

  public:
    explicit red_black_tree_impl(const allocator_type& allocator= {}) noexcept(NO_EXCEPT) : _node_handler(allocator) {}


    template<std::random_access_iterator I, std::sized_sentinel_for<I> S>
        requires std::constructible_from<value_type, std::iter_value_t<I>>
    node_pointer build(I first, S last) noexcept(NO_EXCEPT) {
        if(first == last) return node_handler::nil;
        if(std::ranges::next(first) == last) return this->create(value_type{ *first }, 1);

        const auto length = std::ranges::distance(first, last);
        const auto middle = std::ranges::next(first,  std::bit_floor(to_unsigned(length - 1)));

        node_pointer tree;
        this->merge(tree, this->build(std::move(first), middle), this->build(middle, std::move(last)));

        return tree;
    }


    template<std::random_access_iterator I, std::sized_sentinel_for<I> S>
        requires
            std::constructible_from<value_type, typename std::iter_value_t<I>::first_type> &&
            std::integral<typename std::iter_value_t<I>::second_type>
    node_pointer build(I first, S last) noexcept(NO_EXCEPT) {
        if(first == last) return node_handler::nil;
        if(std::ranges::next(first) == last) return this->create(value_type{ first->first }, first->second);

        const auto length = std::ranges::distance(first, last);
        const auto middle = std::ranges::next(first, std::bit_floor(to_unsigned(length - 1)));

        node_pointer tree;
        this->merge(tree, this->build(std::move(first), middle), this->build(middle, std::move(last)));

        return tree;
    }

    void split(const node_pointer& tree, const size_type pos, node_pointer& left, node_pointer& right) noexcept(NO_EXCEPT) {
        if(pos <= 0) {
            left = node_handler::nil;
            this->merge(right, this->create(value_type{}, -pos), tree);
        }
        else if(tree->size <= pos) {
            right = node_handler::nil;
            this->merge(left, tree, this->create(value_type{}, pos - tree->size));
        }
        else {
            this->_split(tree, pos, left, right);
        }
    }


    void merge(node_pointer& tree, const node_pointer& left, const node_pointer& right) noexcept(NO_EXCEPT) {
        if(left == node_handler::nil) {
            tree = right;
            this->push(tree);
        }
        else if(right == node_handler::nil) {
            tree = left;
            this->push(tree);
        }
        else {
            this->_merge(tree, left, right);
            tree->color = node_colors::BLACK;
        }

        this->pull(tree);
    }
};


} // namespace internal


template<std::integral SizeType = i64, class NodeHandler = uni::node_handlers::reusing<std::allocator<SizeType>>>
struct red_black_tree_context {
    static constexpr bool LEAF_ONLY = true;

    template<class Derived, class ValueType = internal::dummy>
    using substance = internal::red_black_tree_impl<NodeHandler, Derived, SizeType, ValueType>;
};

template<std::integral SizeType = i64, class Allocator = std::allocator<SizeType>>
struct persistent_red_black_tree_context {
    static constexpr bool LEAF_ONLY = true;

    template<class Derived, class ValueType = internal::dummy>
    using substance = internal::red_black_tree_impl<uni::node_handlers::cloneable<Allocator>, Derived, SizeType, ValueType>;
};


namespace pmr {


template<std::integral SizeType = i64>
using red_black_tree_context = uni::red_black_tree_context<SizeType, std::pmr::polymorphic_allocator<SizeType>>;


template<std::integral SizeType = i64>
using persistent_red_black_tree_context = uni::persistent_red_black_tree_context<SizeType, std::pmr::polymorphic_allocator<SizeType>>;


} // namespace pmr


} // namespace uni
