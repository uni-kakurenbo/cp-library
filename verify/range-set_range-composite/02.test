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

#define PROBLEM "https://atcoder.jp/contests/abl/tasks/abl_e"
#define IGNORE

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

#include "data_structure/lazy_segment_tree.hpp"
#include "data_structure/range_action/helper.hpp"

using mint = lib::modint998244353;
valarray<mint> ten, one;

struct S {
    mint sum = 0;
    int len = 0;
    S() {}
    S(mint sum, int len = 1) : sum(sum), len(len) {}
};

S op(S a, S b) {
    return { a.sum * ten[b.len] + b.sum, a.len + b.len };
}
S e() { return S{}; }

using F = int;

S mapping(F f, S s) {
  if(f == 0) return s;
  return { f * one[s.len], s.len };
}

F composition(F f, F g) {
  if(f == 0) return g;
  return f;
}

F id(){ return 0; }


void solve() {
    int n, q; cin >> n >> q;

    ten.resize(n + 1), one.resize(n + 1);
    ten[0] = 1, one[0] = 0;
    REP(i, n) {
        ten[i+1] = ten[i] * 10;
        one[i+1] = one[i] + ten[i];
    }

    using action = lib::actions::helper<S,op,e,F,mapping,composition,id>;
    lib::lazy_segment_tree<action> data(n, S{1});

    REP(q) {
        int l, r, d; cin >> l >> r >> d; --l;
        data(l, r) += d;
        print(data.fold(0, n)->sum);
        // debug(data(0, 2).fold());
    }
}
