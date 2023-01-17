#pragma once


#include <utility>
#include <type_traits>
#include <initializer_list>
#include <random>

#include "snippet/internal/types.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_assert.hpp"

#include "internal/uncopyable.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "random/xorshift.hpp"

#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace internal {

namespace implicit_treap_lib {


// Thanks to: https://github.com/xuzijian629/library2/blob/master/treap/implicit_treap.cpp
template<class OperandMonoid, class OperatorMonoid, OperandMonoid (*map)(const OperandMonoid&, const OperatorMonoid&), OperatorMonoid (*fold)(const OperatorMonoid&, internal::size_t)>
struct base : private uncopyable {
    using operand_monoid = OperandMonoid;
    using operator_monoid = OperatorMonoid;

    using size_type = internal::size_t;

  private:
    struct node;
    using Tree = node*;

    static xorshift rand;

    struct node {
        operand_monoid v, acc;
        operator_monoid lazy;

        xorshift::result_type priority;

        size_type cnt = 1;

        bool rev = false;
        Tree left = nullptr, right = nullptr;

        node(const operand_monoid& v, const base* super)
        : v(v), acc(operand_monoid{}), lazy(operator_monoid{}), priority(super->rand()) {}
        ~node() {
            if(this->left) delete this->left;
            if(this->right) delete this->right;
        }
    };

    Tree root = nullptr;

    inline size_type cnt(const Tree tree) const { return tree ? tree->cnt : 0; }

    inline operand_monoid acc(const Tree tree) const { return tree ? tree->acc : operand_monoid{}; }

    inline void update_cnt(const Tree tree) const { if(tree) tree->cnt = 1 + this->cnt(tree->left) + this->cnt(tree->right); }

    inline void update_acc(const Tree tree) const {
        if(tree) tree->acc = this->acc(tree->left) * tree->v * this->acc(tree->right);
    }

    inline void pushup(const Tree tree) const { this->update_cnt(tree), update_acc(tree); }

    inline void pushdown(const Tree tree) const {
        if(tree and tree->rev) {
            tree->rev = false;
            std::swap(tree->left, tree->right);
            if(tree->left) tree->left->rev ^= 1;
            if(tree->right) tree->right->rev ^= 1;
        }
        if(tree and tree->lazy != operator_monoid{}) {
            if(tree->left) {
                tree->left->lazy = tree->lazy * tree->left->lazy;
                tree->left->acc = map(tree->left->acc, fold(tree->lazy, this->cnt(tree->left)));
            }
            if(tree->right) {
                tree->right->lazy = tree->lazy * tree->right->lazy;
                tree->right->acc = map(tree->right->acc, fold(tree->lazy, this->cnt(tree->right)));
            }
            tree->v = map(tree->v, fold(tree->lazy, 1));
            tree->lazy = operator_monoid{};
        }
        // this->pushup(tree);
    }

    inline void split(const Tree tree, const size_type pos, Tree& left, Tree& right) const {
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

    inline void insert(Tree& tree, const size_type pos, const Tree item) const {
        Tree t1, t2;
        this->split(tree, pos, t1, t2);
        this->merge(t1, t1, item);
        this->merge(tree, t1, t2);
    }

    inline void merge(Tree& tree, const Tree left, const Tree right) const {
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

    inline void erase(Tree& tree, const size_type pos) const {
        Tree t1, t2, t3;
        this->split(tree, pos + 1, t1, t2);
        this->split(t1, pos, t1, t3);
        this->merge(tree, t1, t2);
        if(t3) delete t3;
    }

    inline void apply(Tree tree, const size_type l, const size_type r, const operator_monoid& v) const {
        if(l >= r) return;
        Tree t1, t2, t3;
        this->split(tree, l, t1, t2);
        this->split(t2, r - l, t2, t3);
        t2->lazy = v * t2->lazy;
        // t2->acc = map(t2->acc, fold(v, cnt(t2)));
        this->merge(t2, t2, t3);
        this->merge(tree, t1, t2);
    }

    inline operand_monoid prod(Tree tree, const size_type l, const size_type r) const {
        if(l == r) return operand_monoid{};
        Tree t1, t2, t3;
        this->split(tree, l, t1, t2);
        this->split(t2, r - l, t2, t3);
        operand_monoid ret = t2->acc;
        this->merge(t2, t2, t3);
        this->merge(tree, t1, t2);
        return ret;
    }

    // [l, r)の中で左から何番目か
    inline size_type find(const Tree tree, operand_monoid& v, const size_type offset, const bool dir_left = true) const {
        if(tree->acc * v == v) {
            return -1;
        } else {
            if(dir_left) {
                if(tree->left and tree->left->acc * v != v) {
                    return this->find(tree->left, v, offset, dir_left);
                } else {
                    return tree->v * v != v ? offset + this->cnt(tree->left) : this->find(tree->right, v, offset + this->cnt(tree->left) + 1, dir_left);
                }
            } else {
                if(tree->right and tree->right->acc * v != v) {
                    return this->find(tree->right, v, offset + this->cnt(tree->left) + 1, dir_left);
                } else {
                    return tree->v * v != v ? offset + this->cnt(tree->left) : this->find(tree->left, v, offset, dir_left);
                }
            }
        }
    }

    inline void reverse(Tree tree, const size_type l, const size_type r) const {
        if(l > r) return;
        Tree t1, t2, t3;
        this->split(tree, l, t1, t2);
        this->split(t2, r - l, t2, t3);
        t2->rev ^= 1;
        this->merge(t2, t2, t3);
        this->merge(tree, t1, t2);
    }

    // Same usage as: std::rotate(l, m, r)
    inline void rotate(Tree tree, const size_type l, const size_type m, const size_type r) const {
        this->reverse(tree, l, r);
        this->reverse(tree, l, l + r - m);
        this->reverse(tree, l + r - m, r);
    }

  public:
    inline size_type size() const { return this->cnt(this->root); }

  protected:
    virtual ~base() { delete this->root; }

  public:
    // void insert(const size_type p, const operand_monoid& v) { this->insert(this->root, p, std::make_shared<node>(v, this)); }
    void insert(const size_type p, const operand_monoid& v) { this->insert(this->root, p, new node(v, this)); }

    inline void apply(const size_type l, const size_type r, const operator_monoid& v) { this->apply(this->root, l, r, v); }

    inline operand_monoid prod(const size_type l, const size_type r) const { return this->prod(this->root, l, r); }

    inline void erase(const size_type p) { this->erase(this->root, p); }

    inline void reverse(const size_type l, const size_type r) const { this->reverse(this->root, l, r); }

    inline void rotate(const size_type l, const size_type m, const size_type r) const { this->rotate(this->root, l, m, r); }

    // 二分探索。[l, r)内のkでf0(tr[k], v) != xとなる最左/最右のもの。存在しない場合は-1
    // たとえばMinMonoidの場合、x未満の最左/最右の要素の位置を返す
    inline size_type find(const size_type l, const size_type r, const operand_monoid& v, const bool dir_left) const {
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

template<class OperandMonoid, class OperatorMonoid, OperandMonoid (*map)(const OperandMonoid&, const OperatorMonoid&), OperatorMonoid (*fold)(const OperatorMonoid&, internal::size_t)>
xorshift base<OperandMonoid,OperatorMonoid,map,fold>::rand(std::random_device{}());


template<class Action>
struct core : implicit_treap_lib::base<typename Action::operand_monoid,typename Action::operator_monoid,Action::map,Action::fold> {
  public:
    using action = Action;

    using operand_monoid = typename action::operand_monoid;
    using operator_monoid = typename action::operator_monoid;

    using value_type = typename operand_monoid::value_type;
    using action_type = typename operator_monoid::value_type;

  private:
    using base = implicit_treap_lib::base<operand_monoid,operator_monoid,Action::map,Action::fold>;

  public:
    using size_type = typename base::size_type;

  protected:
    inline void _validate_index_in_right_open([[maybe_unused]] const size_type p) const {
        dev_assert(0 <= p and p < this->size());
    }
    inline void _validate_index_in_closed ([[maybe_unused]] const size_type p) const {
        dev_assert(0 <= p and p <= this->size());
    }
    inline void _validate_rigth_open_interval([[maybe_unused]] const size_type l, [[maybe_unused]] const size_type r) const {
        dev_assert(0 <= l and l <= r and r <= this->size());
    }

    inline size_type _positivize_index(const size_type p) const {
        return p < 0 ? this->size() + p : p;
    }

  public:
    template<class... Args>
    explicit core(Args... args) : core() { this->assign(std::forward<Args>(args)...); }
    template<class T> core(const std::initializer_list<T>& values) : core(std::begin(values), std::end(values)) {}
    core() { static_assert(action::tags.bits() == 0 or action::tags.has(actions::flags::implicit_treap)); }

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    inline void insert(size_type p, const I first, const I last) {
        for(auto itr=first; itr != last; ++itr, ++p) {
            this->insert(p, *itr);
        }
    }

    inline void insert(size_type p, const value_type& v) {
        p = this->_positivize_index(p);
        this->_validate_index_in_closed(p);
        this->base::insert(p, v);
    }

    inline void insert(const size_type p, const value_type& v, const size_type count) {
        REP(k, count) this->insert(p + k, v);
    }

    inline void push_front(const value_type& v, const size_type count = 1) { this->insert(0, v, count); }
    inline void push_back(const value_type& v, const size_type count = 1) { this->insert(this->size(), v, count); }

    inline void resize(const size_type size, const value_type& v = value_type{}) {
        REP(this->size() - size) this->erase(-1);
        REP(i, this->size(), size) this->push_back(v);
    }

    inline void assign(const size_type size, const value_type& v = value_type{}) {
        this->clear(), this->insert(0, v, size);
    }

    template<class T> inline void assign(const std::initializer_list<T>& values) { this->assign(std::begin(values), std::end(values)); }

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    inline void assign(const I first, const I last) { this->clear(), this->insert(0, first, last); }

    inline void fill(const value_type& v) {
        const size_type count = this->size();
        this->clear(), this->insert(0, v, count);
    }
    inline void fill(const size_type l, const size_type r, const value_type& v) {
        REP(p, l, r) this->erase(p), this->insert(p, v);
    }

    inline void erase(size_type p) {
        p = this->_positivize_index(p);
        this->_validate_index_in_right_open(p);
        this->base::erase(p);
    }
    inline void erase(const size_type p, const size_type count) { REP(count) this->erase(p); }
    inline void clear() { this->erase(0, this->size()); }

    inline void pop_front(const size_type count = 1) { this->erase(0, count); }
    inline void pop_back(const size_type count = 1) { this->erase(this->size() - count, count); }


    inline void apply(size_type l, size_type r, const action_type& v) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);
        this->base::apply(l, r, v);
    }
    inline void apply(const size_type p, const action_type& v) { this->apply(p, p+1, v); }
    inline void apply(const action_type& v) { this->apply(0, this->size(), v); }


    inline value_type get(size_type p) const {
        p = this->_positivize_index(p);
        this->_validate_index_in_right_open(p);
        return this->base::prod(p, p+1).val();
    }
    inline value_type operator[](const size_type p) const { return this->get(p); }

    inline value_type front() const { return this->get(0); }
    inline value_type back() const { return this->get(this->size()-1); }

    inline value_type prod(size_type l, size_type r) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);
        return this->base::prod(l, r).val();
    }
    inline value_type prod() const { return this->prod(0, this->size()); }

    inline void reverse(size_type l, size_type r) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r);
        this->base::reverse(l, r);
    }
    inline void reverse() const { this->reverse(0, this->size()); }

    inline void rotate(size_type l, size_type m, size_type r) const {
        l = this->_positivize_index(l), m = this->_positivize_index(m), r = this->_positivize_index(r);
        this->_validate_rigth_open_interval(l, r), dev_assert(l <= m and m < r);
        this->base::rotate(l, m, r).val();
    }
    inline void rotate(const size_type m) const { this->rotate(0, m, this->size()); }

    inline size_type find(size_type l, size_type r, const value_type& v, const bool dir_left = true) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        this->_validate_index_right_closed(l), this->_validate_index_right_closed(r);
        return this->base::find(l, r, v, dir_left);
    }
    inline size_type find(const value_type& v, const bool dir_left = true) const {
        return this->find(0, this->size(), v, dir_left);
    }


    struct iterator : virtual internal::container_iterator_interface<value_type,core> {
        iterator(const core *const ref, const size_type p) : internal::container_iterator_interface<value_type,core>(ref, p) {}

        inline value_type operator*() const override { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


} // namespace implicit_treap_lib

} // namespace internal


template<class action> struct implicit_treap : internal::implicit_treap_lib::core<action> {
    using internal::implicit_treap_lib::core<action>::core;
};


} // namespace lib
