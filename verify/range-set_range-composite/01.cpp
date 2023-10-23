/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */
/* #region template */
#include <bits/stdc++.h>

#include "template.hpp"
/* #endregion */

void solve();

signed main() {
    debug(__cplusplus);
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        DEBUG("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

#include "data_structure/implicit_treap.hpp"
#include "data_structure/range_action/range_set_range_sum.hpp"

using mint = lib::modint998244353;
valarray<mint> ten, one;

template<class T>
struct monoid : lib::algebraic::base<pair<T,int>>, lib::algebraic::associative/*, internal::group, internal::commutative*/ {
    using base = lib::algebraic::base<pair<T,int>>;
    using base::base;
    monoid() : base({ 0, 0 }) {}
    monoid(const T& v) : base({ v, 1 }) {}
    friend inline monoid operator+(const monoid& lhs, const monoid& rhs) noexcept(NO_EXCEPT) {
        // debug(lhs, rhs);
        return monoid({
            lhs->first * ten[rhs->second] + rhs->first,
            lhs->second + rhs->second
        });
    }
    // inline bitxor operator-() const noexcept(NO_EXCEPT) { return -this->val(); }
};

template<class T> struct action : lib::actions::base<lib::algebraic::assignment<T>> {
    static constexpr lib::actions::flags tags{};

    using operand = monoid<T>;
    using operation = lib::algebraic::assignment<T>;

    static operand map(const operand& x, const operation& y) noexcept(NO_EXCEPT) {
        if(y->has_value()) return operand({ y->value() * one[x->second], x->second });
        return x;
    }
};


void solve() {
    int n, q; cin >> n >> q;

    ten.resize(n + 1), one.resize(n + 1);
    ten[0] = 1, one[0] = 0;
    REP(i, n) {
        ten[i+1] = ten[i] * 10;
        one[i+1] = one[i] + ten[i];
    }

    lib::implicit_treap<action<mint>> data(n, mint{1});

    valarray<monoid<mint>> a(n, mint{1});
    REP(q) {
        int l, r, d; cin >> l >> r >> d; --l;
        data(l, r) += d;
        print(data.fold(0, n)->first);
        // debug(data(0, 2).fold());
    }
}
