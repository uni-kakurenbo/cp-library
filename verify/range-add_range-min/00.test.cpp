#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_H"

#include <bits/stdc++.h>
#include "template.hpp"

#include "data_structure/lazy_segment_tree.hpp"
#include "data_structure/range_action/range_add_range_min.hpp"

signed main() {
    int n, q; std::cin >> n >> q;

    lib::lazy_segment_tree<lib::actions::range_add_range_min<ll>> data(n, 0);
    // debug(data);

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, x; std::cin >> l >> r >> x; ++r;
            data(l, r) += x;
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r; ++r;
            print(data(l, r).fold());
        }
        // debug(data);
    }
}
