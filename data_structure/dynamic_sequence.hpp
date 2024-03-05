#pragma once


#include <cassert>
#include <utility>
#include <type_traits>
#include <initializer_list>
#include <random>
#include <concepts>
#include <ranges>

#include "snippet/internal/types.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/uncopyable.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/concepts.hpp"

#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "data_structure/internal/tree_interface.hpp"
#include "data_structure/treap.hpp"

#include "algebraic/internal/concepts.hpp"

#include "action/base.hpp"


namespace lib {

namespace internal {

namespace dynamic_sequence_impl {


template<
    algebraic::internal::monoid OperandMonoid, algebraic::internal::monoid OperatorMonoid,
    OperandMonoid (*_map)(const OperandMonoid&, const OperatorMonoid&),
    OperatorMonoid (*_fold)(const OperatorMonoid&, internal::size_t),
    class Context
>
struct base : protected Context::interface<base<OperandMonoid, OperatorMonoid, _map, _fold, Context>, internal::size_t> {
    using operand = OperandMonoid;
    using operation = OperatorMonoid;

    using size_type = internal::size_t;

  private:
    using interface = typename Context::interface<base, size_type>;
    static_assert(tree_interface<interface>);

    friend interface;

  public:
    using key_type = typename interface::key_type;

  private:
    struct data_type {
        operand val, acc;
        operation lazy;
        bool rev = false;

        data_type() noexcept = default;
        data_type(const operand& _val) noexcept(NO_EXCEPT) : val(_val) {}
    };


    using node_type = typename interface::node_type<data_type>;
    using node_pointer = std::add_pointer_t<node_type>;

    node_pointer root = nullptr;

  public:
    ~base() { delete this->root; }

  private:
    inline operand acc(const node_pointer tree) const noexcept(NO_EXCEPT) { return tree ? tree->acc : operand{}; }

    inline void update_acc(const node_pointer tree) const noexcept(NO_EXCEPT) {
        if(tree) tree->acc = this->acc(tree->left) + tree->val + this->acc(tree->right);
    }

    inline void pushup(const node_pointer tree) const noexcept(NO_EXCEPT) { this->update_acc(tree); }

    inline void pushdown(const node_pointer tree) const noexcept(NO_EXCEPT) {
        if(tree && tree->rev) {
            tree->rev = false;

            std::swap(tree->left, tree->right);

            if(tree->left) tree->left->rev ^= 1;
            if(tree->right) tree->right->rev ^= 1;
        }

        if(tree && tree->lazy != operation{}) {
            if(tree->left) {
                tree->left->lazy = tree->lazy + tree->left->lazy;
                tree->left->acc = _map(tree->left->acc, _fold(tree->lazy, this->interface::size(tree->left)));
            }

            if(tree->right) {
                tree->right->lazy = tree->lazy + tree->right->lazy;
                tree->right->acc = _map(tree->right->acc, _fold(tree->lazy, this->interface::size(tree->right)));
            }

            tree->val = _map(tree->val, _fold(tree->lazy, 1));
            tree->lazy = operation{};
        }
    }


  private:
    inline void insert(node_pointer& tree, const key_type pos, const operand& val) const noexcept(NO_EXCEPT) {
        node_pointer t0, t1;

        this->split(tree, pos, t0, t1);
        this->merge(t0, t0, new node_type(val, pos, this));
        this->merge(tree, t0, t1);
    }

    inline void erase(node_pointer& tree, const key_type l, const key_type r) const noexcept(NO_EXCEPT) {
        node_pointer t0, t1, t2;

        this->split(tree, r, t1, t2);
        this->split(t1, l, t0, t1);
        this->merge(tree, t0, t2);

        if(t1) delete t1;
    }


    inline void apply(node_pointer& tree, const key_type pos, const operation& val) const noexcept(NO_EXCEPT) {
        node_pointer t0, t1, t2;

        this->split(tree, pos + 1, t1, t2);
        this->split(t1, pos, t0, t1);

        if(!t1) t1 = new node_type({}, pos, this);
        t1->lazy = val + t1->lazy;

        this->merge(t1, t0, t1);
        this->merge(tree, t1, t2);
    }

    inline void apply(node_pointer tree, const key_type l, const key_type r, const operation& val) const noexcept(NO_EXCEPT) {
        if(l == r) return;

        node_pointer t0, t1, t2;

        this->split(tree, r, t1, t2);
        this->split(t1, l, t0, t1);

        if(t1) t1->lazy = val + t1->lazy;

        this->merge(t1, t0, t1);
        this->merge(tree, t1, t2);
    }


    inline operand fold(node_pointer tree, const key_type l, const key_type r) const noexcept(NO_EXCEPT) {
        if(l == r) return operand{};

        node_pointer t0, t1, t2;

        this->split(tree, r, t1, t2);
        this->split(t1, l, t0, t1);

        const operand res = t1 ? t1->acc : operand{};

        this->merge(t1, t0, t1);
        this->merge(tree, t1, t2);

        return res;
    }


    template<bool LEFT>
    inline key_type find(const node_pointer tree, operand& val, const key_type offset) const noexcept(NO_EXCEPT) {
        if(tree->acc + val == val) {
            return -1;
        }

        if constexpr(LEFT) {
            if(tree->left and tree->left->acc + val != val) {
                return this->find<true>(tree->left, val, offset);
            }
            else {
                return tree->val + val != val ? offset + this->size(tree->left) : this->find<true>(tree->right, val, offset + this->size(tree->left) + 1);
            }
        }
        else {
            if(tree->right and tree->right->acc + val != val) {
                return this->find<false>(tree->right, val, offset + this->size(tree->left) + 1);
            }
            else {
                return tree->val + val != val ? offset + this->size(tree->left) : this->find<false>(tree->left, val, offset);
            }
        }
    }

    inline void reverse(node_pointer tree, const key_type l, const key_type r) const noexcept(NO_EXCEPT) {
        if(l == r) return;

        node_pointer t0, t1, t2;

        this->split(tree, r, t1, t2);
        this->split(t1, l, t0, t1);

        if(t1) t1->rev ^= 1;

        this->merge(t1, t0, t1);
        this->merge(tree, t1, t2);
    }

    inline void rotate(node_pointer tree, const key_type l, const key_type m, const key_type r) const noexcept(NO_EXCEPT) {
        if(l == m) return;
        if(m == r) this->reverse(tree, l, r);

        node_pointer t0, t1, t2, t3;

        this->split(tree, r, t2, t3);
        this->split(t2, m, t1, t2);
        this->split(t1, l, t0, t1);

        if(t1) t1->rev ^= 1;
        if(t2) t2->rev ^= 1;

        this->merge(t2, t1, t2);

        if(t2) t2->rev ^= 1;

        this->merge(t2, t0, t2);
        this->merge(tree, t2, t3);
    }

  public:
    inline size_type size() const noexcept(NO_EXCEPT) { return this->interface::size(this->root); }

    void insert(const key_type pos, const operand& val)  noexcept(NO_EXCEPT) { this->insert(this->root, pos, val); }

    inline void apply(const key_type l, const key_type r, const operation& val) const noexcept(NO_EXCEPT) { this->apply(this->root, l, r, val); }
    inline void apply(const key_type pos, const operation& val) noexcept(NO_EXCEPT) { this->apply(this->root, pos, val); }

    inline operand fold(const key_type l, const key_type r) const noexcept(NO_EXCEPT) { return this->fold(this->root, l, r); }

    inline void erase(const key_type l, const key_type r) noexcept(NO_EXCEPT) { this->erase(this->root, l, r); }

    inline void clear() const noexcept(NO_EXCEPT) { if(this->root) delete this->root; this->root = nullptr; }

    inline void reverse(const key_type l, const key_type r) const noexcept(NO_EXCEPT) { this->reverse(this->root, l, r); }

    inline void rotate(const key_type l, const key_type m, const key_type r) const noexcept(NO_EXCEPT) { this->rotate(this->root, l, m, r); }

    template<bool LEFT>
    inline key_type find(const key_type l, const key_type r, const operand& val) const noexcept(NO_EXCEPT) {
        if(l == r) return -1;

        node_pointer t0, t1, t2;

        this->split(this->root, l, t0, t1);
        this->split(t1, r - l, t1, t2);

        const key_type res = this->find<LEFT>(t1, val, l);

        this->merge(t1, t1, t2);
        this->merge(this->root, t0, t1);

        return res;
    }
};


template<actions::internal::full_action Action, class Context>
    requires avilable<base<typename Action::operand, typename Action::operation, Action::map, Action::fold, Context>>
struct core : base<typename Action::operand, typename Action::operation, Action::map, Action::fold, Context> {
  public:
    using action = Action;

    using operand = typename action::operand;
    using operation = typename action::operation;

    using value_type = operand;
    using action_type = typename operation::value_type;

  private:
    using base = dynamic_sequence_impl::base<typename Action::operand, typename Action::operation, Action::map, Action::fold, Context>;

  public:
    using key_type = typename base::key_type;
    using size_type = typename base::size_type;

    static_assert(
        (
            Context::IMPLICIT_KEY && std::same_as<key_type, size_type>
        ) ||
        (
            !Context::IMPLICIT_KEY && internal::has_or_more_digits_than<key_type, size_type>
        )
    );

  private:
    inline key_type _positivize_index(const key_type pos) const noexcept(NO_EXCEPT) {
        static_assert(Context::IMPLICIT_KEY);
        return pos < 0 ? this->size() + pos : pos;
    }


    inline consteval key_type _left_bound() const noexcept(NO_EXCEPT) {
        if constexpr(Context::IMPLICIT_KEY) return 0;
        else return std::numeric_limits<key_type>::lowest();
    }

    inline consteval key_type _right_bound() const noexcept(NO_EXCEPT) {
        if constexpr(Context::IMPLICIT_KEY) return this->size();
        else return std::numeric_limits<key_type>::max();
    }

  public:
    template<class... Args>
    explicit core(Args&&... args) noexcept(NO_EXCEPT) : core() { this->assign(std::forward<Args>(args)...); }

    template<std::convertible_to<value_type> T>
    core(const std::initializer_list<T>& values) noexcept(NO_EXCEPT)
      : core(std::ranges::begin(values), std::ranges::end(values))
    {}

    core() noexcept(NO_EXCEPT) {}


    template<std::forward_iterator I, std::sentinel_for<I> S>
    inline void insert(key_type pos, I first, S last) noexcept(NO_EXCEPT) {
        for(auto itr=first; itr != last; ++itr, ++pos) {
            this->insert(pos, *itr);
        }
    }

    template<std::ranges::input_range R>
    inline void insert(const key_type pos, R&& range) noexcept(NO_EXCEPT) {
        return this->insert(pos, std::ranges::begin(range), std::ranges::end(range));
    }

    inline auto& insert(key_type pos, const value_type& val) noexcept(NO_EXCEPT) {
        if constexpr(Context::IMPLICIT_KEY) {
            pos = this->_positivize_index(pos);
            assert(0 <= pos && pos <= this->size());
        }

        this->base::insert(pos, val);

        return *this;
    }

    inline auto& insert(const key_type pos, const value_type& val, const size_type count) noexcept(NO_EXCEPT) {
        REP(k, count) this->insert(pos + k, val);
        return *this;
    }


    bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }


    struct point_reference : internal::point_reference<core, key_type> {
        point_reference(core *const super, const key_type pos) noexcept(NO_EXCEPT)
            requires (Context::IMPLICIT_KEY)
          : internal::point_reference<core, key_type>(super, super->_positivize_index(pos))
        {
        }

        point_reference(core *const super, const key_type pos) noexcept(NO_EXCEPT)
            requires (!Context::IMPLICIT_KEY)
          : internal::point_reference<core, key_type>(super, pos)
        {}


        operator value_type() const noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }
        value_type val() const noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }


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


    struct range_reference : internal::range_reference<core, key_type> {
        range_reference(core *const super, const key_type l, const key_type r) noexcept(NO_EXCEPT)
            requires (Context::IMPLICIT_KEY)
          : internal::range_reference<core, key_type>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            if constexpr(Context::IMPLICIT_KEY) {
                assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
            }
        }

        range_reference(core *const super, const key_type l, const key_type r) noexcept(NO_EXCEPT)
            requires (!Context::IMPLICIT_KEY)
          : internal::range_reference<core, key_type>(super, l, r)
        {}


        inline value_type fold() const noexcept(NO_EXCEPT) {
            if(this->_begin == 0 and this->_end == this->_super->size()) return this->_super->fold();
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
        inline key_type find(const value_type& val) const noexcept(NO_EXCEPT) {
            return this->_super->template base::find<LEFT>(this->_begin, this->_end, val);
        }
    };


    inline auto& push_front(const value_type& val, const size_type count = 1) noexcept(NO_EXCEPT)
    {
        this->insert(0, val, count);
        return *this;
    }

    inline auto& push_back(const value_type& val, const size_type count = 1) noexcept(NO_EXCEPT)
    {
        this->insert(this->size(), val, count);
        return *this;
    }


    inline auto& resize(const size_type size, const value_type& val = value_type{}) noexcept(NO_EXCEPT) {
        REP(this->size() - size) this->erase(-1);
        REP(i, this->size(), size) this->push_back(val);
        return *this;
    }


    inline auto& assign(const size_type size, const value_type& val = value_type{}) noexcept(NO_EXCEPT) {
        this->clear();
        this->insert(0, val, size);
        return *this;
    }

    template<std::convertible_to<value_type> T>
    inline auto& assign(const std::initializer_list<T>& values) noexcept(NO_EXCEPT)
    {
        this->assign(std::ranges::begin(values), std::ranges::end(values));
        return *this;
    }

    template<std::input_iterator I, std::sentinel_for<I> S>
    inline auto& assign(I first, S last) noexcept(NO_EXCEPT) {
        this->clear();
        this->insert(0, first, last);
        return *this;
    }

    template<std::ranges::input_range R>
    inline auto& assign(R&& range) noexcept(NO_EXCEPT) {
        return this->assign(ALL(range));
    }


    inline auto& fill(const value_type& val) noexcept(NO_EXCEPT) {
        const size_type count = this->size();
        this->clear(), this->insert(0, val, count);
        return *this;
    }

    inline auto& fill(const key_type l, const key_type r, const value_type& val) noexcept(NO_EXCEPT) {
        this->erase(l, r);
        REP(pos, l, r) this->insert(pos, val);
        return *this;
    }


    inline auto& set(const key_type pos, const value_type& val) noexcept(NO_EXCEPT) {
        this->fill(pos, pos+1, val);
        return *this;
    }


    inline auto& erase(key_type pos) noexcept(NO_EXCEPT) {
        if constexpr(Context::IMPLICIT_KEY) {
            pos = this->_positivize_index(pos);
            assert(0 <= pos && pos < this->size());
        }

        this->base::erase(pos, pos + 1);

        return *this;
    }

    inline auto& erase(key_type l, key_type r) noexcept(NO_EXCEPT) {
        if constexpr(Context::IMPLICIT_KEY) {
            l = this->_positivize_index(l), r = this->_positivize_index(r);
            assert(0 <= l && l <= r && r <= this->size());
        }

        this->base::erase(l, r);

        return *this;
    }


    inline auto& clear() noexcept(NO_EXCEPT) {
        this->base::erase(0, this->size());
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


    inline auto& apply(key_type l, key_type r, const action_type& val) noexcept(NO_EXCEPT) {
        if constexpr(Context::IMPLICIT_KEY) {
            l = this->_positivize_index(l), r = this->_positivize_index(r);
            assert(0 <= l && l <= r && r <= this->size());
        }

        this->base::apply(l, r, val);

        return *this;
    }

    inline auto& apply(const key_type pos, const action_type& val) noexcept(NO_EXCEPT) {
        this->base::apply(pos, val);
        return *this;
    }

    inline auto& apply(const action_type& val) noexcept(NO_EXCEPT) {
        this->apply(this->_left_bound(), this->_right_bound(), val);
        return *this;
    }


    inline value_type get(key_type pos) const noexcept(NO_EXCEPT) {
        if constexpr(Context::IMPLICIT_KEY) {
            pos = this->_positivize_index(pos);
            assert(0 <= pos && pos < this->size());
        }

        return this->base::fold(pos, pos+1).val();
    }


    inline point_reference operator[](const key_type pos) noexcept(NO_EXCEPT) { return point_reference(this, pos); }
    inline range_reference operator()(const key_type l, const key_type r) noexcept(NO_EXCEPT) { return range_reference(this, l, r); }


    inline value_type front() const noexcept(NO_EXCEPT) { return this->get(0); }

    inline value_type back() const noexcept(NO_EXCEPT) { return this->get(this->size()-1); }


    inline value_type fold(key_type l, key_type r) const noexcept(NO_EXCEPT) {
        if constexpr(Context::IMPLICIT_KEY) {
            l = this->_positivize_index(l), r = this->_positivize_index(r);
            assert(0 <= l && l <= r && r <= this->size());
        }

        return this->base::fold(l, r).val();
    }

    inline value_type fold() const noexcept(NO_EXCEPT) { return this->fold(0, this->size()); }


    inline auto& reverse(key_type l, key_type r) noexcept(NO_EXCEPT) {
        if constexpr(Context::IMPLICIT_KEY) {
            l = this->_positivize_index(l), r = this->_positivize_index(r);
            assert(0 <= l && l <= r && r <= this->size());
        }

        this->base::reverse(l, r);

        return *this;
    }

    inline auto& reverse() noexcept(NO_EXCEPT) {
        this->reverse(0, this->size());
        return *this;
    }


    // Same usage as: std::rotate(l, m, r)
    inline auto& rotate(key_type l, key_type m, key_type r) noexcept(NO_EXCEPT) {
        if constexpr(Context::IMPLICIT_KEY) {
            l = this->_positivize_index(l), m = this->_positivize_index(m), r = this->_positivize_index(r);
            assert(0 <= l && l <= m && m < r && r <= this->size());
        }

        this->base::rotate(l, m, r);

        return *this;
    }

    // Same usage as: std::rotate(0, m, size)
    inline auto& rotate(const key_type m) noexcept(NO_EXCEPT) {
        this->rotate(0, m, this->size());
        return *this;
    }


    // Find the min / max k in [l, r) that satisfies (this[k] + x) != x.
    // If no such k is found, return -1.
    template<bool LEFT = true>
    inline key_type find(key_type l, key_type r, const value_type& val) const noexcept(NO_EXCEPT) {
        if constexpr(Context::IMPLICIT_KEY) {
            l = this->_positivize_index(l), r = this->_positivize_index(r);
            assert(0 <= l && l <= r && r <= this->size());
        }

        return this->template base::find<LEFT>(l, r, val);
    }

    // Find the min / max k in whole that satisfies (this[k] + x) != x.
    // If no such k is found, return -1.
    template<bool LEFT = true>
    inline key_type find(const value_type& val) const noexcept(NO_EXCEPT) {
        return this->find<LEFT>(0, this->size(), val);
    }

    struct iterator;

  protected:
    using iterator_interface = internal::container_iterator_interface<value_type,core,iterator>;

  public:
    struct iterator : iterator_interface {
        iterator() noexcept = default;
        iterator(const core *const ref, const key_type pos) noexcept(NO_EXCEPT) : iterator_interface(ref, pos) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline iterator end() const noexcept(NO_EXCEPT) { return iterator(this, this->size()); }
};


} // namespace dynamic_sequence_impl

} // namespace internal


template<actions::internal::full_action Action, class Context = treap_context<>>
    requires internal::avilable<internal::dynamic_sequence_impl::core<Action, Context>>
struct dynamic_sequence : internal::dynamic_sequence_impl::core<Action, Context> {
    using internal::dynamic_sequence_impl::core<Action, Context>::core;
};


} // namespace lib
