#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_H"

#include <bits/stdc++.h>
#include "template.hpp"

#include "data_structure/implicit_treap.hpp"
#include "data_structure/range_action/range_affine_range_minmax.hpp"

signed main() {
    int n, q; std::cin >> n >> q;

    lib::implicit_treap<lib::actions::range_affine_range_minmax<ll>> data(n, spair<ll>{ 0, 0 });

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, x; std::cin >> l >> r >> x; ++r;
            data(l, r) += { 1, x };
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r; ++r;
            print(data(l, r).fold()->first);
        }
        // debug(data);
    }
}
