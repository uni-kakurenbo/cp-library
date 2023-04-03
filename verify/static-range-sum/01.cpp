#include <bits/stdc++.h>
#include "template.hpp"

#include "data_structure/disjoint_sparse_table.hpp"
#include "data_structure/monoid/sum.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<int> a(n); REP(i, n) std::cin >> a[i];
    lib::disjoint_sparse_table<lib::monoids::sum<i64>> sum(ALL(a));

    REP(q) {
        int l, r; std::cin >> l >> r;
        print(sum(l, r).fold());
    }
}
