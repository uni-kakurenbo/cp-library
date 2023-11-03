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

#define PROBLEM "https://atcoder.jp/contests/abc223/tasks/abc223_f"

#include "data_structure/segment_tree.hpp"

template<class T>
struct monoid : lib::algebraic::base<spair<T>>, lib::algebraic::associative/*, lib::algebraic::invertible, lib::algebraic::commutative*/ {
    using base = lib::algebraic::base<spair<T>>;
    using base::base;
    // monoid() : base() {}
    monoid(const T& v) : base({ v, v }) {}
    friend inline monoid operator+(const monoid& lhs, const monoid& rhs) noexcept(NO_EXCEPT) {
        return monoid({ lib::min(lhs->first, lhs->second + rhs->first), lhs->second + rhs->second });
    }
    // inline bitxor operator-() const noexcept(NO_EXCEPT) { return -this->val(); }
};

void solve() {
    int n, q; cin >> n >> q;
    string s; cin >> s;

    lib::segment_tree<monoid<int>> data(n);
    REP(i, n) data[i] = s[i] == '(' ? 1 : -1;

    REP(q) {
        int t, l, r; cin >> t >> l >> r;
        if(t == 1) {
            --l, --r;
            auto tmp = data[l].val();
            data[l] = data[r].val(), data[r] = tmp;
        }
        if(t == 2) {
            --l;
            auto [ pm, sm ] = data(l, r).fold().val();
            print(pm >= 0 and sm == 0 ? "Yes" : "No");
        }
        debug(data);
    }
}
