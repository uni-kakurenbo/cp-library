#include <bits/stdc++.h>
#include "template.hpp"

#include "data_structure/segment_tree.hpp"
#include "data_structure/range_action/range_min.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<int> a(n); REP(i, n) std::cin >> a[i];
    lib::segment_tree<lib::actions::range_min<int>> min(ALL(a));

    REP(q) {
        int l, r; std::cin >> l >> r;
        print(min(l, r).fold());
    }
}
