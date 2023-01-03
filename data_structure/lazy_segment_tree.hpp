#pragma once


#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "atcoder/internal_bit"



namespace lib {

namespace internal {

namespace lazy_segment_tree_lib {

// Thanks to: atcoder::lazy_segtree
template<
    class S, S (*op)(S, S), S (*e)(),
    class F, S (*map)(F, S), F (*compose)(F, F), F (*id)()
>
struct base {
  private:
    int _n, size, log;
    std::vector<S> d;
    std::vector<F> lz;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
    void all_apply(int k, F f) {
        d[k] = map(f, d[k]);
        if (k < size) lz[k] = compose(f, lz[k]);
    }
    void push(int k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = id();
    }

  public:
    base() : base(0) {}
    explicit base(int n) : base(std::vector<S>(n, e())) {}
    explicit base(const std::vector<S>& v) : _n(int(v.size())) {
        log = internal::ceil_pow2(_n);
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        lz = std::vector<F>(size, id());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    inline void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    inline S get(int p) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }

    inline S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return e();

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        S sml = e(), smr = e();
        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }

        return op(sml, smr);
    }

    inline S all_prod() { return d[1]; }

    inline void apply(int p, F f) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = map(f, d[p]);
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    inline void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return;

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        {
            int l2 = l, r2 = r;
            while (l < r) {
                if (l & 1) all_apply(l++, f);
                if (r & 1) all_apply(--r, f);
                l >>= 1;
                r >>= 1;
            }
            l = l2;
            r = r2;
        }

        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }

    template<bool (*g)(S)> inline int max_right(int l) {
        return max_right(l, [](S x) { return g(x); });
    }
    template<class G> inline int max_right(int l, G g) {
        assert(0 <= l && l <= _n);
        assert(g(e()));
        if (l == _n) return _n;
        l += size;
        for (int i = log; i >= 1; i--) push(l >> i);
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(op(sm, d[l]))) {
                while (l < size) {
                    push(l);
                    l = (2 * l);
                    if (g(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template<bool (*g)(S)> inline int min_left(int r) {
        return min_left(r, [](S x) { return g(x); });
    }
    template<class G> inline int min_left(int r, G g) {
        assert(0 <= r && r <= _n);
        assert(g(e()));
        if (r == 0) return 0;
        r += size;
        for (int i = log; i >= 1; i--) push((r - 1) >> i);
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(op(d[r], sm))) {
                while (r < size) {
                    push(r);
                    r = (2 * r + 1);
                    if (g(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
};


template<class, class = std::void_t<>> struct core {};

template<class Monoid>
struct core<Monoid, std::void_t<typename internal::is_monoid_t<Monoid>>> : base<Monoid> {
  public:
    using monoid = Monoid;

  private:
    using base = typename segment_tree_lib::base<monoid>;

  public:
    using value_type = typename monoid::value_type;
    using size_type = typename base::size_type;

  public:
    explicit core(const size_type n = 0) : base(n) {}

    explicit core(const size_type n, const value_type& v) : base(n) { if(v != 0) REP(i, n) this->base::set(i, v); }

    explicit core(const std::initializer_list<value_type>& init_list) : core(ALL(init_list)) {}

    template<class I, std::enable_if_t<std::is_same_v<value_type, typename std::iterator_traits<I>::value_type>>* = nullptr>
    explicit core(const I first, const I last) : core(std::distance(first, last)) {
        size_type pos = 0;
        for(auto itr=first; itr!=last; ++itr, ++pos) this->base::set(pos, *itr);
    }

    inline void set(const size_type p, const value_type& x) {
        dev_assert(0 <= p and p < this->size());
        this->base::set(p, x);
    }

    inline value_type get(const size_type p) const {
        dev_assert(0 <= p and p < this->size());
        return this->base::prod(p, p+1).val();
    }
    inline value_type operator[](const size_type pos) const { return this->get(pos); }

    inline value_type prod(const size_type l, const size_type r) const {
        dev_assert(0 <= l and l <= r and r <= this->size());
        return this->base::prod(l, r).val();
    }
    inline value_type prod(const size_type r) const {
        dev_assert(0 <= r and r <= this->size());
        return this->base::prod(0, r).val();
    }
    inline value_type prod() const {
        return this->base::all_prod().val();
    }


    struct iterator : virtual internal::container_iterator_interface<value_type,core> {
        iterator(const core *const ref, const size_type pos) : internal::container_iterator_interface<value_type,core>(ref, pos) {}

        inline value_type operator*() const override { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};

template<class Action>
struct core<Action, std::void_t<typename internal::is_action_t<Action>>> : core<typename Action::operand_monoid> {
    using action = Action;
    using core<typename action::operand_monoid>::core;
    static_assert(action::tags.has(actions::flags::segment_tree));
};


} // namespace lazy_segment_tree_lib

} // namespace internal


template<class monoid> struct segment_tree : internal::segment_tree_lib::core<monoid> {
    using internal::segment_tree_lib::core<monoid>::core;
};


} // namespace lib
