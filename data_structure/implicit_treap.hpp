#pragma once


#include <cassert>
#include <utility>
#include <type_traits>
#include <initializer_list>
#include <random>

#include "snippet/internal/types.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/uncopyable.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "random/xorshift.hpp"

#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace internal {

namespace implicit_treap_impl {


// Thanks to: https://github.com/xuzijian629/library2/blob/master/treap/implicit_treap.cpp
template<class OperandMonoid, class OperatorMonoid, OperandMonoid (*_map)(const OperandMonoid&, const OperatorMonoid&), OperatorMonoid (*_fold)(const OperatorMonoid&, internal::size_t)>
struct base : private uncopyable {
    using operand = OperandMonoid;
    using operation = OperatorMonoid;

    using size_type = internal::size_t;

  private:
    struct node;
    using Tree = node*;

    static xorshift rand;

    struct node {
        operand v, acc;
        operation lazy;

        xorshift::result_type priority;

        size_type cnt = 1;

        bool rev = false;
        Tree left = nullptr, right = nullptr;

        node(const operand& v, const base* super) noexcept(NO_EXCEPT)
        : v(v), acc(operand{}), lazy(operation{}), priority(super->rand()) {}
        ~node() {
            if(this->left) delete this->left;
            if(this->right) delete this->right;
        }
    };

    Tree root = nullptr;

    inline size_type cnt(const Tree tree) const noexcept(NO_EXCEPT) { return tree ? tree->cnt : 0; }

    inline operand acc(const Tree tree) const noexcept(NO_EXCEPT) { return tree ? tree->acc : operand{}; }

    inline void update_cnt(const Tree tree) const noexcept(NO_EXCEPT) { if(tree) tree->cnt = 1 + this->cnt(tree->left) + this->cnt(tree->right); }

    inline void update_acc(const Tree tree) const noexcept(NO_EXCEPT) {
        if(tree) tree->acc = this->acc(tree->left) + tree->v + this->acc(tree->right);
    }

    inline void pushup(const Tree tree) const noexcept(NO_EXCEPT) { this->update_cnt(tree), update_acc(tree); }

    inline void pushdown(const Tree tree) const noexcept(NO_EXCEPT) {
        if(tree and tree->rev) {
            tree->rev = false;
            std::swap(tree->left, tree->right);
            if(tree->left) tree->left->rev ^= 1;
            if(tree->right) tree->right->rev ^= 1;
        }
        if(tree and tree->lazy != operation{}) {
            if(tree->left) {
                tree->left->lazy = tree->lazy + tree->left->lazy;
                tree->left->acc = _map(tree->left->acc, _fold(tree->lazy, this->cnt(tree->left)));
            }
            if(tree->right) {
                tree->right->lazy = tree->lazy + tree->right->lazy;
                tree->right->acc = _map(tree->right->acc, _fold(tree->lazy, this->cnt(tree->right)));
            }
            tree->v = _map(tree->v, _fold(tree->lazy, 1));
            tree->lazy = operation{};
        }
        // this->pushup(tree);
    }

    inline void split(const Tree tree, const size_type pos, Tree& left, Tree& right) const noexcept(NO_EXCEPT) {
        if(!tree) {
            left = right = nullptr;
            return;
        }
        this->pushdown(tree);
        const size_type implicit_key = this->cnt(tree->left) + 1;
        if(pos < implicit_key) {
            this->split(tree->left, pos, left, tree->left), right = std::move(tree);
            this->pushup(right);
        }
        else {
            this->split(tree->right, pos - implicit_key, tree->right, right), left = std::move(tree);
            this->pushup(left);
        }
    }

    inline void insert(Tree& tree, const size_type pos, const Tree item) const noexcept(NO_EXCEPT) {
        Tree t1, t2;
        this->split(tree, pos, t1, t2);
        this->merge(t1, t1, item);
        this->merge(tree, t1, t2);
    }

    inline void merge(Tree& tree, const Tree left, const Tree right) const noexcept(NO_EXCEPT) {
        this->pushdown(left);
        this->pushdown(right);
        if(!left || !right) {
            tree = left ? left : right;
        }
        else if(left->priority > right->priority) {
            this->merge(left->right, left->right, right), tree = std::move(left);
        }
        else {
            this->merge(right->left, left, right->left), tree = std::move(right);
        }
        this->pushup(tree);
    }

    inline void erase(Tree& tree, const size_type pos) const noexcept(NO_EXCEPT) {
        Tree t1, t2, t3;
        this->split(tree, pos + 1, t1, t2);
        this->split(t1, pos, t1, t3);
        this->merge(tree, t1, t2);
        if(t3) delete t3;
    }

    inline void apply(Tree tree, const size_type l, const size_type r, const operation& v) const noexcept(NO_EXCEPT) {
        if(l >= r) return;
        Tree t1, t2, t3;
        this->split(tree, l, t1, t2);
        this->split(t2, r - l, t2, t3);
        t2->lazy = v + t2->lazy;
        // t2->acc = _map(t2->acc, _fold(v, cnt(t2)));
        this->merge(t2, t2, t3);
        this->merge(tree, t1, t2);
    }

    inline operand fold(Tree tree, const size_type l, const size_type r) const noexcept(NO_EXCEPT) {
        if(l == r) return operand{};
        Tree t1, t2, t3;
        this->split(tree, l, t1, t2);
        this->split(t2, r - l, t2, t3);
        operand ret = t2->acc;
        this->merge(t2, t2, t3);
        this->merge(tree, t1, t2);
        return ret;
    }

    // rank of value v in [l, r)
    inline size_type find(const Tree tree, operand& v, const size_type offset, const bool dir_left = true) const noexcept(NO_EXCEPT) {
        if(tree->acc + v == v) {
            return -1;
        } else {
            if(dir_left) {
                if(tree->left and tree->left->acc + v != v) {
                    return this->find(tree->left, v, offset, dir_left);
                } else {
                    return tree->v + v != v ? offset + this->cnt(tree->left) : this->find(tree->right, v, offset + this->cnt(tree->left) + 1, dir_left);
                }
            } else {
                if(tree->right and tree->right->acc + v != v) {
                    return this->find(tree->right, v, offset + this->cnt(tree->left) + 1, dir_left);
                } else {
                    return tree->v + v != v ? offset + this->cnt(tree->left) : this->find(tree->left, v, offset, dir_left);
                }
            }
        }
    }

    inline void reverse(Tree tree, const size_type l, const size_type r) const noexcept(NO_EXCEPT) {
        if(l >= r) return;
        Tree t1, t2, t3;
        this->split(tree, l, t1, t2);
        this->split(t2, r - l, t2, t3);
        t2->rev ^= 1;
        this->merge(t2, t2, t3);
        this->merge(tree, t1, t2);
    }

    // Same usage as: std::rotate(l, m, r)
    inline void rotate(Tree tree, const size_type l, const size_type m, const size_type r) const noexcept(NO_EXCEPT) {
        this->reverse(tree, l, r);
        this->reverse(tree, l, l + r - m);
        this->reverse(tree, l + r - m, r);
    }

  public:
    inline size_type size() const noexcept(NO_EXCEPT) { return this->cnt(this->root); }

  protected:
    virtual ~base() { delete this->root; }

  public:
    // void insert(const size_type p, const operand& v) { this->insert(this->root, p, std::make_shared<node>(v, this)); }
    void insert(const size_type p, const operand& v) noexcept(NO_EXCEPT) { this->insert(this->root, p, new node(v, this)); }

    inline void apply(const size_type l, const size_type r, const operation& v) noexcept(NO_EXCEPT) { this->apply(this->root, l, r, v); }

    inline operand fold(const size_type l, const size_type r) const noexcept(NO_EXCEPT) { return this->fold(this->root, l, r); }

    inline void erase(const size_type p) noexcept(NO_EXCEPT) { this->erase(this->root, p); }

    inline void reverse(const size_type l, const size_type r) const noexcept(NO_EXCEPT) { this->reverse(this->root, l, r); }

    inline void rotate(const size_type l, const size_type m, const size_type r) const noexcept(NO_EXCEPT) { this->rotate(this->root, l, m, r); }

    // 二分探索。[l, r)内のkでf0(tr[k], v) != xとなる最左/最右のもの。存在しない場合は-1
    // たとえばMinMonoidの場合、x未満の最左/最右の要素の位置を返す
    inline size_type find(const size_type l, const size_type r, const operand& v, const bool dir_left) const noexcept(NO_EXCEPT) {
        if(l >= r) return -1;
        Tree t1, t2, t3;
        this->split(this->root, l, t1, t2);
        this->split(t2, r - l, t2, t3);
        const size_type ret = this->find(t2, v, l, dir_left);
        this->merge(t2, t2, t3);
        this->merge(this->root, t1, t2);
        return ret;
    }
};

template<class OperandMonoid, class OperatorMonoid, OperandMonoid (*_map)(const OperandMonoid&, const OperatorMonoid&), OperatorMonoid (*_fold)(const OperatorMonoid&, internal::size_t)>
xorshift base<OperandMonoid,OperatorMonoid,_map,_fold>::rand(std::random_device{}());


template<class Action>
struct core : base<typename Action::operand,typename Action::operation,Action::map,Action::fold> {
    static_assert(
        Action::tags.none() or
        Action::tags.has(actions::flags::range_folding, actions::flags::range_operation)
    );

  public:
    using action = Action;

    using operand = typename action::operand;
    using operation = typename action::operation;

    using value_type = operand;
    using action_type = typename operation::value_type;

  private:
    using base = implicit_treap_impl::base<operand,operation,Action::map,Action::fold>;

  public:
    using size_type = typename base::size_type;

  protected:
    inline size_type _positivize_index(const size_type p) const noexcept(NO_EXCEPT) {
        return p < 0 ? this->size() + p : p;
    }

  public:
    template<class... Args>
    explicit core(Args... args) noexcept(NO_EXCEPT) : core() { this->assign(std::forward<Args>(args)...); }
    template<class T> core(const std::initializer_list<T>& values) noexcept(NO_EXCEPT) : core(std::begin(values), std::end(values)) {}
    core() noexcept(NO_EXCEPT) {}

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    inline void insert(size_type p, const I first, const I last) noexcept(NO_EXCEPT) {
        for(auto itr=first; itr != last; ++itr, ++p) {
            this->insert(p, *itr);
        }
    }

    bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }

    struct point_reference : internal::point_reference<core> {
        point_reference(core *const super, const size_type p) noexcept(NO_EXCEPT)
          : internal::point_reference<core>(super, super->_positivize_index(p))
        {
            assert(0 <= this->_pos && this->_pos < this->_super->size());
        }

        operator value_type() const noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }
        value_type val() const noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }

        inline point_reference& set(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->set(this->_pos, v);
            return *this;
        }
        inline point_reference& operator=(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->set(this->_pos, v);
            return *this;
        }

        inline point_reference& apply(const action_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator<<=(const action_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
    };

    struct range_reference : internal::range_reference<core> {
        range_reference(core *const super, const size_type l, const size_type r) noexcept(NO_EXCEPT)
          : internal::range_reference<core>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

        inline value_type fold() noexcept(NO_EXCEPT) {
            if(this->_begin == 0 and this->_end == this->_super->size()) return this->_super->fold();
            return this->_super->fold(this->_begin, this->_end);
        }

        inline range_reference& fill(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->fill(this->_begin, this->_end, v);
            return *this;
        }

        inline range_reference& apply(const action_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }
        inline range_reference& operator<<=(const action_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }

        inline size_type find(const value_type& v, const bool dir_left = true) const noexcept(NO_EXCEPT) {
            return this->_super->base::find(this->_begin, this->_end, v, dir_left);
        }
    };


    inline void insert(size_type p, const value_type& v) noexcept(NO_EXCEPT) {
        p = this->_positivize_index(p), assert(0 <= p && p <= this->size());
        this->base::insert(p, v);
    }

    inline void insert(const size_type p, const value_type& v, const size_type count) noexcept(NO_EXCEPT) {
        REP(k, count) this->insert(p + k, v);
    }

    inline void push_front(const value_type& v, const size_type count = 1) noexcept(NO_EXCEPT) { this->insert(0, v, count); }
    inline void push_back(const value_type& v, const size_type count = 1) noexcept(NO_EXCEPT) { this->insert(this->size(), v, count); }

    inline void resize(const size_type size, const value_type& v = value_type{}) noexcept(NO_EXCEPT) {
        REP(this->size() - size) this->erase(-1);
        REP(i, this->size(), size) this->push_back(v);
    }

    inline void assign(const size_type size, const value_type& v = value_type{}) noexcept(NO_EXCEPT) {
        this->clear(), this->insert(0, v, size);
    }

    template<class T> inline void assign(const std::initializer_list<T>& values) noexcept(NO_EXCEPT) { this->assign(std::begin(values), std::end(values)); }

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    inline void assign(const I first, const I last) noexcept(NO_EXCEPT) { this->clear(), this->insert(0, first, last); }

    inline void fill(const value_type& v) noexcept(NO_EXCEPT) {
        const size_type count = this->size();
        this->clear(), this->insert(0, v, count);
    }
    inline void fill(const size_type l, const size_type r, const value_type& v) noexcept(NO_EXCEPT) {
        REP(p, l, r) this->erase(p), this->insert(p, v);
    }

    inline void erase(size_type p) noexcept(NO_EXCEPT) {
        p = this->_positivize_index(p), assert(0 <= p && p < this->size());
        this->base::erase(p);
    }
    inline void erase(const size_type p, const size_type count) noexcept(NO_EXCEPT) { REP(count) this->erase(p); }
    inline void clear() noexcept(NO_EXCEPT) { this->erase(0, this->size()); }

    inline void pop_front(const size_type count = 1) noexcept(NO_EXCEPT) { this->erase(0, count); }
    inline void pop_back(const size_type count = 1) noexcept(NO_EXCEPT) { this->erase(this->size() - count, count); }


    inline void apply(size_type l, size_type r, const action_type& v) noexcept(NO_EXCEPT) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->size());
        this->base::apply(l, r, v);
    }
    inline void apply(const size_type p, const action_type& v) noexcept(NO_EXCEPT) { this->apply(p, p+1, v); }
    inline void apply(const action_type& v) noexcept(NO_EXCEPT) { this->apply(0, this->size(), v); }


    inline value_type get(size_type p) const noexcept(NO_EXCEPT) {
        p = this->_positivize_index(p), assert(0 <= p && p < this->size());
        return this->base::fold(p, p+1).val();
    }

    inline point_reference operator[](const size_type p) noexcept(NO_EXCEPT) { return point_reference(this, p); }
    inline range_reference operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) { return range_reference(this, l, r); }

    inline value_type front() const noexcept(NO_EXCEPT) { return this->get(0); }
    inline value_type back() const noexcept(NO_EXCEPT) { return this->get(this->size()-1); }

    inline value_type fold(size_type l, size_type r) const noexcept(NO_EXCEPT) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->size());
        return this->base::fold(l, r).val();
    }
    inline value_type fold() const noexcept(NO_EXCEPT) { return this->fold(0, this->size()); }

    inline void reverse(size_type l, size_type r) const noexcept(NO_EXCEPT) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->size());
        this->base::reverse(l, r);
    }
    inline void reverse() const noexcept(NO_EXCEPT) { this->reverse(0, this->size()); }

    inline void rotate(size_type l, size_type m, size_type r) const noexcept(NO_EXCEPT) {
        l = this->_positivize_index(l), m = this->_positivize_index(m), r = this->_positivize_index(r);
        assert(0 <= l && l <= m && m < r && r <= this->size());
        this->base::rotate(l, m, r);
    }
    inline void rotate(const size_type m) const noexcept(NO_EXCEPT) { this->rotate(0, m, this->size()); }

    inline size_type find(size_type l, size_type r, const value_type& v, const bool dir_left = true) const noexcept(NO_EXCEPT) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->size());
        return this->base::find(l, r, v, dir_left);
    }
    inline size_type find(const value_type& v, const bool dir_left = true) const noexcept(NO_EXCEPT) {
        return this->find(0, this->size(), v, dir_left);
    }

  protected:
    using iterator_interface = internal::container_iterator_interface<value_type,core>;

  public:
    struct iterator : virtual iterator_interface {
        iterator(const core *const ref, const size_type p) noexcept(NO_EXCEPT) : iterator_interface(ref, p) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
        inline value_type operator[](const typename iterator_interface::difference_type count) const noexcept(NO_EXCEPT) { return *(*this + count); }
    };

    inline iterator begin() const noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline iterator end() const noexcept(NO_EXCEPT) { return iterator(this, this->size()); }
};


} // namespace implicit_treap_impl

} // namespace internal


template<class action> struct implicit_treap : internal::implicit_treap_impl::core<action> {
    using internal::implicit_treap_impl::core<action>::core;
};


} // namespace lib
