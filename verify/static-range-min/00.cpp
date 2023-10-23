// #include <bits/stdc++.h>
#include "template.hpp"

#include "data_structure/disjoint_sparse_table.hpp"
#include "algebraic/minimum.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<int> a(n); REP(i, n) std::cin >> a[i];
    lib::disjoint_sparse_table<lib::algebraic::minimum<int>> min(a);

    REP(q) {
        int l, r; std::cin >> l >> r;
        print(min(l, r).fold());
    }
}
