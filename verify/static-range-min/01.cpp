// #include <bits/stdc++.h>
#include "template.hpp"

#include "data_structure/disjoint_sparse_table.hpp"
#include "data_structure/range_action/range_min.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<int> a(n); REP(i, n) std::cin >> a[i];
    lib::disjoint_sparse_table<lib::actions::range_min<int>> min(a);
    static_assert(lib::actions::internal::action<lib::actions::range_min<int>>);

    REP(q) {
        int l, r; std::cin >> l >> r;
        print(min(l, r).fold());
    }
}
