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

#include "data_structure/internal/tree_interface.hpp"
#include "data_structure/treap.hpp"

#include "algebraic/internal/concepts.hpp"

#include "action/base.hpp"
#include "action/helpers.hpp"

#include "debugger/debug.hpp"


namespace lib {

namespace internal {

namespace dynamic_set_impl {


namespace internal {


template<class Operand>
struct data_type {
    Operand val, acc;

    data_type() noexcept = default;
    data_type(const Operand& _val) noexcept(NO_EXCEPT) : val(_val) {}

    friend bool operator==(const data_type& lhs, const data_type& rhs) noexcept(NO_EXCEPT) {
        return lhs.val == rhs.val;
    }

    friend bool operator<(const data_type& lhs, const data_type& rhs) noexcept(NO_EXCEPT) {
        return lhs.val < rhs.val;
    }

    auto _debug() const { return this->val; }
};


} // namespace internal


template<actions::internal::full_action Action, class Context>
struct core : Context::interface<core<Action, Context>, internal::data_type<typename Action::operand>> {
    using action = Action;
    using operand = Action::operand;

  private:
    using data_type = internal::data_type<operand>;

    using interface = typename Context::interface<core, data_type>;
    static_assert(tree_interface<interface>);

    friend interface;

  protected:
    using node_type = typename interface::node_type;
    using node_pointer = typename interface::node_pointer;

  public:
    using size_type = typename interface::size_type;

  private:
    inline void pushup(const node_pointer tree) const noexcept(NO_EXCEPT) {
        tree->data.acc = tree->left->data.acc + tree->length * tree->data.val + tree->right->data.acc;
    }

    inline void pushdown(const node_pointer) const noexcept(NO_EXCEPT) { /* do nothing */ }


    template<std::random_access_iterator I, std::sized_sentinel_for<I> S>
        requires std::convertible_to<std::iter_value_t<I>, operand>
    node_pointer _build(I first, S last) noexcept(NO_EXCEPT) {
        if(first == last) return node_type::nil;

        const auto length = std::ranges::distance(first, last);
        const auto middle = std::next(first, length >> 1);

        node_pointer tree = this->create_node(operand{ *middle }, 1);
        tree->left = this->_build(first, middle);
        tree->right = this->_build(std::next(middle), last);

        this->interface::pushup(tree);

        return tree;
    }


    template<std::random_access_iterator I, std::sized_sentinel_for<I> S>
        requires
            std::convertible_to<typename std::iter_value_t<I>::first_type, operand> &&
            std::integral<typename std::iter_value_t<I>::second_type>
    node_pointer _build(I first, S last) noexcept(NO_EXCEPT) {
        if(first == last) return node_type::nil;

        const auto length = std::ranges::distance(first, last);
        const auto middle = std::ranges::next(first, length >> 1);

        node_pointer tree = this->create_node(operand{ middle->first }, middle->second );
        tree->left = this->_build(first, middle);
        tree->right = this->_build(std::next(middle), last);

        this->interface::pushup(tree);

        return tree;
    }

  public:
    using interface::interface;


    template<std::random_access_iterator I, std::sized_sentinel_for<I> S>
    node_pointer build(I first, S last) {
        std::vector<i64> val;
        val.assign(first, last);
        std::ranges::sort(val);

        const auto tree = this->_build(ALL(val));
        debug(this->dump_rich(tree));

        interface::rectify(tree);

        return tree;
    }


    using interface::split;
    using interface::merge;

    inline void split(const node_pointer tree, const size_type l, const size_type r, node_pointer& t0, node_pointer& t1, node_pointer& t2) noexcept(NO_EXCEPT) {
        this->split(tree, r, t1, t2);
        this->split(t1, l, t0, t1);
    }

    inline void merge(node_pointer& tree, node_pointer t0, const node_pointer t1, const node_pointer t2) noexcept(NO_EXCEPT) {
        this->merge(t0, t0, t1);
        this->merge(tree, t0, t2);
    }


    void insert(node_pointer& tree, const operand& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        if(count == 0) return;
        if(count < 0) return this->erase(tree, val, -count);

        node_pointer t0, t1;

        this->split(tree, { val }, t0, t1);
        this->merge(tree, t0, this->create_node(val, count), t1);
    }

    void insert_unique(node_pointer& tree, const operand& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        if(count == 0) return;
        if(count < 0) return this->erase(tree, val, -count);

        node_pointer t0, t1, t2;

        this->split(tree, { val }, t0, t1);
        this->template split<true>(t1, { val }, t1, t2);

        if(t1 == node_type::nil) t1 = this->create_node(val, count);

        this->merge(tree, t0, t1, t2);
    }


    void erase(node_pointer& tree, const operand& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        if(count == 0) return;
        if(count < 0) return this->insert(tree, val, -count);

        node_pointer t0, t1, t2;

        this->split(tree, { val }, t0, t1);
        this->split(t1, count, t1, t2);

        this->delete_tree(t1);
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

        this->delete_tree(t1);
        this->merge(t2, t0, t2);
        this->merge(tree, t2, t3);
    }


    void erase(node_pointer& tree, const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        assert(0 <= l && l <= r && r <= tree->size);
        node_pointer t0, t1, t2;

        this->split(tree, l, r, t0, t1, t2);

        this->delete_tree(t1);
        this->merge(tree, t0, t1, t2);
    }


    auto pop(node_pointer& tree, const size_type pos, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(0 <= pos && pos + count <= tree->size);
        if(count == 0) return operand{};

        node_pointer t0, t1, t2;

        this->split(tree, pos, t0, t1);
        this->split(t1, count, t1, t2);

        const auto res = t1->data.acc;

        this->delete_tree(t1);
        this->merge(tree, t0, t2);

        return res;
    }


    operand fold(node_pointer& tree, const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        assert(0 <= l && l <= r && r <= tree->size);
        if(l == r) return operand{};

        node_pointer t0, t1, t2;

        this->split(tree, l, r, t0, t1, t2);

        const operand res = t1 ? t1->data.acc : operand{};

        this->merge(tree, t0, t1, t2);

        return res;
    }

    template<bool STRICT = false>
    spair<size_type> find(node_type& tree, const operand& val) noexcept(NO_EXCEPT) {
        node_pointer t0, t1;

        this->template split<STRICT>(tree, { val }, t0, t1);

        const size_type res = t0->size;

        this->merge(tree, t0, t1);

        return res;
    }

    spair<size_type> equal_range(node_pointer& tree, const operand& val) noexcept(NO_EXCEPT) {
        node_pointer t0, t1, t2;

        this->template split<true>(tree, { val }, t1, t2);
        this->split(t1, { val }, t0, t1);

        const size_type lower = t0->size;

        this->merge(t1, t0, t1);

        const size_type upper = t1->size;

        this->merge(tree, t1, t2);

        return { lower, upper };
    }


    debugger::debug_t dump_rich(const node_pointer tree, const std::string prefix, const int dir, size_type& index) const {
        if(!tree || tree == node_type::nil) return prefix + "\n";

        this->pushdown(tree);

        const auto left = this->dump_rich(tree->left, prefix + (dir == 1 ? "| " : "  "), -1, index);
        const auto here = prefix + "--+ " + debugger::dump(index) + " : " + debugger::dump(tree->data) + " [" + debugger::dump(tree->length) + "]\n";
        index += tree->length;

        const auto right = this->dump_rich(tree->right, prefix + (dir == -1 ? "| " : "  "), 1, index);

        return left + here + right;
    }

    inline debugger::debug_t dump_rich(const node_pointer tree, const std::string prefix = "   ", const int dir = 0) const {
        size_type index = 0;
        return this->dump_rich(tree, prefix, dir, index);
    }

    debugger::debug_t _debug(const node_pointer tree) const {
        if(!tree || tree == node_type::nil) return "";

        this->pushdown(tree);

        return
            "(" +
            this->_debug(tree->left) + " " +
            debugger::dump(tree->data) + " [" +
            debugger::dump(tree->length) + "] " +
            this->_debug(tree->right) +
            ")";
    }
};



} // namespace dynamic_set_impl

} // namespace internal


template<class Value, class Context = treap_context<>>
struct dynamic_set : dynamic_set<actions::make_full_t<Value>, Context> {
    using dynamic_set<actions::make_full_t<Value>, Context>::dynamic_set;
};


template<actions::internal::full_action Action, class Context>
    requires internal::available_with<internal::dynamic_set_impl::core, Action, Context>
struct dynamic_set<Action, Context> : private internal::dynamic_set_impl::core<Action, Context> {
  public:
    using action = Action;

    using operand = typename action::operand;

    using value_type = operand;

    using core = internal::dynamic_set_impl::core<Action, Context>;

    using node_type = typename core::node_type;
    using node_pointer = typename core::node_pointer;

  private:
    using allocator_type = typename core::allocator_type;

  public:
    using size_type = typename core::size_type;

  private:
    node_pointer _root = node_type::nil;

    size_type _offset = 0;


  public:
    ~dynamic_set() { this->delete_tree(this->_root); }

    dynamic_set(const allocator_type& alloc = {}) noexcept(NO_EXCEPT) : core(alloc) {};

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    dynamic_set(I first, S last, const allocator_type& alloc = {}) noexcept(NO_EXCEPT)
      : dynamic_set(alloc)
    {
        this->assign(first, last);
    }


    explicit dynamic_set(const size_type size, const value_type& val, const allocator_type& alloc = {}) noexcept(NO_EXCEPT)
      : dynamic_set(alloc)
    {
        this->assign(size, val);
    }

    explicit dynamic_set(const size_type size, const allocator_type& alloc = {}) noexcept(NO_EXCEPT)
      : dynamic_set(size, value_type{}, alloc)
    {}

    template<std::ranges::input_range R>
    explicit dynamic_set(R&& range, const allocator_type& alloc = {}) noexcept(NO_EXCEPT)
      : dynamic_set(ALL(range), alloc)
    {}

    template<std::convertible_to<value_type> T>
    dynamic_set(const std::initializer_list<T>& values, const allocator_type& alloc = {}) noexcept(NO_EXCEPT)
      : dynamic_set(values, alloc)
    {}


    size_type size() const noexcept(NO_EXCEPT) { return this->_root->size; }

    bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }


    inline void clear() noexcept(NO_EXCEPT) {
        this->delete_tree(this->_root);
        this->_root = node_type::nil;
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
        if constexpr(UNIQUE) this->core::insert_unique(this->_root, val, count);
        else this->core::insert(this->_root, val, count);

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
        if constexpr(LIMIT) this->core::erase_limit(this->_root, val, count);
        else this->core::erase(this->_root, val, count);

        return *this;
    }

    inline auto& erase(const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        this->core::erase(this->_root, l, r);
        return *this;
    }


    inline value_type pop(const size_type pos, const size_type count = 1) noexcept(NO_EXCEPT) {
        return this->core::pop(this->_root, pos, count);
    }

    inline value_type pop_min(const size_type count = 1) noexcept(NO_EXCEPT) {
        return this->pop(0, count);
    }

    inline value_type pop_max(const size_type count = 1) noexcept(NO_EXCEPT) {
        return this->pop(this->size() - count, count);
    }


    inline operand fold(const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        return this->core::fold(this->_root, l, r);
    }

    inline value_type get(const size_type k) noexcept(NO_EXCEPT) {
        return this->fold(k, k + 1);
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

    inline value_type fold() noexcept(NO_EXCEPT) {
        return this->_root->data.acc;
    }


    inline value_type operator[](const size_type k) noexcept(NO_EXCEPT) {
        return this->get(k);
    }

    inline value_type operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        return this->fold(l, r);
    }


    struct iterator;


    inline size_type lower_bound(const value_type& val) noexcept(NO_EXCEPT) {
        return iterator{ this, this->find(this->_root, val) };
    }

    inline size_type upper_bound(const value_type& val) noexcept(NO_EXCEPT) {
        return iterator{ this, this->template find<true>(this->_root, val) };
    }

    inline auto equal_range(const value_type& val) noexcept(NO_EXCEPT) {
        const auto [ lower, upper ] = this->core::equal_range(this->_root, val);
        return std::make_pair(iterator{ this, lower }, iterator{ this, upper });
    }


    inline size_type count(const value_type& val) noexcept(NO_EXCEPT) {
        const auto [ lower, upper ] = this->core::equal_range(this->_root, val);
        return upper - lower;
    }

    inline bool contains(const value_type& val) noexcept(NO_EXCEPT) {
        return this->count(val) == 0;
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
        iterator() noexcept = default;
        iterator(dynamic_set *const ref, const size_type k) noexcept(NO_EXCEPT) : iterator_interface(ref, k) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() noexcept(NO_EXCEPT) { return iterator{ this, 0 }; }
    inline iterator end() noexcept(NO_EXCEPT) { return iterator{ this, this->size() }; }


    using core::dump_rich;
    using core::_debug;


    debugger::debug_t dump_rich(const std::string prefix = "   ") const {
        return "\n" + this->dump_rich(this->_root, prefix);
    }


    debugger::debug_t _debug() const {
        return "[ " + this->_debug(this->_root) + " ]";
    }
};


} // namespace lib
