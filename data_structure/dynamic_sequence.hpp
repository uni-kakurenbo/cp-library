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
#include "internal/ranges.hpp"

#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "global/constants.hpp"

#include "data_structure/internal/tree_interface.hpp"
#include "data_structure/internal/tree_dumper.hpp"
#include "data_structure/internal/tree_enumerator.hpp"

#include "data_structure/treap.hpp"

#include "algebraic/internal/concepts.hpp"

#include "action/base.hpp"
#include "action/helpers.hpp"

#include "adaptor/vector.hpp"

#include "debugger/debug.hpp"


namespace lib {

namespace internal {

namespace dynamic_sequence_impl {


namespace internal {


template<class ValueType>
struct data_type {
    ValueType val;
    bool rev = false;

    data_type() noexcept = default;
    data_type(const ValueType& _val) noexcept(NO_EXCEPT) : val(_val) {}

    auto _debug() const { return this->val; }
};


} // namespace internal


template<class ValueType, class Context>
struct core
  : Context::interface<
        core<ValueType, Context>,
        internal::data_type<ValueType>
    >,
    enumerable_tree<
        core<ValueType, Context>,
        typename Context::interface<core<ValueType, Context>, internal::data_type<ValueType>>,
        ValueType,
        Context::LEAF_ONLY
    >
{
    using value_type = ValueType;

    using data_type = internal::data_type<value_type>;


    using interface = typename Context::interface<core, data_type>;
    static_assert(tree_interface<interface>);

    friend interface;


    using node_handler = typename interface::node_handler;

    using node_type = typename interface::node_type;
    using node_pointer = typename interface::node_pointer;


    using size_type = typename interface::size_type;


    inline value_type get(const node_pointer& node) const noexcept(NO_EXCEPT) {
        if constexpr(Context::LEAF_ONLY) {
            if(node->is_leaf()) return node->size * node->data.val;
            return node->data.val;
        }
        else {
            return node->data.val;
        }
    }


    template<class... Args>
    inline constexpr void pull(Args&&...) const noexcept {}

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
    }


    template<class... Args>
    inline void update(Args&&...) const noexcept(NO_EXCEPT) {}

    using interface::interface;

    using interface::split;
    using interface::merge;

    inline void split(const node_pointer& tree, const size_type l, const size_type r, node_pointer& t0, node_pointer& t1, node_pointer& t2) noexcept(NO_EXCEPT) {
        this->split(tree, r, t1, t2);
        this->split(t1, l, t0, t1);
    }

    inline void merge(node_pointer& tree, node_pointer t0, const node_pointer& t1, const node_pointer& t2) noexcept(NO_EXCEPT) {
        this->merge(t0, t0, t1);
        this->merge(tree, t0, t2);
    }

    void insert(node_pointer& tree, const size_type pos, const value_type& val, const size_type count = 1) noexcept(NO_EXCEPT) {
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


    void erase(node_pointer& tree, const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        assert(l <= r);
        node_pointer t0, t1, t2;

        this->split(tree, l, r, t0, t1, t2);
        this->dispose(t1);
        this->merge(tree, t0, t2);
    }

    auto pop(node_pointer& tree, const size_type pos, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(0 <= pos && 0 <= count);
        if(count == 0) return value_type{};

        node_pointer t0, t1, t2;

        this->split(tree, pos, t0, t1);
        this->split(t1, count, t1, t2);

        const auto res = this->get(t1);

        this->dispose(t1);
        this->merge(tree, t0, t2);

        return res;
    }


    void fill(node_pointer& tree, const size_type l, const size_type r, const value_type& val) noexcept(NO_EXCEPT) {
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


    value_type get(node_pointer tree, const size_type pos) noexcept(NO_EXCEPT) {
        if(tree == node_handler::nil || pos < 0 || pos >= tree->size) return value_type{};

        this->interface::push(tree);

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
};



} // namespace dynamic_sequence_impl

} // namespace internal


template<class ValueType, class Context = treap_context<>>
    requires internal::available_with<internal::dynamic_sequence_impl::core, ValueType, Context>
struct dynamic_sequence
  : private internal::dumpable_tree<
        dynamic_sequence<ValueType, Context>,
        internal::dynamic_sequence_impl::core<ValueType, Context>,
        Context::LEAF_ONLY
    >
{
  public:
    using value_type = ValueType;

    using core = internal::dynamic_sequence_impl::core<value_type, Context>;

    using node_handler = typename core::node_handler;
    using allocator_type = typename core::allocator_type;

    using node_type = typename core::node_type;
    using node_pointer = typename core::node_pointer;

    using size_type = typename core::size_type;

  private:
    using dumper = internal::dumpable_tree<dynamic_sequence, core, Context::LEAF_ONLY>;
    friend dumper;

    core _impl;

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


    inline size_type offset() const noexcept(NO_EXCEPT) { return this->_offset; }

    inline node_pointer& root() noexcept(NO_EXCEPT) { return this->_root; }
    inline const node_pointer& root() const noexcept(NO_EXCEPT) { return this->_root; }

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_root->size; }

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


    inline value_type front() noexcept(NO_EXCEPT) {
        return this->_impl.fold(this->_root, 0, 1);
    }

    inline value_type back() noexcept(NO_EXCEPT) {
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
        this->_impl.rotate(this->_root, 0, lib::mod(count, this->size()), this->size());
        return *this;
    }

    // Same usage as: std::rotate(:m:)
    inline auto& rotate_right(const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(!this->empty());
        this->_impl.rotate(this->_root, 0, this->size() - lib::mod(count, this->size()), this->size());
        return *this;
    }


    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    inline auto& insert(size_type pos, I first, S last) noexcept(NO_EXCEPT) {
        this->_normalize_index(pos);
        this->_impl.insert(this->_root, pos, first, last);
        return *this;
    }

    inline auto& insert(size_type pos, const value_type& val, const size_type count = 1) noexcept(NO_EXCEPT) {
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

    inline auto& fill(size_type l, size_type r, const value_type& val) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->_impl.fill(this->_root, l, r, val);
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

    inline value_type get(const size_type pos) noexcept(NO_EXCEPT) {
        return this->_impl.get(this->_root, pos);
    }


    inline auto pop_front(const size_type count = 1) noexcept(NO_EXCEPT) {
        return this->pop(0, count);
    }

    inline auto pop_back(const size_type count = 1) noexcept(NO_EXCEPT) {
        return this->pop(this->size() - count, count);
    }


    inline auto& rotate_left(const size_type l, const size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(l < r);
        return this->rotate(l, l + lib::mod(count, r - l), r);
    }

    inline auto& rotate_right(const size_type l, const size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(l < r);
        return this->rotate(l, r - lib::mod(count, r - l), r);
    }


    struct point_reference : internal::point_reference<dynamic_sequence, size_type> {
        point_reference(dynamic_sequence *const super, const size_type pos) noexcept(NO_EXCEPT)
          : internal::point_reference<dynamic_sequence, size_type>(super, pos)
        {}

        operator value_type() noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }
        value_type val() noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }

        inline point_reference& set(const value_type& val) noexcept(NO_EXCEPT) {
            this->_super->set(this->_pos, val);
            return *this;
        }

        inline point_reference& operator=(const value_type& val) noexcept(NO_EXCEPT) {
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

        inline range_reference& fill(const value_type& val) noexcept(NO_EXCEPT) {
            this->_super->fill(this->_begin, this->_end, val);
            return *this;
        }
        inline range_reference& operator=(const value_type& val) noexcept(NO_EXCEPT) {
            this->_super->fill(this->_begin, this->_end, val);
            return *this;
        }
    };


    inline point_reference operator[](const size_type pos) noexcept(NO_EXCEPT) {
        return point_reference(this, pos);
    }

    inline range_reference operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        return range_reference(this, l, r);
    }


    struct iterator;

  protected:
    using iterator_interface = internal::container_iterator_interface<value_type, dynamic_sequence, iterator>;

  public:
    struct iterator : iterator_interface {
        iterator() noexcept = default;
        iterator(dynamic_sequence *const ref, const size_type pos) noexcept(NO_EXCEPT) : iterator_interface(ref, pos) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline iterator end() noexcept(NO_EXCEPT) { return iterator(this, this->size()); }


    using dumper::dump_rich;
    using dumper::_debug;


    debugger::debug_t dump_rich(const node_pointer& tree, const std::string prefix = "   ", const int dir = 0) {
        size_type index = this->_offset;
        return this->dump_rich(tree, prefix, dir, index);
    }

    debugger::debug_t dump_rich(const std::string prefix = "   ") {
        return "\n" + this->dump_rich(this->_root, prefix);
    }


    debugger::debug_t _debug() {
        return "[ " + this->_debug(this->_root) + " ]";
    }

};


} // namespace lib
