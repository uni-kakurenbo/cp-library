#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_E"

#include <bits/stdc++.h>

#include "snippet/iterations.hpp"
#include "snippet/aliases.hpp"

#include "data_structure/fenwick_tree.hpp"
#include "data_structure/range_action/range_add.hpp"

#include "template.hpp"

signed main() {
    int n, q; std::cin >> n >> q;

    lib::fenwick_tree<lib::actions::range_add<ll>> data(n);
    debug(data);

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, x; std::cin >> l >> r >> x; --l;
            data(l, r) += x;
        }
        if(t == 1) {
            int p; std::cin >> p; --p;
            print(data[p]);
        }
        debug(data);
    }

    return 0;
}
