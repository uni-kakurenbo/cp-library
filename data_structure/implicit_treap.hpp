#pragma once

#include <utility>
#include <type_traits>
#include <memory>
#include <initializer_list>

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
template<class OperandMonoid, class OperatorMonoid, OperandMonoid (*g)(const OperandMonoid&, const OperatorMonoid&), OperatorMonoid (*p)(const OperatorMonoid&, internal::size_t)>
struct base : private uncopyable {
    using operand_monoid = OperandMonoid;
    using operator_monoid = OperatorMonoid;

    using size_t = internal::size_t;

  private:
    struct node;
    using Tree = node*;

    mutable xorshift rand;

    struct node {
        operand_monoid value, acc;
        operator_monoid lazy;

        xorshift::result_type priority;

        size_t cnt = 1;

        bool rev = false;
        Tree left = nullptr, right = nullptr;

        node(const operand_monoid& value, const base* super)
        : value(value), acc(operand_monoid{}), lazy(operator_monoid{}), priority(super->rand()) {}
        ~node() {
            if(this->left) delete this->left;
            if(this->right) delete this->right;
        }
    };

    Tree root = nullptr;

    inline size_t cnt(const Tree tree) const { return tree ? tree->cnt : 0; }

    inline operand_monoid acc(const Tree tree) const { return tree ? tree->acc : operand_monoid{}; }

    inline void update_cnt(const Tree tree) const { if(tree) tree->cnt = 1 + this->cnt(tree->left) + this->cnt(tree->right); }

    inline void update_acc(const Tree tree) const {
        if(tree) tree->acc = this->acc(tree->left) * tree->value * this->acc(tree->right);
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
                tree->left->lazy = tree->left->lazy * tree->lazy;
                tree->left->acc = g(tree->left->acc, p(tree->lazy, this->cnt(tree->left)));
            }
            if(tree->right) {
                tree->right->lazy = tree->right->lazy * tree->lazy;
                tree->right->acc = g(tree->right->acc, p(tree->lazy, this->cnt(tree->right)));
            }
            tree->value = g(tree->value, p(tree->lazy, 1));
            tree->lazy = operator_monoid{};
        }
        // this->pushup(tree);
    }

    inline void split(const Tree tree, const size_t key, Tree& left, Tree& right) const {
        if(!tree) {
            left = right = nullptr;
            return;
        }
        this->pushdown(tree);
        const size_t implicit_key = this->cnt(tree->left) + 1;
        if(key < implicit_key) {
            this->split(tree->left, key, left, tree->left), right = std::move(tree);
            this->pushup(right);
        }
        else {
            this->split(tree->right, key - implicit_key, tree->right, right), left = std::move(tree);
            this->pushup(left);
        }
    }

    inline void insert(Tree& tree, const size_t key, const Tree item) const {
        Tree t1, t2;
        this->split(tree, key, t1, t2);
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

    inline void erase(Tree& tree, const size_t key) const {
        Tree t1, t2, t3;
        this->split(tree, key + 1, t1, t2);
        this->split(t1, key, t1, t3);
        this->merge(tree, t1, t2);
        if(t3) delete t3;
    }

    inline void apply(Tree tree, const size_t first, const size_t last, const operator_monoid& value) const {
        if(first >= last) return;
        Tree t1, t2, t3;
        this->split(tree, first, t1, t2);
        this->split(t2, last - first, t2, t3);
        t2->lazy = t2->lazy * value;
        // t2->acc = g(t2->acc, p(value, cnt(t2)));
        this->merge(t2, t2, t3);
        this->merge(tree, t1, t2);
    }

    inline operand_monoid prod(Tree tree, const size_t first, const size_t last) const {
        if(first == last) return operand_monoid{};
        Tree t1, t2, t3;
        this->split(tree, first, t1, t2);
        this->split(t2, last - first, t2, t3);
        operand_monoid ret = t2->acc;
        this->merge(t2, t2, t3);
        this->merge(tree, t1, t2);
        return ret;
    }

    // [first, last)の中で左から何番目か
    inline size_t find(const Tree tree, operand_monoid& value, const size_t offset, const bool dir_left = true) const {
        if(tree->acc * value == value) {
            return -1;
        } else {
            if(dir_left) {
                if(tree->left and tree->left->acc * value != value) {
                    return this->find(tree->left, value, offset, dir_left);
                } else {
                    return tree->value * value != value ? offset + this->cnt(tree->left) : this->find(tree->right, value, offset + this->cnt(tree->left) + 1, dir_left);
                }
            } else {
                if(tree->right and tree->right->acc * value != value) {
                    return this->find(tree->right, value, offset + this->cnt(tree->left) + 1, dir_left);
                } else {
                    return tree->value * value != value ? offset + this->cnt(tree->left) : this->find(tree->left, value, offset, dir_left);
                }
            }
        }
    }

    inline void reverse(Tree tree, const size_t first, const size_t last) const {
        if(first > last) return;
        Tree t1, t2, t3;
        this->split(tree, first, t1, t2);
        this->split(t2, last - first, t2, t3);
        t2->rev ^= 1;
        this->merge(t2, t2, t3);
        this->merge(tree, t1, t2);
    }

    // [first, last)の先頭がmになるようにシフトさせる。std::rotateと同じ仕様
    inline void rotate(Tree tree, const size_t first, const size_t middle, const size_t last) const {
        this->reverse(tree, first, last);
        this->reverse(tree, first, first + last - middle);
        this->reverse(tree, first + last - middle, last);
    }

  public:
    inline size_t size() const { return this->cnt(this->root); }

  protected:
    virtual ~base() { delete this->root; }

  public:
    // void insert(const size_t pos, const operand_monoid& value) { this->insert(this->root, pos, std::make_shared<node>(value, this)); }
    void insert(const size_t pos, const operand_monoid& value) { this->insert(this->root, pos, new node(value, this)); }

    inline void apply(const size_t first, const size_t last, const operator_monoid& value) { this->apply(this->root, first, last, value); }

    inline operand_monoid prod(const size_t first, const size_t last) const { return this->prod(this->root, first, last); }

    inline void erase(const size_t pos) { this->erase(this->root, pos); }

    inline void reverse(const size_t first, const size_t last) const { this->reverse(this->root, first, last); }

    inline void rotate(const size_t first, const size_t middle, const size_t last) const { this->rotate(this->root, first, middle, last); }

    // 二分探索。[first, last)内のkでf0(tr[k], value) != xとなる最左/最右のもの。存在しない場合は-1
    // たとえばMinMonoidの場合、x未満の最左/最右の要素の位置を返す
    inline size_t find(const size_t first, const size_t last, const operand_monoid& value, const bool dir_left) const {
        if(first >= last) return -1;
        Tree t1, t2, t3;
        this->split(this->root, first, t1, t2);
        this->split(t2, last - first, t2, t3);
        const size_t ret = this->find(t2, value, first, dir_left);
        this->merge(t2, t2, t3);
        this->merge(this->root, t1, t2);
        return ret;
    }
};


template<class Action>
struct core : implicit_treap_lib::base<typename Action::operand_monoid,typename Action::operator_monoid,Action::map,Action::compose> {
  public:
    using action = Action;

    using operand_monoid = typename action::operand_monoid;
    using operator_monoid = typename action::operator_monoid;

    using value_type = typename operand_monoid::value_type;
    using action_type = typename operator_monoid::value_type;

  private:
    using base = implicit_treap_lib::base<operand_monoid,operator_monoid,Action::map,Action::compose>;

  public:
    using size_t = typename base::size_t;

  protected:
    inline void _validate_index_in_right_open(const size_t _pos) const {
        dev_assert(0 <= _pos and _pos < this->size());
    }
    inline void _validate_index_in_closed(const size_t _pos) const {
        dev_assert(0 <= _pos and _pos <= this->size());
    }
    inline void _validate_rigth_open_interval(const size_t _l, const size_t _r) const {
        dev_assert(0 <= _l and _l <= _r and _r <= this->size());
    }

    inline size_t _positivize_index(const size_t pos) const {
        return pos < 0 ? this->size() + pos : pos;
    }

  public:
    template<class... Args>
    explicit core(Argsand... args) : core() { this->assign(std::forward<Args>(args)...); }
    core(const std::initializer_list<value_type>& values) : core(std::begin(values), std::end(values)) {}
    core() { static_assert(action::tags.has(actions::flags::implicit_treap)); }

    template<class I, std::enable_if_t<std::is_same_v<value_type, typename std::iterator_traits<I>::value_type>>* = nullptr>
    inline void insert(size_t pos, const I first, const I last) {
        for(auto itr=first; itr != last; ++itr, ++pos) {
            this->insert(pos, *itr);
        }
    }

    inline void insert(size_t pos, const value_type& value) {
        pos = this->_positivize_index(pos);
        this->_validate_index_in_closed(pos);
        this->base::insert(pos, value);
    }

    inline void insert(const size_t first, const value_type& value, const size_t count) {
        REP(pos, count) this->insert(first + pos, value);
    }

    inline void push_front(const value_type& value, const size_t count = 1) { this->insert(0, value, count); }
    inline void push_back(const value_type& value, const size_t count = 1) { this->insert(this->size(), value, count); }

    inline void resize(const size_t size, const value_type& value = value_type{}) {
        REP(this->size() - size) this->erase(-1);
        REP(i, this->size(), size) this->push_back(value);
    }

    inline void assign(const size_t size, const value_type& value = value_type{}) {
        this->clear(), this->insert(0, value, size);
    }

    inline void assign(const std::initializer_list<value_type>& values) { this->assign(std::begin(values), std::end(values)); }

    template<class I, std::enable_if_t<std::is_same_v<value_type, typename std::iterator_traits<I>::value_type>>* = nullptr>
    inline void assign(const I first, const I second) { this->clear(), this->insert(0, first, second); }

    inline void fill(const value_type& value) {
        const size_t count = this->size();
        this->clear(), this->insert(0, value, count);
    }
    inline void fill(const size_t first, const size_t last, const value_type& value) {
        REP(pos, first, last) this->erase(pos), this->insert(pos, value);
    }

    inline void erase(size_t pos) {
        pos = this->_positivize_index(pos);
        this->_validate_index_in_right_open(pos);
        this->base::erase(pos);
    }
    inline void erase(const size_t pos, const size_t count) { REP(count) this->erase(pos); }
    inline void clear() { this->erase(0, this->size()); }

    inline void pop_front(const size_t count = 1) { this->erase(0, count); }
    inline void pop_back(const size_t count = 1) { this->erase(this->size() - count, count); }


    inline void apply(size_t first, size_t last, const action_type& value) {
        first = this->_positivize_index(first), last = this->_positivize_index(last);
        this->_validate_rigth_open_interval(first, last);
        this->base::apply(first, last, value);
    }
    inline void apply(const size_t p, const action_type& value) { this->apply(p, p+1, value); }
    inline void apply(const action_type& value) { this->apply(0, this->size(), value); }


    inline value_type get(size_t pos) const {
        pos = this->_positivize_index(pos);
        this->_validate_index_in_right_open(pos);
        return this->base::prod(pos, pos+1).val();
    }
    inline value_type operator[](const size_t pos) const { return this->get(pos); }

    inline value_type front() const { return this->get(0); }
    inline value_type back() const { return this->get(this->size()-1); }

    inline value_type prod(size_t first, size_t last) const {
        first = this->_positivize_index(first), last = this->_positivize_index(last);
        this->_validate_rigth_open_interval(first, last);
        return this->base::prod(first, last).val();
    }
    inline value_type prod() const { return this->prod(0, this->size()); }

    inline void reverse(size_t first, size_t last) const {
        first = this->_positivize_index(first), last = this->_positivize_index(last);
        this->_validate_rigth_open_interval(first, last);
        this->base::reverse(first, last);
    }
    inline void reverse() const { this->reverse(0, this->size()); }

    inline void rotate(size_t first, size_t middle, size_t last) const {
        first = this->_positivize_index(first), middle = this->_positivize_index(middle), last = this->_positivize_index(last);
        this->_validate_rigth_open_interval(first, last), dev_assert(first <= middle and middle < last);
        this->base::rotate(first, middle, last).val();
    }
    inline void rotate(const size_t middle) const { this->rotate(0, middle, this->size()); }

    inline size_t find(size_t first, size_t last, const value_type& value, const bool dir_left = true) const {
        first = this->_positivize_index(first), last = this->_positivize_index(last);
        this->_validate_index_right_closed(first), this->_validate_index_right_closed(last);
        return this->base::find(first, last, value, dir_left);
    }
    inline size_t find(const value_type& value, const bool dir_left = true) const {
        return this->find(0, this->size(), value, dir_left);
    }


    struct iterator : virtual internal::container_iterator_interface<value_type,core> {
        iterator(const core *const ref, const size_t pos) : internal::container_iterator_interface<value_type,core>(ref, pos) {}

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
