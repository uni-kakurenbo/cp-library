#pragma once

#include <cstdint>
#include <algorithm>
#include <limits>
#include <utility>

#include "template.hpp"

#include "internal/develop/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "random/xorshift.hpp"


namespace Lib {

// Thanks to: https://github.com/xuzijian629/library2/blob/master/treap/implicit_treap.cpp

template<class T0, class T1>
class ImplicitTreap {
    // T0上の演算、単位元
    virtual T0 f0(T0, T0) const = 0;
    const T0 u0;

    // T1上の演算、単位元
    virtual T1 f1(T1, T1) const = 0;
    const T1 u1;

    // T0に対するT1の作用
    virtual T0 g(T0, T1) const = 0;

    // 多数のt1(T1)に対するf1の合成
    virtual T1 p(T1, int) const = 0;

    XorShift random_engine;
    using Priority = XorShift::result_type;

    struct Node {
        T0 value, acc;
        T1 lazy;

        Priority priority;
        Internal::Size cnt = 1;

        bool rev = false;
        Node *l = nullptr, *r = nullptr;

        Node(const T0 value_, const Priority priority_, const T0 u0_, const T1 u1_) : value(value_), acc(u0_), lazy(u1_), priority(priority_) {}
    };

    Node *root = nullptr;

    using Tree = Node*;

    // TODO: iterator
    // using iterator = Iterator;

    Internal::Size cnt(const Tree t) const { return t ? t->cnt : 0; }

    T0 acc(const Tree t) const { return t ? t->acc : u0; }

    void update_acc(Tree t) const {
        if (t) {
            t->cnt = 1 + cnt(t->l) + cnt(t->r);
        }
    }

    void update_acc(Tree t) const {
        if (t) {
            t->acc = this->f0(acc(t->l), this->f0(t->value, acc(t->r)));
        }
    }

    void pushup(Tree t) const { this->this->update_acc(t), update_acc(t); }

    void pushdown(Tree t) const {
        if (t && t->rev) {
            t->rev = false;
            std::swap(t->l, t->r);
            if (t->l) t->l->rev ^= 1;
            if (t->r) t->r->rev ^= 1;
        }
        if (t && t->lazy != u1) {
            if (t->l) {
                t->l->lazy = this->f1(t->l->lazy, t->lazy);
                t->l->acc = this->g(t->l->acc, this->p(t->lazy, cnt(t->l)));
            }
            if (t->r) {
                t->r->lazy = this->f1(t->r->lazy, t->lazy);
                t->r->acc = this->g(t->r->acc, this->p(t->lazy, cnt(t->r)));
            }
            t->value = this->g(t->value, this->p(t->lazy, 1));
            t->lazy = u1;
        }
        this->pushup(t);
    }

    void split(Tree t, Internal::Size key, Tree &l, Tree &r) {
        if (!t) {
            l = r = nullptr;
            return;
        }
        pushdown(t);
        Internal::Size implicit_key = cnt(t->l) + 1;
        if (key < implicit_key) {
            split(t->l, key, l, t->l), r = t;
        }
        else {
            split(t->r, key - implicit_key, t->r, r), l = t;
        }
        this->pushup(t);
    }

    void insert(Tree &t, Internal::Size key, Tree item) {
        Tree t1, t2;
        split(t, key, t1, t2);
        merge(t1, t1, item);
        merge(t, t1, t2);
    }

    void merge(Tree &t, Tree l, Tree r) {
        pushdown(l);
        pushdown(r);
        if (!l || !r) {
            t = l ? l : r;
        } else if (l->priority > r->priority) {
            merge(l->r, l->r, r), t = l;
        } else {
            merge(r->l, l, r->l), t = r;
        }
        this->pushup(t);
    }

    void erase(Tree &t, Internal::Size key) {
        Tree t1, t2, t3;
        split(t, key + 1, t1, t2);
        split(t1, key, t1, t3);
        merge(t, t1, t2);
    }

    void update(Tree t, Internal::Size l, Internal::Size r, T1 x) {
        if (l >= r) return;
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2, t3);
        t2->lazy = this->f1(t2->lazy, x);
        t2->acc = this->g(t2->acc, this->p(x, cnt(t2)));
        merge(t2, t2, t3);
        merge(t, t1, t2);
    }

    T0 query(Tree t, Internal::Size l, Internal::Size r) {
        if (l == r) return u0;
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2, t3);
        T0 ret = t2->acc;
        merge(t2, t2, t3);
        merge(t, t1, t2);
        return ret;
    }

    // [l, r)の中で左から何番目か
    Internal::Size find(Tree t, T0 x, Internal::Size offset, bool left = true) {
        if (this->f0(t->acc, x) == x) {
            return -1;
        } else {
            if (left) {
                if (t->l && this->f0(t->l->acc, x) != x) {
                    return find(t->l, x, offset, left);
                } else {
                    return (this->f0(t->value, x) != x) ? offset + cnt(t->l) : find(t->r, x, offset + cnt(t->l) + 1, left);
                }
            } else {
                if (t->r && this->f0(t->r->acc, x) != x) {
                    return find(t->r, x, offset + cnt(t->l) + 1, left);
                } else {
                    return (this->f0(t->value, x) != x) ? offset + cnt(t->l) : find(t->l, x, offset, left);
                }
            }
        }
    }

    void reverse(Tree t, Internal::Size l, Internal::Size r) {
        if (l > r) return;
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2, t3);
        t2->rev ^= 1;
        merge(t2, t2, t3);
        merge(t, t1, t2);
    }

    // [l, r)の先頭がmになるようにシフトさせる。std::rotateと同じ仕様
    void rotate(Tree t, Internal::Size l, Internal::Size m, Internal::Size r) {
        reverse(t, l, r);
        reverse(t, l, l + r - m);
        reverse(t, l + r - m, r);
    }

    std::string _dump(Tree t) {
        if(!t) return "";
        this->pushdown(t);

        return this->_dump(t->l) + Debug::lit(t->value) + " " + this->_dump(t->r);
    }

public:
    ImplicitTreap(T0 u0_, T1 u1_) : u0(u0_), u1(u1_) {}

    template<class I, std::enable_if_t<std::is_same_v<T0,typename std::iterator_traits<I>::value_type>,std::nullptr_t> = nullptr>
    void insert(const I first, const I last) {
        int index = 0;
        for(auto itr=first; itr != last; ++itr, ++index) {
            this->insert(index, *itr);
        }
    }

    Internal::Size size() { return cnt(root); }

    void insert(Internal::Size pos, T0 x) { insert(root, pos, new Node(x, random_engine(), u0, u1)); }

    void update(Internal::Size l, Internal::Size r, T1 x) { update(root, l, r, x); }

    T0 query(Internal::Size l, Internal::Size r) { return query(root, l, r); }

    // 二分探索。[l, r)内のkでf0(tr[k], x) != xとなる最左/最右のもの。存在しない場合は-1
    // たとえばMinMonoidの場合、x未満の最左/最右の要素の位置を返す
    Internal::Size binary_search(Internal::Size l, Internal::Size r, T0 x, bool left = true) {
        if (l >= r) return -1;
        Tree t1, t2, t3;
        split(root, l, t1, t2);
        split(t2, r - l, t2, t3);
        Internal::Size ret = find(t2, x, l, left);
        merge(t2, t2, t3);
        merge(root, t1, t2);
        return ret;
    }

    void erase(Internal::Size pos) { erase(root, pos); }

    void reverse(Internal::Size l, Internal::Size r) { reverse(root, l, r); }

    void rotate(Internal::Size l, Internal::Size m, Internal::Size r) { rotate(root, l, m, r); }

    std::string _debug() {
        if constexpr(DEVELOPMENT_MODE) return this->_dump(root);
        return "";
    }

    T0 operator[](Internal::Size pos) { return query(pos, pos + 1); }
};

template <class T0, class T1>
struct RangeChmin : public ImplicitTreap<T0, T1> {
    using ImplicitTreap<T0, T1>::ImplicitTreap;
    RangeChmin() : RangeChmin(std::numeric_limits<T0>::max(), std::numeric_limits<T1>::min()) {}
    T0 f0(T0 x, T0 y) const override { return std::min(x, y); }
    T1 f1(T1 x, T1 y) const override { return y == std::numeric_limits<T1>::min() ? x : y; }
    T0 g(T0 x, T1 y) const override { return y == std::numeric_limits<T1>::min() ? x : y; }
    T1 p(T1 x, int) const override { return x; }
};
template <class T0, class T1>
struct RangeChmax : public ImplicitTreap<T0, T1> {
    using ImplicitTreap<T0, T1>::ImplicitTreap;
    RangeChmax() : RangeChmax(std::numeric_limits<T0>::min(), std::numeric_limits<T1>::max()) {}
    T0 f0(T0 x, T0 y) const override { return std::max(x, y); }
    T1 f1(T1 x, T1 y) const override { return y == std::numeric_limits<T1>::max() ? x : y; }
    T0 g(T0 x, T1 y) const override { return y == std::numeric_limits<T1>::max() ? x : y; }
    T1 p(T1 x, int) const override { return x; }
};

template <class T0, class T1>
struct RangeAddRangeSum : public ImplicitTreap<T0, T1> {
    using ImplicitTreap<T0, T1>::ImplicitTreap;
    RangeAddRangeSum() : RangeAddRangeSum(0, 0) {}
    T0 f0(T0 x, T0 y) const override { return x + y; }
    T1 f1(T1 x, T1 y) const override { return x + y; }
    T0 g(T0 x, T1 y) const override { return x + y; }
    T1 p(T1 x, int len) const override { return x * len; }
};

template <class T0, class T1>
struct RangeChminAdd : public ImplicitTreap<T0, T1> {
    using ImplicitTreap<T0, T1>::ImplicitTreap;
    RangeChminAdd() : RangeChminAdd(std::numeric_limits<T0>::max(), 0) {}
    T0 f0(T0 x, T0 y) const override { return std::min(x, y); }
    T1 f1(T1 x, T1 y) const override { return x + y; }
    T0 g(T0 x, T1 y) const override { return x + y; }
    T1 p(T1 x, int) const override { return x; }
};
template <class T0, class T1>
struct RangeChmaxAdd : public ImplicitTreap<T0, T1> {
    using ImplicitTreap<T0, T1>::ImplicitTreap;
    RangeChmaxAdd() : RangeChmaxAdd(std::numeric_limits<T0>::min(), 0) {}
    T0 f0(T0 x, T0 y) const override { return std::max(x, y); }
    T1 f1(T1 x, T1 y) const override { return x + y; }
    T0 g(T0 x, T1 y) const override { return x + y; }
    T1 p(T1 x, int) const override { return x; }
};

template <class T0, class T1>
struct RangeSum : public ImplicitTreap<T0, T1> {
    using ImplicitTreap<T0, T1>::ImplicitTreap;
    RangeSum() : RangeSum(0, std::numeric_limits<T1>::min()) {}
    T0 f0(T0 x, T0 y) const override { return x + y; }
    T1 f1(T1 x, T1 y) const override { return y == std::numeric_limits<T1>::min() ? x : y; }
    T0 g(T0 x, T1 y) const override { return y == std::numeric_limits<T1>::min() ? x : y; }
    T1 p(T1 x, int len) const override { return x == std::numeric_limits<T1>::min() ? std::numeric_limits<T1>::min() : x * len; }
};

template <class T0>
struct RangeAffineRangeSum : public ImplicitTreap<T0,std::pair<T0, T0>> {
    using T1 = std::pair<T0, T0>;  // first * x + second
    using ImplicitTreap<T0, T1>::ImplicitTreap;
    RangeAffineRangeSum() : RangeAffineRangeSum(0, {1, 0}) {}
    T0 f0(T0 x, T0 y) const override { return x + y; }
    T1 f1(T1 x, T1 y) const override { return {x.first * y.first, x.second * y.first + y.second}; }
    T0 g(T0 x, T1 y) const override { return y.first * x + y.second; }
    T1 p(T1 x, int len) const override { return {x.first, x.second * len}; }
    // update(i, j, {a, b}); // [i, j)にax + bを作用
    // update(i, j, {0, a}); // update
    // update(i, j, {1, a}); // 加算
    // update(i, j, {a, 0}); // 倍
};

template <class T>
struct RangeMinmaxAffine : public ImplicitTreap<std::pair<T, T>, std::pair<T, T>> {
    using T0 = std::pair<T, T>;  // {std::min, max}
    using T1 = std::pair<T, T>;  // first * x + second
    using ImplicitTreap<T0, T1>::ImplicitTreap;
    RangeMinmaxAffine()
        : RangeMinmaxAffine({std::numeric_limits<T>::max(), -std::numeric_limits<T>::max()}, {1, 0}) {
    }  // TODO: _u1を使うとコンパイル通らない原因不明
    T0 f0(T0 x, T0 y) const override { return {std::min(x.first, y.first), max(x.second, y.second)}; }
    T1 f1(T1 x, T1 y) const override { return {x.first * y.first, x.second * y.first + y.second}; }
    T0 g(T0 x, T1 y) const override {
        T0 ret = {x.first * y.first + y.second, x.second * y.first + y.second};
        if (y.first < 0) swap(ret.first, ret.second);
        return ret;
    }
    T1 p(T1 x, int len) const override { return x; }
    // update(i, j, {a, b}); // [i, j)にax + bを作用
    // update(i, j, {0, a}); // update
    // update(i, j, {1, a}); // 加算
    // update(i, j, {a, 0}); // 倍
};

} // namespace Lib
