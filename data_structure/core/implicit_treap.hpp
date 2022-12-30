#pragma once

#include <utility>
#include <type_traits>
#include <memory>

#include "snippet/internal/types.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_assert.hpp"

#include "internal/uncopyable.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "random/xorshift.hpp"

namespace lib {

namespace implicit_treap_lib {


// Thanks to: https://github.com/xuzijian629/library2/blob/master/treap/implicit_treap.cpp
template<class T0, class T1>
struct base : private uncopyable {
    using size_t = internal::size_t;

  private:
    // T0上の演算、単位元
    virtual T0 f0(T0, T0) const = 0;
    const T0 u0;

    // T1上の演算、単位元
    virtual T1 f1(T1, T1) const = 0;
    const T1 u1;

    // T0に対するT1の作用
    virtual T0 g(T0, T1) const = 0;

    // 多数のt1(T1)に対するf1の合成
    virtual T1 p(T1, size_t) const = 0;


    struct node;
    using Tree = node*;

    mutable xorshift rand;

    struct node {
        T0 value, acc;
        T1 lazy;

        xorshift::result_type priority;

        size_t cnt = 1;

        bool rev = false;
        Tree left = nullptr, right = nullptr;

        node(const T0& value, const base* super)
        : value(value), acc(super->u0), lazy(super->u1), priority(super->rand()) {}
        ~node() {
            if(this->left) delete this->left;
            if(this->right) delete this->right;
        }
    };

    Tree root = nullptr;

    inline size_t cnt(const Tree tree) const { return tree ? tree->cnt : 0; }

    inline T0 acc(const Tree tree) const { return tree ? tree->acc : u0; }

    inline void update_cnt(const Tree tree) const { if(tree) tree->cnt = 1 + this->cnt(tree->left) + this->cnt(tree->right); }

    inline void update_acc(const Tree tree) const {
        if(tree) tree->acc = this->f0(this->acc(tree->left), this->f0(tree->value, this->acc(tree->right)));
    }

    inline void pushup(const Tree tree) const { this->update_cnt(tree), update_acc(tree); }

    inline void pushdown(const Tree tree) const {
        if(tree && tree->rev) {
            tree->rev = false;
            std::swap(tree->left, tree->right);
            if(tree->left) tree->left->rev ^= 1;
            if(tree->right) tree->right->rev ^= 1;
        }
        if(tree && tree->lazy != u1) {
            if(tree->left) {
                tree->left->lazy = this->f1(tree->left->lazy, tree->lazy);
                tree->left->acc = this->g(tree->left->acc, this->p(tree->lazy, this->cnt(tree->left)));
            }
            if(tree->right) {
                tree->right->lazy = this->f1(tree->right->lazy, tree->lazy);
                tree->right->acc = this->g(tree->right->acc, this->p(tree->lazy, this->cnt(tree->right)));
            }
            tree->value = this->g(tree->value, this->p(tree->lazy, 1));
            tree->lazy = u1;
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

    inline void apply(Tree tree, const size_t first, const size_t last, const T1& value) const {
        if(first >= last) return;
        Tree t1, t2, t3;
        this->split(tree, first, t1, t2);
        this->split(t2, last - first, t2, t3);
        t2->lazy = this->f1(t2->lazy, value);
        // t2->acc = this->g(t2->acc, this->p(value, cnt(t2)));
        this->merge(t2, t2, t3);
        this->merge(tree, t1, t2);
    }

    inline T0 prod(Tree tree, const size_t first, const size_t last) const {
        if(first == last) return u0;
        Tree t1, t2, t3;
        this->split(tree, first, t1, t2);
        this->split(t2, last - first, t2, t3);
        T0 ret = t2->acc;
        this->merge(t2, t2, t3);
        this->merge(tree, t1, t2);
        return ret;
    }

    // [first, last)の中で左から何番目か
    inline size_t find(const Tree tree, T0& value, const size_t offset, const bool dir_left = true) const {
        if(this->f0(tree->acc, value) == value) {
            return -1;
        } else {
            if(dir_left) {
                if(tree->left && this->f0(tree->left->acc, value) != value) {
                    return this->find(tree->left, value, offset, dir_left);
                } else {
                    return (this->f0(tree->value, value) != value) ? offset + this->cnt(tree->left) : this->find(tree->right, value, offset + this->cnt(tree->left) + 1, dir_left);
                }
            } else {
                if(tree->right && this->f0(tree->right->acc, value) != value) {
                    return this->find(tree->right, value, offset + this->cnt(tree->left) + 1, dir_left);
                } else {
                    return (this->f0(tree->value, value) != value) ? offset + this->cnt(tree->left) : this->find(tree->left, value, offset, dir_left);
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
    explicit base(const T0& u0, const T1& u1) : u0(u0), u1(u1) {}
    virtual ~base() { delete this->root; }

    // void insert(const size_t pos, const T0& value) { this->insert(this->root, pos, std::make_shared<node>(value, this)); }
    void insert(const size_t pos, const T0& value) { this->insert(this->root, pos, new node(value, this)); }

    inline void apply(const size_t first, const size_t last, const T1& value) { this->apply(this->root, first, last, value); }

    inline T0 prod(const size_t first, const size_t last) const { return this->prod(this->root, first, last); }

    inline void erase(const size_t pos) { this->erase(this->root, pos); }

    inline void reverse(const size_t first, const size_t last) const { this->reverse(this->root, first, last); }

    inline void rotate(const size_t first, const size_t middle, const size_t last) const { this->rotate(this->root, first, middle, last); }

    // 二分探索。[first, last)内のkでf0(tr[k], value) != xとなる最左/最右のもの。存在しない場合は-1
    // たとえばMinMonoidの場合、x未満の最左/最右の要素の位置を返す
    inline size_t find(const size_t first, const size_t last, const T0& value, const bool dir_left) const {
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


} // namespace implicit_treap_lib


template<class T0, class T1>
struct implicit_treap : implicit_treap_lib::base<T0,T1> {
  private:
    using base = implicit_treap_lib::base<T0,T1>;

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
    explicit implicit_treap(const T0 u0, const T1 u1) : base::base(u0, u1) {}

  protected:
    template<class... Args>
    inline void initialize(Args&&... args) { this->assign(std::forward<Args>(args)...); }
    inline void initialize() {}

  public:

    template<class I, std::enable_if_t<std::is_same_v<T0, typename std::iterator_traits<I>::value_type>>* = nullptr>
    inline void insert(size_t pos, const I first, const I last) {
        for(auto itr=first; itr != last; ++itr, ++pos) {
            this->insert(pos, *itr);
        }
    }

    inline void insert(size_t pos, const T0& value) {
        pos = this->_positivize_index(pos);
        this->_validate_index_in_closed(pos);
        this->base::insert(pos, value);
    }

    inline void insert(const size_t first, const T0& value, const size_t count) {
        REP(pos, count) this->insert(first + pos, value);
    }

    inline void push_front(const T0& value, const size_t count = 1) { this->insert(0, value, count); }
    inline void push_back(const T0& value, const size_t count = 1) { this->insert(this->size(), value, count); }

    inline void resize(const size_t size, const T0& value = T0{}) {
        REP(this->size() - size) this->erase(-1);
        REP(i, this->size(), size) this->push_back(value);
    }

    inline void assign(const size_t size, const T0& value = T0{}) {
        this->clear(), this->insert(0, value, size);
    }

    template<class I, std::enable_if_t<std::is_same_v<T0, typename std::iterator_traits<I>::value_type>>* = nullptr>
    inline void assign(const I first, const I second) { this->clear(), this->insert(0, first, second); }

    inline void fill(const T0& value) {
        const size_t count = this->size();
        this->clear(), this->insert(0, value, count);
    }
    inline void fill(const size_t first, const size_t last, const T0& value) {
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


    inline void apply(size_t first, size_t last, const T1& value) {
        first = this->_positivize_index(first), last = this->_positivize_index(last);
        this->_validate_rigth_open_interval(first, last);
        this->base::apply(first, last, value);
    }
    inline void apply(const size_t p, const T1& value) { this->apply(p, p+1, value); }
    inline void apply(const T1& value) { this->apply(0, this->size(), value); }


    inline T0 get(size_t pos) const {
        pos = this->_positivize_index(pos);
        this->_validate_index_in_right_open(pos);
        return this->base::prod(pos, pos+1);
    }
    inline T0 operator[](const size_t pos) const { return this->get(pos); }

    inline T0 front() const { return this->get(0); }
    inline T0 back() const { return this->get(this->size()-1); }

    inline T0 prod(size_t first, size_t last) const {
        first = this->_positivize_index(first), last = this->_positivize_index(last);
        this->_validate_rigth_open_interval(first, last);
        return this->base::prod(first, last);
    }
    inline T0 prod() const { return this->prod(0, this->size()); }

    inline void reverse(size_t first, size_t last) const {
        first = this->_positivize_index(first), last = this->_positivize_index(last);
        this->_validate_rigth_open_interval(first, last);
        this->base::reverse(first, last);
    }
    inline void reverse() const { this->reverse(0, this->size()); }

    inline void rotate(size_t first, size_t middle, size_t last) const {
        first = this->_positivize_index(first), middle = this->_positivize_index(middle), last = this->_positivize_index(last);
        this->_validate_rigth_open_interval(first, last), dev_assert(first <= middle and middle < last);
        this->base::rotate(first, middle, last);
    }
    inline void rotate(const size_t middle) const { this->rotate(0, middle, this->size()); }

    inline size_t find(size_t first, size_t last, const T0& value, const bool dir_left = true) const {
        first = this->_positivize_index(first), last = this->_positivize_index(last);
        this->_validate_index_right_closed(first), this->_validate_index_right_closed(last);
        return this->base::find(first, last, value, dir_left);
    }
    inline size_t find(const T0& value, const bool dir_left = true) const {
        return this->find(0, this->size(), value, dir_left);
    }


    struct iterator : virtual internal::container_iterator_base<T0,implicit_treap> {
        iterator(const implicit_treap *const ref, const size_t pos) : internal::container_iterator_base<T0,implicit_treap>(ref, pos) {}

        inline T0 operator*() const override { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


} // namespace lib
