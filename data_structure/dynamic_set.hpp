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
    requires (!Context::LEAF_ONLY)
struct set_core :  internal::basic_core<ActionOrValue, set_core<ActionOrValue, Context>, Context> {
  private:
    using base = internal::basic_core<ActionOrValue, set_core, Context>;

  public:
    using base::base;


    using data_type = base::data_type;

    using operand = base::operand;


    using node_handler = typename base::node_handler;

    using node_type = typename base::node_type;
    using node_pointer = typename base::node_pointer;


    using size_type = typename base::size_type;


    inline void pull(const node_pointer tree) const noexcept(NO_EXCEPT) {
        tree->data.acc = tree->left->data.acc + tree->length * tree->data.val + tree->right->data.acc;
    }

    inline constexpr void push(const node_pointer) const noexcept(NO_EXCEPT) { /* do nothing */ }


    template<std::random_access_iterator I, std::sized_sentinel_for<I> S>
    node_pointer build(I first, S last) {
        std::vector<i64> val;
        val.assign(first, last);
        std::ranges::sort(val);

        return this->base::build(ALL(val));
    }


    void insert(node_pointer& tree, const operand& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        if(count == 0) return;
        if(count < 0) return this->erase(tree, val, -count);

        node_pointer t0, t1;

        this->split(tree, { val }, t0, t1);
        this->merge(tree, t0, this->create(val, count), t1);
    }

    void insert_unique(node_pointer& tree, const operand& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        if(count == 0) return;
        if(count < 0) return this->erase(tree, val, -count);

        node_pointer t0, t1;
        bool exist = false;

        this->template split<false, true>(tree, { val }, t0, t1, &exist);

        if(exist) this->merge(tree, t0, t1);
        else this->merge(tree, t0, this->create(val, count), t1);
    }


    void erase(node_pointer& tree, const operand& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        if(count == 0) return;
        if(count < 0) return this->insert(tree, val, -count);

        node_pointer t0, t1, t2;

        this->split(tree, { val }, t0, t1);
        this->split(t1, count, t1, t2);

        this->dispose(t1);
        this->merge(tree, t0, t2);
    }

    void erase_limit(node_pointer& tree, const operand& val, size_type count = 1) noexcept(NO_EXCEPT) {
        if(count == 0) return;
        if(count < 0) return this->insert(tree, val, -count);

        node_pointer t0, t1, t2, t3;

        this->split(tree, { val }, t0, t1);
        this->template split<true>(t1, { val }, t1, t3);

        if(count >= t1->size) count = t1->size;
        this->split(t1, count, t1, t2);

        this->dispose(t1);
        this->merge(t2, t0, t2);
        this->merge(tree, t2, t3);
    }


    auto fold(node_pointer tree, const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        assert(0 <= l && l <= r && r <= tree->size);
        return this->base::fold(tree, l, r);
    }

    void erase(node_pointer& tree, const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        assert(0 <= l && l <= r && r <= tree->size);
        this->base::erase(tree, l, r);
    }


    auto pop(node_pointer& tree, const size_type pos, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(0 <= pos && 0 <= count && pos + count <= tree->size);
        return this->base::pop(tree, pos, count);
    }

    auto get(node_pointer tree, const size_type pos) noexcept(NO_EXCEPT) {
        assert(0 <= pos && pos < tree->size);
        return this->base::get(tree, pos);
    }

    template<bool STRICT = false>
    auto find(node_pointer& tree, const operand& val) noexcept(NO_EXCEPT) {
        node_pointer t0, t1;

        this->template split<STRICT>(tree, { val }, t0, t1);

        const auto res = t0->size;

        this->merge(tree, t0, t1);

        return res;
    }

    auto equal_range(node_pointer& tree, const operand& val) noexcept(NO_EXCEPT) {
        node_pointer t0, t1, t2;

        this->template split<true>(tree, { val }, t1, t2);
        this->split(t1, { val }, t0, t1);

        const auto lower = t0->size;

        this->merge(t1, t0, t1);

        const auto upper = t1->size;

        this->merge(tree, t1, t2);

        return std::make_pair(std::move(lower), std::move(upper));
    }
};



} // namespace dynamic_tree_impl

} // namespace internal


template<class Value, class Context = treap_context<>>
struct dynamic_set : dynamic_set<actions::make_full_t<Value>, Context> {
    using dynamic_set<actions::make_full_t<Value>, Context>::dynamic_set;
};


template<actions::internal::full_action ActionOrValue, class Context>
    requires internal::available_with<internal::dynamic_tree_impl::set_core, ActionOrValue, Context>
struct dynamic_set<ActionOrValue, Context>
  : private internal::dumpable_tree<
        dynamic_set<ActionOrValue, Context>,
        internal::dynamic_tree_impl::set_core<ActionOrValue, Context>,
        Context::LEAF_ONLY
    >
{
  public:
    using action = ActionOrValue;
    using operand = typename action::operand;

    using value_type = operand;

    using set_core = internal::dynamic_tree_impl::set_core<ActionOrValue, Context>;

    using node_handler = typename set_core::node_handler;
    using allocator_type = typename set_core::allocator_type;

    using node_type = typename set_core::node_type;
    using node_pointer = typename set_core::node_pointer;

    using size_type = typename set_core::size_type;

  private:
    using dumper = internal::dumpable_tree<dynamic_set, set_core, Context::LEAF_ONLY>;
    friend dumper;

    set_core _impl;

    node_pointer _root = node_handler::nil;

    size_type _offset = 0;


  public:
    ~dynamic_set() { this->_impl.dispose(this->_root); }

    dynamic_set(const allocator_type& allocator = allocator_type()) noexcept(NO_EXCEPT) : _impl(allocator) {};

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    dynamic_set(I first, S last, const allocator_type& allocator = allocator_type()) noexcept(NO_EXCEPT)
      : _impl(allocator)
    {
        this->assign(first, last);
    }


    explicit dynamic_set(const size_type size, const value_type& val, const allocator_type& allocator = allocator_type()) noexcept(NO_EXCEPT)
      : _impl(allocator)
    {
        this->assign(size, val);
    }

    explicit dynamic_set(const size_type size, const allocator_type& allocator = allocator_type()) noexcept(NO_EXCEPT)
      : dynamic_set(size, value_type{}, allocator)
    {}

    template<std::ranges::input_range R>
    explicit dynamic_set(R&& range, const allocator_type& allocator = allocator_type()) noexcept(NO_EXCEPT)
      : dynamic_set(ALL(range), allocator)
    {}

    template<std::convertible_to<value_type> T>
    dynamic_set(const std::initializer_list<T>& values, const allocator_type& allocator = allocator_type()) noexcept(NO_EXCEPT)
      : dynamic_set(values, allocator)
    {}


    inline node_pointer& root() noexcept(NO_EXCEPT) { return this->_root; }
    inline const node_pointer& root() const noexcept(NO_EXCEPT) { return this->_root; }

    size_type size() const noexcept(NO_EXCEPT) { return this->_root->size; }

    bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }


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
    }


    template<bool UNIQUE = false, std::input_iterator I, std::sized_sentinel_for<I> S>
    inline auto& assign(I first, S last) noexcept(NO_EXCEPT) {
        this->clear();
        this->insert<UNIQUE>(first, last);
        return *this;
    }

    template<bool UNIQUE = false>
    inline auto& assign(const size_type size, const value_type& val = value_type{}) noexcept(NO_EXCEPT) {
        this->clear();
        this->insert<UNIQUE>(val, size);
        return *this;
    }

    template<bool UNIQUE = false, std::ranges::input_range R>
    inline auto& assign(R&& range) noexcept(NO_EXCEPT) {
        return this->assign<UNIQUE>(ALL(range));
    }

    template<bool UNIQUE = false, std::convertible_to<value_type> T>
    inline auto& assign(const std::initializer_list<T>& values) noexcept(NO_EXCEPT) {
        return this->assign<UNIQUE>(values);
    }


    template<bool UNIQUE = false>
    inline auto& insert(const operand& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        if constexpr(UNIQUE) this->_impl.insert_unique(this->_root, val, count);
        else this->_impl.insert(this->_root, val, count);

        return *this;
    }

    template<bool UNIQUE = false, std::input_iterator I, std::sized_sentinel_for<I> S>
    inline auto& insert(I first, S last) noexcept(NO_EXCEPT) {
        for(; first != last; ++first) this->template insert<UNIQUE>(*first);
        return *this;
    }

    template<bool UNIQUE = false, std::ranges::input_range R>
    inline auto& insert(R&& range) noexcept(NO_EXCEPT) {
        return this->template insert<UNIQUE>(ALL(range));
    }

    inline auto& fill(const value_type& val) noexcept(NO_EXCEPT) {
        const size_type size = this->size();
        this->clear();
        this->insert(val, size);
        return *this;
    }


    template<bool LIMIT = true>
    inline auto& erase(const value_type& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        if constexpr(LIMIT) this->_impl.erase_limit(this->_root, val, count);
        else this->_impl.erase(this->_root, val, count);

        return *this;
    }

    inline auto& erase(const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        this->_impl.erase(this->_root, l, r);
        return *this;
    }


    inline value_type pop(const size_type pos, const size_type count = 1) noexcept(NO_EXCEPT) {
        return this->_impl.pop(this->_root, pos, count);
    }

    inline value_type pop_min(const size_type count = 1) noexcept(NO_EXCEPT) {
        return this->pop(0, count);
    }

    inline value_type pop_max(const size_type count = 1) noexcept(NO_EXCEPT) {
        return this->pop(this->size() - count, count);
    }


    inline operand fold(const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        return this->_impl.fold(this->_root, l, r);
    }

    inline value_type get(const size_type k) noexcept(NO_EXCEPT) {
        return this->_impl.get(this->_root, k);
    }

    inline operand median(const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        return this->get((r - l) >> 1);
    }

    inline value_type min() noexcept(NO_EXCEPT) {
        return this->get(0);
    }

    inline value_type median() noexcept(NO_EXCEPT) {
        return this->get(this->size() >> 1);
    }

    inline value_type max() noexcept(NO_EXCEPT) {
        return this->get(this->size() - 1);
    }

    inline auto fold() noexcept(NO_EXCEPT) {
        return this->_root->data.acc;
    }


    inline value_type operator[](const size_type k) noexcept(NO_EXCEPT) {
        return this->get(k);
    }

    inline value_type operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        return this->fold(l, r);
    }


    struct iterator;


    inline auto lower_bound(const value_type& val) noexcept(NO_EXCEPT) {
        return iterator{ this, this->_impl.template find<false>(this->_root, val) };
    }

    inline auto upper_bound(const value_type& val) noexcept(NO_EXCEPT) {
        return iterator{ this, this->_impl.template find<true>(this->_root, val) };
    }

    inline auto equal_range(const value_type& val) noexcept(NO_EXCEPT) {
        const auto [ lower, upper ] = this->_impl.equal_range(this->_root, val);
        return std::make_pair(iterator{ this, lower }, iterator{ this, upper });
    }


    inline size_type count(const value_type& val) noexcept(NO_EXCEPT) {
        const auto [ lower, upper ] = this->_impl.equal_range(this->_root, val);
        return upper - lower;
    }

    inline bool contains(const value_type& val) noexcept(NO_EXCEPT) {
        return this->count(val) == 1;
    }


    inline auto& erase(const iterator& itr) noexcept(NO_EXCEPT) {
        return this->erase(itr.pos(), itr.pos() + 1);
    }

    inline auto& erase(const iterator& l, const iterator& r) noexcept(NO_EXCEPT) {
        return this->erase(l.pos(), r.pos());
    }


  protected:
    using iterator_interface = internal::container_iterator_interface<value_type,dynamic_set,iterator>;

  public:
    struct iterator : iterator_interface {
        using iterator_interface::iterator_interface;
    };

    inline auto begin() noexcept(NO_EXCEPT) { return iterator{ this, 0 }; }
    inline auto end() noexcept(NO_EXCEPT) { return iterator{ this, this->size() }; }

    inline auto rbegin() const noexcept(NO_EXCEPT) { return std::make_reverse_iterator(this->end()); }
    inline auto rend() const noexcept(NO_EXCEPT) { return std::make_reverse_iterator(this->begin()); }


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
