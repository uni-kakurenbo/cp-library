#pragma once


#include <cassert>
#include <utility>
#include <type_traits>
#include <initializer_list>
#include <random>
#include <concepts>
#include <ranges>
#include <iterator>

#include "snippet/internal/types.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/uncopyable.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/concepts.hpp"
#include "internal/exception.hpp"

#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "global/constants.hpp"

#include "data_structure/internal/basic_tree_concept.hpp"
#include "data_structure/internal/tree_dumper.hpp"
#include "data_structure/internal/dynamic_tree.hpp"

#include "data_structure/treap.hpp"

#include "algebraic/internal/concepts.hpp"

#include "action/base.hpp"
#include "action/helpers.hpp"

#include "debugger/debug.hpp"


namespace uni {

namespace internal {

namespace dynamic_tree_impl {


template<class ActionOrValue, class Context>
struct sequence_core : internal::basic_core<ActionOrValue, sequence_core<ActionOrValue, Context>, Context> {
  private:
    using base = typename internal::basic_core<ActionOrValue, sequence_core, Context>;

  public:
    static constexpr bool LAZY = actions::internal::effective_action<ActionOrValue>;

    using base::base;

    using data_type = base::data_type;

    using operand = base::operand;
    using operation = base::operation;


    using node_handler = typename base::node_handler;

    using node_type = typename base::node_type;
    using node_pointer = typename base::node_pointer;


    using size_type = typename base::size_type;


    inline void pull(const node_pointer& tree) const noexcept(NO_EXCEPT) {
        if constexpr(Context::LEAF_ONLY) {
            tree->data.val = this->val(tree->left) + this->val(tree->right);
        }
        else if constexpr(LAZY) {
            tree->data.acc = tree->left->data.acc + tree->length * tree->data.val + tree->right->data.acc;
        }
    }

    inline void push(const node_pointer& tree) noexcept(NO_EXCEPT) {
        if(tree->data.rev) {
            tree->data.rev = false;

            std::swap(tree->left, tree->right);

            if(tree->left != node_handler::nil) {
                this->clone(tree->left);
                tree->left->data.rev ^= 1;
            }
            if(tree->right != node_handler::nil) {
                this->clone(tree->right);
                tree->right->data.rev ^= 1;
            }
        }

        if constexpr(LAZY) {
            if(tree->data.lazy != operation{}) {
                if constexpr(Context::LEAF_ONLY) {
                    if(tree->left != node_handler::nil) {
                        this->clone(tree->left);
                        if(!tree->left->is_leaf()) tree->left->data.lazy = tree->data.lazy + tree->left->data.lazy;
                        tree->left->data.val = ActionOrValue::map(tree->left->data.val, ActionOrValue::fold(tree->data.lazy, tree->left->size));
                    }

                    if(tree->right != node_handler::nil) {
                        this->clone(tree->right);
                        if(!tree->right->is_leaf()) tree->right->data.lazy = tree->data.lazy + tree->right->data.lazy;
                        tree->right->data.val = ActionOrValue::map(tree->right->data.val, ActionOrValue::fold(tree->data.lazy, tree->right->size));
                    }

                    tree->data.val = ActionOrValue::map(tree->data.val, tree->data.lazy);
                }
                else {
                    if(tree->left != node_handler::nil) {
                        tree->left->data.lazy = tree->data.lazy + tree->left->data.lazy;
                        tree->left->data.acc = ActionOrValue::map(tree->left->data.acc, ActionOrValue::fold(tree->data.lazy, tree->left->size));
                    }

                    if(tree->right != node_handler::nil) {
                        tree->right->data.lazy = tree->data.lazy + tree->right->data.lazy;
                        tree->right->data.acc = ActionOrValue::map(tree->right->data.acc, ActionOrValue::fold(tree->data.lazy, tree->right->size));
                    }

                    tree->data.val = ActionOrValue::map(tree->data.val, tree->data.lazy);
                }

                tree->data.lazy = operation{};
            }
        }
    }


    inline void update(const node_pointer& tree) noexcept(NO_EXCEPT) {
        if(tree == node_handler::nil) return;
        this->base::push(tree);
        this->base::pull(tree);
    }



    void insert(node_pointer& tree, const size_type pos, const operand& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(count >= 0);
        if(count == 0) return;

        node_pointer t0, t1;

        this->split(tree, pos, t0, t1);
        this->merge(tree, t0, this->create(val, count), t1);
    }

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    void insert(node_pointer& tree, const size_type pos, I first, S last) noexcept(NO_EXCEPT) {
        node_pointer t0, t1;

        this->split(tree, pos, t0, t1);
        this->merge(tree, t0, this->build(first, last), t1);
    }


    void add(node_pointer& tree, const size_type pos, const operand& val) noexcept(NO_EXCEPT) {
        node_pointer t0, t1, t2;

        this->split(tree, pos, t0, t1);
        this->split(t1, 1, t1, t2);

        if(t1 == node_handler::nil) t1 = this->create(data_type{}, 1);
        t1->data.val = val + t1->data.val;

        this->merge(tree, t0, t1, t2);
    }


    void fill(node_pointer& tree, const size_type l, const size_type r, const operand& val) noexcept(NO_EXCEPT) {
        assert(l <= r);
        if(l == r) return;

        node_pointer t0, t1, t2;
        this->split(tree, l, r, t0, t1, t2);

        this->dispose(t1);
        t1 = this->create(val, r - l);

        this->merge(tree, t0, t1, t2);
    }

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    void fill(node_pointer& tree, const size_type pos, I first, S last) noexcept(NO_EXCEPT) {
        node_pointer t0, t1, t2;

        this->split(tree, pos, pos + std::ranges::distance(first, last), t0, t1, t2);
        this->dispose(t1);
        this->merge(tree, t0, this->build(first, last), t2);
    }


    void apply(node_pointer& tree, const size_type l, const size_type r, const operation& val) noexcept(NO_EXCEPT)
        requires LAZY
    {
        assert(l <= r);
        if(l == r) return;

        node_pointer t0, t1, t2;

        this->split(tree, l, r, t0, t1, t2);

        if(t1 == node_handler::nil) t1 = this->create(data_type{}, r - l);
        t1->data.lazy = val + t1->data.lazy;

        this->merge(tree, t0, t1, t2);
    }

    void reverse(node_pointer& tree, const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        assert(l <= r);
        if(l == r) return;

        node_pointer t0, t1, t2;

        this->split(tree, l, r, t0, t1, t2);

        if(t1 != node_handler::nil) t1->data.rev ^= 1;

        this->merge(tree, t0, t1, t2);
    }


    void shift_left(node_pointer& tree, const size_type l, const size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(l <= r);

        if(count < 0) return this->shift_right(tree, l, r, -count);
        assert(l + count <= r);

        if(count == 0) return;
        if(count == r - l) return this->erase(tree, l, r);

        node_pointer t0, t1, t2, t3;

        this->split(tree, l + count, r, t1, t2, t3);
        this->split(t1, l, t0, t1);

        this->dispose(t1);

        this->merge(t2, t2, this->create({}, r - l - count));
        this->merge(tree, t0, t2, t3);
    }

    void shift_right(node_pointer& tree, const size_type l, const size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(l <= r);

        if(count < 0) return this->shift_left(tree, l, r, -count);
        assert(l + count <= r);

        if(count == 0) return;
        if(count == r - l) return this->erase(tree, l, r);

        node_pointer t0, t1, t2, t3;

        this->split(tree, l, r - count, t1, t2, t3);
        this->split(t1, l, t0, t1);

        this->dispose(t2);

        this->merge(t1, this->create({}, r - l - count), t1);
        this->merge(tree, t0, t1, t3);
    }

    void rotate(node_pointer& tree, const size_type l, const size_type m, const size_type r) noexcept(NO_EXCEPT) {
        assert(l <= m && m < r);
        if(l == m) return;

        node_pointer t0, t1, t2, t3;

        this->split(tree, m, r, t1, t2, t3);
        this->split(t1, l, t0, t1);
        this->merge(t2, t2, t1);
        this->merge(tree, t0, t2, t3);
    }


    template<bool LEFT>
    size_type find(const node_pointer& tree, operand& val, const size_type offset) noexcept(NO_EXCEPT) {
        if(tree->data.acc + val == val) {
            return -1;
        }

        if constexpr(LEFT) {
            if(tree->left != node_handler::nil and tree->left->data.acc + val != val) {
                return this->find<true>(tree->left, val, offset);
            }
            else {
                return tree->data.val + val != val ? offset + tree->left->size : this->find<true>(tree->right, val, offset + tree->left->size + 1);
            }
        }
        else {
            if(tree->right != node_handler::nil and tree->right->data.acc + val != val) {
                return this->find<false>(tree->right, val, offset + tree->left->size + 1);
            }
            else {
                return tree->data.val + val != val ? offset + tree->left->size : this->find<false>(tree->left, val, offset);
            }
        }
    }

    template<bool LEFT>
    inline size_type find(node_pointer& tree, const size_type l, const size_type r, const operand& val) noexcept(NO_EXCEPT) {
        if(l == r) return -1;

        node_pointer t0, t1, t2;

        this->split(tree, l, r, t0, t1, t2);

        const size_type res = this->find<LEFT>(t1, val, l);

        this->merge(tree, t0, t1, t2);

        return res;
    }
};



} // namespace dynamic_tree_impl

} // namespace internal


template<class ActionOrValue, class Context = treap_context<>>
    requires internal::available_with<internal::dynamic_tree_impl::sequence_core, ActionOrValue, Context>
struct dynamic_sequence
  : private internal::dumpable_tree<
        dynamic_sequence<ActionOrValue, Context>,
        internal::dynamic_tree_impl::sequence_core<ActionOrValue, Context>,
        Context::LEAF_ONLY
    >
{
  private:
    using sequence_core = internal::dynamic_tree_impl::sequence_core<ActionOrValue, Context>;

    template<class T>
    static consteval auto _to_operator() {
        if constexpr(requires { typename T::value_type; }) return typename T::value_type{};
        else return T{};
    }

  public:
    using operand = typename sequence_core::operand;
    using operation = typename sequence_core::operation;

    using value_type = operand;
    using operator_type = decltype(_to_operator<operation>());

    using node_handler = typename sequence_core::node_handler;
    using allocator_type = typename sequence_core::allocator_type;

    using node_type = typename sequence_core::node_type;
    using node_pointer = typename sequence_core::node_pointer;

    using size_type = typename sequence_core::size_type;

  private:
    using dumper = internal::dumpable_tree<dynamic_sequence, sequence_core, Context::LEAF_ONLY>;
    friend dumper;

    sequence_core _impl;

    node_pointer _root = node_handler::nil;

    size_type _offset = 0;


    template<std::same_as<size_type>... SizeTypes>
    inline void _normalize_index(SizeTypes&... indices) noexcept(NO_EXCEPT) {
        const size_type min_index = std::min({ indices... });
        ((indices -= this->_offset), ...);
        if(min_index < this->_offset) this->_offset = min_index;
    }


  public:
    ~dynamic_sequence() { this->_impl.dispose(this->_root); }

    dynamic_sequence(const allocator_type& allocator = {}) noexcept(NO_EXCEPT) : _impl(allocator) {};

    dynamic_sequence(const node_pointer& root, const size_type offset = 0, const allocator_type& allocator = {}) noexcept(NO_EXCEPT)
      : _impl(allocator), _root(root), _offset(offset)
    {};

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    dynamic_sequence(I first, S last, const allocator_type& allocator = {}) noexcept(NO_EXCEPT)
      : _impl(allocator)
    {
        this->assign(first, last);
    }


    explicit dynamic_sequence(const size_type size, const value_type& val, const allocator_type& allocator = {}) noexcept(NO_EXCEPT)
      : _impl(allocator)
    {
        this->assign(size, val);
    }

    explicit dynamic_sequence(const size_type size, const allocator_type& allocator = {}) noexcept(NO_EXCEPT)
      : dynamic_sequence(size, value_type{}, allocator)
    {}

    template<std::ranges::input_range R>
        requires (!std::same_as<std::decay_t<R>, dynamic_sequence>)
    explicit dynamic_sequence(R&& range, const allocator_type& allocator = {}) noexcept(NO_EXCEPT)
      : dynamic_sequence(ALL(range), allocator)
    {}

    template<std::convertible_to<value_type> T>
    dynamic_sequence(const std::initializer_list<T>& values, const allocator_type& allocator = {}) noexcept(NO_EXCEPT)
      : dynamic_sequence(values, allocator)
    {}

    inline auto offset() const noexcept(NO_EXCEPT) { return this->_offset; }

    inline auto& root() noexcept(NO_EXCEPT) { return this->_root; }
    inline const auto& root() const noexcept(NO_EXCEPT) { return this->_root; }

    inline auto size() const noexcept(NO_EXCEPT) { return this->_root->size; }

    inline bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }


    template<internal::resizable_range Container>
    inline auto to() noexcept(NO_EXCEPT) {
        Container res;
        res.resize(this->size());

        auto itr = std::ranges::begin(res);
        this->_impl.enumerate(this->_root, itr);

        return res;
    }


    inline void clear() noexcept(NO_EXCEPT) {
        this->_impl.dispose(this->_root);
        this->_root = node_handler::nil;
        this->_offset = 0;
    }


    inline auto clone() const noexcept { return *this; }

    inline auto clone(size_type l, size_type r) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        node_pointer t0, t1, t2;
        this->_impl.split(this->_root, l, r, t0, t1, t2);
        this->_impl.merge(this->_root, t0, t1, t2);
        return dynamic_sequence(t1, this->_offset);
    }

    inline auto split(size_type pos) noexcept(NO_EXCEPT) {
        this->_normalize_index(pos);
        node_pointer t0, t1;
        this->_impl.split(this->_root, pos, t0, t1);
        return std::make_pair(dynamic_sequence(t0, this->_offset), dynamic_sequence(t1, this->_offset));
    }

    inline auto extract(size_type l, size_type r) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        node_pointer t0, t1, t2;
        this->_impl.split(this->_root, l, r, t0, t1, t2);
        this->_impl.merge(this->_root, t0, t2);
        return dynamic_sequence(t1, this->_offset);
    }

    inline auto& insert(size_type pos, const dynamic_sequence& other) noexcept(NO_EXCEPT) {
        this->_normalize_index(pos);
        node_pointer t0, t1;
        this->_impl.split(this->_root, pos, t0, t1);
        this->_impl.merge(this->_root, t0, other._root, t1);
        return *this;
    }

    inline auto& replace(size_type l, size_type r, const dynamic_sequence& other) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        node_pointer t0, t1, t2;
        this->_impl.split(this->_root, l, r, t0, t1, t2);
        this->_impl.merge(this->_root, t0, other._root, t2);
        return *this;
    }

    inline auto& merge(const dynamic_sequence& other) noexcept(NO_EXCEPT) {
        this->_impl.merge(this->_root, this->_root, other._root);
        return *this;
    }


    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    inline auto& assign(I first, S last) noexcept(NO_EXCEPT) {
        this->clear();
        this->_root = this->_impl.build(first, last);
        return *this;
    }

    inline auto& assign(const size_type size, const value_type& val = value_type{}) noexcept(NO_EXCEPT) {
        this->clear();
        this->_impl.insert(this->_root, 0, val, size);
        return *this;
    }

    template<std::ranges::input_range R>
    inline auto& assign(R&& range) noexcept(NO_EXCEPT) {
        return this->assign(ALL(range));
    }

    template<std::convertible_to<value_type> T>
    inline auto& assign(const std::initializer_list<T>& values) noexcept(NO_EXCEPT) {
        return this->assign(values);
    }


    inline auto& resize(const size_type size, const value_type& val = value_type{}) noexcept(NO_EXCEPT) {
        if(this->size() > size) this->_impl.erase(this->_root, size, this->size());
        if(this->size() < size) this->push_back(val, size - this->size());
        return *this;
    }


    inline auto& fill(const value_type& val) noexcept(NO_EXCEPT) {
        this->_impl.fill(this->_root, 0, this->size(), val);
        return *this;
    }

    inline auto fold() noexcept(NO_EXCEPT) {
        return this->_impl.val(this->_root);
    }

    inline auto& apply(const operator_type& val) noexcept(NO_EXCEPT) {
        this->_root->data.lazy = this->_root->data.lazy + val;
        this->update(this->_root);
        return *this;
    }

    inline auto front() noexcept(NO_EXCEPT) {
        return this->_impl.fold(this->_root, 0, 1);
    }

    inline auto back() noexcept(NO_EXCEPT) {
        return this->_impl.fold(this->_root, this->size() - 1, this->size());
    }


    inline auto& push_front(const value_type& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_impl.insert(this->_root, 0, val, count);
        return *this;
    }

    inline auto& push_back(const value_type& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_impl.insert(this->_root, this->size(), val, count);
        return *this;
    }

    inline auto& reverse() noexcept(NO_EXCEPT) {
        this->_root->data.rev ^= 1;
        this->update(this->_root);
        return *this;
    }

    inline auto& shift_left(const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_impl.shift_left(this->_root, 0, this->size(), count);
        return *this;
    }

    inline auto& shift_right(const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_impl.shift_right(this->_root, 0, this->size(), count);
        return *this;
    }

    // Same usage as: std::rotate(:m:)
    inline auto& rotate(size_type m) noexcept(NO_EXCEPT) {
        this->_normalize_index(m);
        this->_impl.rotate(this->_root, 0, m, this->size());
        return *this;
    }

    // Same usage as: std::rotate(:m:)
    inline auto& rotate_left(const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(!this->empty());
        this->_impl.rotate(this->_root, 0, uni::mod(count, this->size()), this->size());
        return *this;
    }

    // Same usage as: std::rotate(:m:)
    inline auto& rotate_right(const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(!this->empty());
        this->_impl.rotate(this->_root, 0, this->size() - uni::mod(count, this->size()), this->size());
        return *this;
    }


    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    inline auto& insert(size_type pos, I first, S last) noexcept(NO_EXCEPT) {
        this->_normalize_index(pos);
        this->_impl.insert(this->_root, pos, first, last);
        return *this;
    }

    inline auto& insert(size_type pos, const operand& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_normalize_index(pos);
        this->_impl.insert(this->_root, pos, val, count);
        return *this;
    }

    inline auto& erase(size_type l, size_type r) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->_impl.erase(this->_root, l, r);
        return *this;
    }

    inline auto& erase(const size_type pos) noexcept(NO_EXCEPT) {
        return this->erase(pos, pos + 1);
    }

    inline auto pop(size_type pos, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_normalize_index(pos);
        return this->_impl.pop(this->_root, pos, count);
    }

    inline auto get(size_type pos) noexcept(NO_EXCEPT) {
        this->_normalize_index(pos);
        return this->_impl.get(this->_root, pos);
    }

    inline auto& add(size_type pos, const operation& val) noexcept(NO_EXCEPT) {
        this->_normalize_index(pos);
        this->_impl.add(this->_root, pos, val);
        return *this;
    }

    inline auto& fill(size_type l, size_type r, const value_type& val) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->_impl.fill(this->_root, l, r, val);
        return *this;
    }

    inline auto fold(size_type l, size_type r) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        return this->_impl.fold(this->_root, l, r);
    }

    inline auto& apply(size_type l, size_type r, const operation& val) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->_impl.apply(this->_root, l, r, val);
        return *this;
    }

    inline auto& reverse(size_type l, size_type r) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->_impl.reverse(this->_root, l, r);
        return *this;
    }

    inline auto& rotate(size_type l, size_type m, size_type r) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, m, r);
        this->_impl.rotate(this->_root, l, m, r);
        return *this;
    }

    inline auto& shift_left(size_type l, size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->_impl.shift_left(this->_root, l, r, count);
        return *this;
    }

    inline auto& shift_right(size_type l, size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->_impl.shift_right(this->_root, l, r, count);
        return *this;
    }


    template<std::ranges::input_range R>
        requires (!std::same_as<std::decay_t<R>, dynamic_sequence>)
    inline auto& insert(const size_type pos, R&& range) noexcept(NO_EXCEPT) {
        return this->insert(pos, ALL(range));
    }


    inline auto& set(const size_type pos, const value_type& val) noexcept(NO_EXCEPT) {
        return this->fill(pos, pos + 1, val);
    }


    inline auto pop_front(const size_type count = 1) noexcept(NO_EXCEPT) {
        return this->pop(0, count);
    }

    inline auto pop_back(const size_type count = 1) noexcept(NO_EXCEPT) {
        return this->pop(this->size() - count, count);
    }


    inline auto& apply(const size_type pos, const operator_type& val) noexcept(NO_EXCEPT) {
        return this->apply(pos, pos + 1, val);
    }


    inline auto& rotate_left(const size_type l, const size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(l < r);
        return this->rotate(l, l + uni::mod(count, r - l), r);
    }

    inline auto& rotate_right(const size_type l, const size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(l < r);
        return this->rotate(l, r - uni::mod(count, r - l), r);
    }


    // Find the min / max k in [l, r) that satisfies (this[k] + x) != x.
    // If no such k is found, return -1.
    template<bool LEFT = true>
    inline auto find(const size_type l, const size_type r, const value_type& val) noexcept(NO_EXCEPT) {
        return this->template find<LEFT>(l, r - 1, r);
    }

    // Find the min / max k in whole that satisfies (this[k] + x) != x.
    // If no such k is found, return -1.
    template<bool LEFT = true>
    inline auto find(const value_type& val) noexcept(NO_EXCEPT) {
        return this->find<LEFT>(0, this->size(), val);
    }


    struct point_reference : internal::point_reference<dynamic_sequence, size_type> {
        point_reference(dynamic_sequence *const super, const size_type pos) noexcept(NO_EXCEPT)
          : internal::point_reference<dynamic_sequence, size_type>(super, pos)
        {}

        operator value_type() noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }
        auto val() noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }


        inline auto& operator+=(const operator_type& val) noexcept(NO_EXCEPT) {
            this->_super->add(this->_pos, val);
            return *this;
        }

        inline auto& operator*=(const operator_type& val) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, val);
            return *this;
        }

        inline auto& operator=(const value_type& val) noexcept(NO_EXCEPT) {
            this->_super->set(this->_pos, val);
            return *this;
        }
    };


    struct range_reference : internal::range_reference<dynamic_sequence, size_type> {
        range_reference(dynamic_sequence *const super, const size_type l, const size_type r) noexcept(NO_EXCEPT)
          : internal::range_reference<dynamic_sequence, size_type>(super, l, r)
        {}

        inline auto clone() noexcept(NO_EXCEPT) {
            return this->_super->clone(this->_begin, this->_end);
        }

        inline auto fold() noexcept(NO_EXCEPT) {
            return this->_super->fold(this->_begin, this->_end);
        }

        inline auto& operator=(const value_type& val) noexcept(NO_EXCEPT) {
            this->_super->fill(this->_begin, this->_end, val);
            return *this;
        }

        inline auto& operator*=(const operator_type& val) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, val);
            return *this;
        }

        // Find the min / max k in [l, r) that satisfies (this[k] + x) != x.
        // If no such k is found, return -1.
        template<bool LEFT = true>
        inline auto find(const value_type& val) noexcept(NO_EXCEPT) {
            return this->_super->template find<LEFT>(this->_begin, this->_end, val);
        }
    };


    inline auto operator[](const size_type pos) noexcept(NO_EXCEPT) { return point_reference(this, pos); }
    inline auto operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) { return range_reference(this, l, r); }


    struct iterator;

  protected:
    using iterator_interface = internal::container_iterator_interface<value_type, dynamic_sequence, iterator>;

  public:
    struct iterator : iterator_interface {
        iterator() noexcept = default;
        iterator(dynamic_sequence *const ref, const size_type pos) noexcept(NO_EXCEPT) : iterator_interface(ref, pos) {}

        inline auto operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    };

    inline auto begin() noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline auto end() noexcept(NO_EXCEPT) { return iterator(this, this->size()); }


    using dumper::dump_rich;
    using dumper::_debug;


    debugger::debug_t dump_rich(const std::string prefix = "   ") {
        return "\n" + this->dump_rich(this->_root, prefix);
    }


    debugger::debug_t _debug() {
        return "[ " + this->_debug(this->_root) + " ]";
    }

};


} // namespace uni
