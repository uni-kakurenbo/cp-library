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

namespace dynamic_sequence_impl {


template<class Operand, class Operation>
struct data_type {
    Operand val, acc;
    Operation lazy;
    bool rev = false;

    data_type() noexcept = default;
    data_type(const Operand& _val) noexcept(NO_EXCEPT) : val(_val) {}

    auto _debug() const { return this->val; }
};


template<actions::internal::full_action Action, class Context>
struct core : Context::interface<core<Action, Context>, tree_indexing_policy::implicit_key, data_type<typename Action::operand, typename Action::operation>> {
    using action = Action;
    using operand = Action::operand;
    using operation = Action::operation;

  private:
    using interface = typename Context::interface<core, tree_indexing_policy::implicit_key, data_type<operand, operation>>;
    static_assert(tree_interface<interface>);

    friend interface;

  protected:
    using node_type = typename interface::node_type;
    using node_pointer = typename interface::node_pointer;

  public:
    using size_type = typename interface::size_type;

  private:
    inline static operand val(const node_pointer tree) noexcept(NO_EXCEPT) { return tree ? tree->length * tree->data.val : operand{}; }

    inline static operand acc(const node_pointer tree) noexcept(NO_EXCEPT) { return tree ? tree->data.acc : operand{}; }

    inline static void pushup(const node_pointer tree) noexcept(NO_EXCEPT) {
        tree->data.acc = core::acc(tree->left) + core::val(tree) + core::acc(tree->right);
    }

    inline static void pushdown(const node_pointer tree) noexcept(NO_EXCEPT) {
        if(tree && tree->data.rev) {
            tree->data.rev = false;

            std::swap(tree->left, tree->right);

            if(tree->left) tree->left->data.rev ^= 1;
            if(tree->right) tree->right->data.rev ^= 1;
        }

        if(tree && tree->data.lazy != operation{}) {
            if(tree->left) {
                tree->left->data.lazy = tree->data.lazy + tree->left->data.lazy;
                tree->left->data.acc = action::map(tree->left->data.acc, action::fold(tree->data.lazy, interface::size(tree->left)));
            }

            if(tree->right) {
                tree->right->data.lazy = tree->data.lazy + tree->right->data.lazy;
                tree->right->data.acc = action::map(tree->right->data.acc, action::fold(tree->data.lazy, interface::size(tree->right)));
            }

            tree->data.val = action::map(tree->data.val, tree->data.lazy);
            tree->data.lazy = operation{};
        }
    }

  public:
    template<std::random_access_iterator I, std::sized_sentinel_for<I> S>
        requires std::convertible_to<std::iter_value_t<I>, operand>
    static node_pointer build(I first, S last) {
        std::vector<int>;
        if(first == last) return nullptr;

        const auto length = std::ranges::distance(first, last);
        const auto middle = std::next(first, length >> 1);

        node_pointer tree;

        core::merge(tree, core::build(first, middle), new node_type(operand{ *middle }, 1), core::build(std::next(middle), last));

        return tree;
    }

    template<std::random_access_iterator I, std::sized_sentinel_for<I> S>
        requires
            std::convertible_to<typename std::iter_value_t<I>::first_type, operand> &&
            std::integral<typename std::iter_value_t<I>::second_type>
    static node_pointer build(I first, S last) noexcept(NO_EXCEPT) {
        if(first == last) return nullptr;

        const auto length = std::ranges::distance(first, last);
        const auto middle = std::next(first, length >> 1);

        node_pointer tree;
        core::merge(tree, core::build(first, middle), new node_type(middle->first, middle->second), core::build(std::next(middle), last));

        return tree;
    }


    using interface::split;
    using interface::merge;

    inline static void split(const node_pointer tree, const size_type l, const size_type r, node_pointer& t0, node_pointer& t1, node_pointer& t2) noexcept(NO_EXCEPT) {
        core::split(tree, r, t1, t2);
        core::split(t1, l, t0, t1);
    }

    inline static void merge(node_pointer& tree, node_pointer t0, const node_pointer t1, const node_pointer t2) noexcept(NO_EXCEPT) {
        core::merge(t0, t0, t1);
        core::merge(tree, t0, t2);
    }


    static void insert(node_pointer& tree, const size_type pos, const operand& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        node_pointer t0, t1;

        core::split(tree, pos, t0, t1);
        core::merge(tree, t0, new node_type(val, count), t1);
    }

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    static void insert(node_pointer& tree, const size_type pos, I first, S last) noexcept(NO_EXCEPT) {
        node_pointer t0, t1;

        core::split(tree, pos, t0, t1);
        core::merge(tree, t0, core::build(first, last), t1);
    }


    static void erase(node_pointer& tree, const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        assert(l <= r);
        node_pointer t0, t1, t2;

        core::split(tree, l, r, t0, t1, t2);

        delete t1;

        core::merge(tree, t0, t2);
    }


    static void fill(node_pointer& tree, const size_type l, const size_type r, const operand& val) noexcept(NO_EXCEPT) {
        assert(l <= r);
        if(l == r) return;

        node_pointer t0, t1, t2;
        core::split(tree, l, r, t0, t1, t2);

        delete t1;
        t1 = new node_type(val, r - l);

        core::merge(tree, t0, t1, t2);
    }

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    static void fill(node_pointer& tree, const size_type pos, I first, S last) noexcept(NO_EXCEPT) {
        node_pointer t0, t1, t2;

        core::split(tree, pos, pos + std::ranges::distance(first, last), t0, t1, t2);

        delete t1;

        core::merge(tree, t0, core::build(first, last), t2);
    }


    static void apply(node_pointer& tree, const size_type l, const size_type r, const operation& val) noexcept(NO_EXCEPT) {
        assert(l <= r);
        if(l == r) return;

        node_pointer t0, t1, t2;

        core::split(tree, l, r, t0, t1, t2);

        if(!t1) t1 = new node_type({}, r - l);
        t1->data.lazy = val + t1->data.lazy;

        core::merge(tree, t0, t1, t2);
    }


    operand fold(node_pointer& tree, const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        assert(l <= r);
        if(l == r) return operand{};

        node_pointer t0, t1, t2;

        core::split(tree, l, r, t0, t1, t2);

        const operand res = t1 ? t1->data.acc : operand{};

        core::merge(tree, t0, t1, t2);

        return res;
    }


    static void reverse(node_pointer& tree, const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        assert(l <= r);
        if(l == r) return;

        node_pointer t0, t1, t2;

        core::split(tree, l, r, t0, t1, t2);

        if(t1) t1->data.rev ^= 1;

        core::merge(tree, t0, t1, t2);
    }


    static void shift_left(node_pointer& tree, const size_type l, const size_type r, const size_type count) noexcept(NO_EXCEPT) {
        assert(l <= r);

        if(count < 0) return core::shift_right(tree, l, r, -count);
        assert(l + count <= r);

        if(count == 0) return;
        if(count == r - l) return core::erase(tree, l, r);

        node_pointer t0, t1, t2, t3;

        core::split(tree, l + count, r, t1, t2, t3);
        core::split(t1, l, t0, t1);

        delete t1;

        core::merge(t2, t2, new node_type({}, r - l - count));
        core::merge(tree, t0, t2, t3);
    }

    static void shift_right(node_pointer& tree, const size_type l, const size_type r, const size_type count) noexcept(NO_EXCEPT) {
        assert(l <= r);

        if(count < 0) return core::shift_left(tree, l, r, -count);
        assert(l + count <= r);

        if(count == 0) return;
        if(count == r - l) return core::erase(tree, l, r);

        node_pointer t0, t1, t2, t3;

        core::split(tree, l, r - count, t1, t2, t3);
        core::split(t1, l, t0, t1);

        delete t2;

        core::merge(t1, new node_type({}, r - l - count), t1);
        core::merge(tree, t0, t1, t3);
    }

    static void rotate(node_pointer& tree, const size_type l, const size_type m, const size_type r) noexcept(NO_EXCEPT) {
        assert(l <= m && m < r);
        if(l == m) return;

        node_pointer t0, t1, t2, t3;

        core::split(tree, m, r, t1, t2, t3);
        core::split(t1, l, t0, t1);
        core::merge(t2, t2, t1);
        core::merge(tree, t0, t2, t3);
    }


    template<bool LEFT>
    size_type find(const node_pointer tree, operand& val, const size_type offset) noexcept(NO_EXCEPT) {
        if(tree->data.acc + val == val) {
            return -1;
        }

        if constexpr(LEFT) {
            if(tree->left and tree->left->data.acc + val != val) {
                return core::find<true>(tree->left, val, offset);
            }
            else {
                return tree->data.val + val != val ? offset + core::size(tree->left) : core::find<true>(tree->right, val, offset + core::size(tree->left) + 1);
            }
        }
        else {
            if(tree->right and tree->right->data.acc + val != val) {
                return core::find<false>(tree->right, val, offset + core::size(tree->left) + 1);
            }
            else {
                return tree->data.val + val != val ? offset + core::size(tree->left) : core::find<false>(tree->left, val, offset);
            }
        }
    }

    template<bool LEFT>
    inline size_type find(node_pointer& tree, const size_type l, const size_type r, const operand& val) noexcept(NO_EXCEPT) {
        if(l == r) return -1;

        node_pointer t0, t1, t2;

        core::split(tree, l, r, t0, t1, t2);

        const size_type res = core::find<LEFT>(t1, val, l);

        core::merge(tree, t0, t1, t2);

        return res;
    }


    static debugger::debug_t dump_rich(const node_pointer tree, const std::string prefix, const int dir, size_type& index) {
        if (!tree) return prefix + "\n";

        core::pushdown(tree);

        const auto left = core::dump_rich(tree->left, prefix + (dir == 1 ? "| " : "  "), -1, index);
        const auto here = prefix + "--+ " + debugger::dump(index) + " : " + debugger::dump(tree->data) + " [" + debugger::dump(tree->length) + "]\n";
        index += tree->length;

        const auto right = core::dump_rich(tree->right, prefix + (dir == -1 ? "| " : "  "), 1, index);

        return left + here + right;
    }

    inline static debugger::debug_t dump_rich(const node_pointer tree, const std::string prefix = "   ", const int dir = 0) {
        size_type index = 0;
        return core::dump_rich(tree, prefix, dir, index);
    }

    static debugger::debug_t _debug(const node_pointer tree) {
        if (!tree) return "";

        core::pushdown(tree);

        return
            "(" +
            core::_debug(tree->left) + " " +
            debugger::dump(tree->data) + " [" +
            debugger::dump(tree->length) + "] " +
            core::_debug(tree->right) +
            ")";
    }
};



} // namespace dynamic_sequence_impl

} // namespace internal


template<class Value, class = treap_context<>>
struct dynamic_sequence : dynamic_sequence<actions::make_full_t<Value>> {
    using dynamic_sequence<actions::make_full_t<Value>>::dynamic_sequence;
};


template<actions::internal::full_action Action, class Context>
    requires internal::available_with<internal::dynamic_sequence_impl::core, Action, Context>
struct dynamic_sequence<Action, Context> : private internal::dynamic_sequence_impl::core<Action, Context> {
  public:
    using action = Action;

    using operand = typename action::operand;
    using operation = typename action::operation;

    using value_type = operand;
    using action_type = typename operation::value_type;

    using core = internal::dynamic_sequence_impl::core<Action, Context>;

  private:
    using node_pointer = typename core::node_pointer;

  public:
    using size_type = typename core::size_type;

  private:
    node_pointer _root = nullptr;

    size_type _offset = 0;


    template<std::same_as<size_type>... SizeTypes>
    inline void _normalize_index(SizeTypes&... indices) noexcept(NO_EXCEPT) {
        const size_type min_index = std::min({ indices... });
        ((indices -= this->_offset), ...);
        if(min_index < this->_offset) this->_offset = min_index;
    }


  public:
    ~dynamic_sequence() { delete this->_root; }

    dynamic_sequence() noexcept = default;

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    dynamic_sequence(I first, S last) noexcept(NO_EXCEPT) { this->assign(first, last); }

    explicit dynamic_sequence(const size_type size, const value_type& val = value_type{}) noexcept(NO_EXCEPT) { this->assign(size, val); }

    template<std::ranges::input_range R>
    explicit dynamic_sequence(R&& range) noexcept(NO_EXCEPT) : dynamic_sequence(ALL(range)) {}

    template<std::convertible_to<value_type> T>
    dynamic_sequence(const std::initializer_list<T>& values) noexcept(NO_EXCEPT) : dynamic_sequence(values) {}


    size_type size() const noexcept(NO_EXCEPT) { return this->core::size(this->_root); }

    bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }


    inline void clear() noexcept(NO_EXCEPT) {
        delete this->_root;
        this->_root = nullptr;
        this->_offset = 0;
    }


    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    inline auto& assign(I first, S last) noexcept(NO_EXCEPT) {
        this->clear();
        this->_root = this->core::build(first, last);
        return *this;
    }

    inline auto& assign(const size_type size, const value_type& val = value_type{}) noexcept(NO_EXCEPT) {
        this->clear();
        this->core::insert(this->_root, 0, val, size);
        return *this;
    }

    template<std::ranges::input_range R>
    inline auto& assign(R&& range) noexcept(NO_EXCEPT) { return this->assign(ALL(range)); }

    template<std::convertible_to<value_type> T>
    inline auto& assign(const std::initializer_list<T>& values) noexcept(NO_EXCEPT) { return this->assign(values); }


    inline auto& resize(const size_type size, const value_type& val = value_type{}) noexcept(NO_EXCEPT) {
        if(this->size() > size) this->core::erase(this->_root, size, this->size());
        if(this->size() < size) this->core::push_back(val, size - this->size());
        return *this;
    }


    inline auto& fill(const value_type& val) noexcept(NO_EXCEPT) {
        this->core::fill(this->_root, 0, this->size());
        return *this;
    }

    inline value_type fold() noexcept(NO_EXCEPT) {
        return this->core::fold(this->_root, 0, this->size());
    }

    inline auto& apply(const action_type& val) noexcept(NO_EXCEPT) {
        this->core::apply(this->_root, 0, this->size(), val);
        return *this;
    }

    inline value_type front() noexcept(NO_EXCEPT) {
        return this->core::fold(this->_root, 0, 1);
    }

    inline value_type back() noexcept(NO_EXCEPT) {
        return this->core::fold(this->_root, this->size() - 1, this->size());
    }


    inline auto& push_front(const value_type& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->core::insert(this->_root, 0, val, count);
        return *this;
    }

    inline auto& push_back(const value_type& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->core::insert(this->_root, this->size(), val, count);
        return *this;
    }

    inline auto& reverse() noexcept(NO_EXCEPT) {
        this->core::reverse(this->_root, 0, this->size());
        return *this;
    }

    inline auto& shift_left(const size_type count = 1) noexcept(NO_EXCEPT) {
        this->core::shift_left(this->_root, 0, this->size(), count);
        return *this;
    }

    inline auto& shift_right(const size_type count = 1) noexcept(NO_EXCEPT) {
        this->core::shift_right(this->_root, 0, this->size(), count);
        return *this;
    }

    // Same usage as: std::rotate(:m:)
    inline auto& rotate(size_type m) noexcept(NO_EXCEPT) {
        this->_normalize_index(m);
        this->core::rotate(this->_root, 0, m, this->size());
        return *this;
    }

    // Same usage as: std::rotate(:m:)
    inline auto& rotate_left(const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(!this->empty());
        this->core::rotate(this->_root, 0, lib::mod(count, this->size()), this->size());
        return *this;
    }

    // Same usage as: std::rotate(:m:)
    inline auto& rotate_right(const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(!this->empty());
        this->core::rotate(this->_root, 0, this->size() - lib::mod(count, this->size()), this->size());
        return *this;
    }


    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    inline auto& insert(size_type pos, I first, S last) noexcept(NO_EXCEPT) {
        this->_normalize_index(pos);
        this->core::insert(this->_root, pos, first, last);
        return *this;
    }

    inline auto& insert(size_type pos, const operand& val, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_normalize_index(pos);
        this->core::insert(this->_root, pos, val, count);
        return *this;
    }

    inline void erase(size_type l, size_type r) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->core::erase(this->_root, l, r);
    }

    inline auto& fill(size_type l, size_type r, const value_type& val) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->core::fill(this->_root, l, r, val);
        return *this;
    }

    inline operand fold(size_type l, size_type r) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        return this->core::fold(this->_root, l, r);
    }

    inline auto& apply(size_type l, size_type r, const operation& val) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->core::apply(this->_root, l, r, val);
        return *this;
    }

    inline auto& reverse(size_type l, size_type r) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->core::reverse(this->_root, l, r);
        return *this;
    }

    inline auto& rotate(size_type l, size_type m, size_type r) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, m, r);
        this->core::rotate(this->_root, l, m, r);
        return *this;
    }

    inline auto& shift_left(size_type l, size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->core::shift_left(this->_root, l, r, count);
        return *this;
    }

    inline auto& shift_right(size_type l, size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        this->_normalize_index(l, r);
        this->core::shift_right(this->_root, l, r, count);
        return *this;
    }


    template<std::ranges::input_range R>
    inline auto& insert(const size_type pos, R&& range) noexcept(NO_EXCEPT) {
        this->insert(pos, std::ranges::begin(range), std::ranges::end(range));
        return *this;
    }


    inline auto& set(const size_type pos, const value_type& val) noexcept(NO_EXCEPT) {
        return this->fill(pos, pos + 1, val);
    }

    inline value_type get(const size_type pos) noexcept(NO_EXCEPT) {
        return this->fold(pos, pos + 1).val();
    }


    inline auto& erase(const size_type pos) noexcept(NO_EXCEPT) {
        this->erase(pos, pos + 1);
        return *this;
    }


    inline auto& pop_front(const size_type count = 1) noexcept(NO_EXCEPT) {
        this->erase(0, count);
        return *this;
    }

    inline auto& pop_back(const size_type count = 1) noexcept(NO_EXCEPT) {
        this->erase(this->size() - count, count);
        return *this;
    }


    inline auto& apply(const size_type pos, const action_type& val) noexcept(NO_EXCEPT) {
        this->apply(pos, pos + 1, val);
        return *this;
    }


    inline auto& rotate_left(const size_type l, const size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(l < r);
        return this->rotate(l, l + lib::mod(count, r - l), r);
    }

    inline auto& rotate_right(const size_type l, const size_type r, const size_type count = 1) noexcept(NO_EXCEPT) {
        assert(l < r);
        return this->rotate(l, r - lib::mod(count, r - l), r);
    }


    // Find the min / max k in [l, r) that satisfies (this[k] + x) != x.
    // If no such k is found, return -1.
    template<bool LEFT = true>
    inline size_type find(const size_type l, const size_type r, const value_type& val) noexcept(NO_EXCEPT) {
        return this->template core::find<LEFT>(l, r - 1, r);
    }

    // Find the min / max k in whole that satisfies (this[k] + x) != x.
    // If no such k is found, return -1.
    template<bool LEFT = true>
    inline size_type find(const value_type& val) noexcept(NO_EXCEPT) {
        return this->find<LEFT>(0, this->size(), val);
    }


    struct point_reference : internal::point_reference<dynamic_sequence, size_type> {
        point_reference(dynamic_sequence *const super, const size_type pos) noexcept(NO_EXCEPT)
          : internal::point_reference<dynamic_sequence, size_type>(super, pos)
        {}

        operator value_type() noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }
        value_type val() noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }


        inline point_reference& apply(const action_type& val) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, val);
            return *this;
        }
        inline point_reference& operator+=(const action_type& val) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, val);
            return *this;
        }


        inline point_reference& set(const action_type& val) noexcept(NO_EXCEPT) {
            this->_super->set(this->_pos, val);
            return *this;
        }

        inline point_reference& operator=(const action_type& val) noexcept(NO_EXCEPT) {
            this->_super->set(this->_pos, val);
            return *this;
        }
    };


    struct range_reference : internal::range_reference<dynamic_sequence, size_type> {
        range_reference(dynamic_sequence *const super, const size_type l, const size_type r) noexcept(NO_EXCEPT)
          : internal::range_reference<dynamic_sequence, size_type>(super, l, r)
        {}


        inline value_type fold() noexcept(NO_EXCEPT) {
            return this->_super->fold(this->_begin, this->_end);
        }

        inline range_reference& fill(const value_type& val) noexcept(NO_EXCEPT) {
            this->_super->fill(this->_begin, this->_end, val);
            return *this;
        }
        inline range_reference& operator=(const value_type& val) noexcept(NO_EXCEPT) {
            this->_super->fill(this->_begin, this->_end, val);
            return *this;
        }

        inline range_reference& apply(const action_type& val) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, val);
            return *this;
        }
        inline range_reference& operator+=(const action_type& val) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, val);
            return *this;
        }

        // Find the min / max k in [l, r) that satisfies (this[k] + x) != x.
        // If no such k is found, return -1.
        template<bool LEFT = true>
        inline size_type find(const value_type& val) noexcept(NO_EXCEPT) {
            return this->_super->template core::find<LEFT>(this->_begin, this->_end, val);
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
    using iterator_interface = internal::container_iterator_interface<value_type,dynamic_sequence,iterator>;

  public:
    struct iterator : iterator_interface {
        iterator() noexcept = default;
        iterator(dynamic_sequence *const ref, const size_type pos) noexcept(NO_EXCEPT) : iterator_interface(ref, pos) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline iterator end() noexcept(NO_EXCEPT) { return iterator(this, this->size()); }


    using core::dump_rich;
    using core::_debug;


    debugger::debug_t dump_rich(const typename core::node_pointer tree, const std::string prefix = "   ", const int dir = 0) const {
        size_type index = this->_offset;
        return this->dump_rich(tree, prefix, dir, index);
    }

    debugger::debug_t dump_rich(const std::string prefix = "   ") const {
        return "\n" + this->dump_rich(this->_root, prefix);
    }


    debugger::debug_t _debug() const {
        return "[ " + this->_debug(this->_root) + " ]";
    }

};


} // namespace lib
