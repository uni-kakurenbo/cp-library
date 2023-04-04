#include <bits/stdc++.h>
#include "template.hpp"

#include "data_structure/fenwick_tree.hpp"
#include "algebraic/addition.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<int> a(n); REP(i, n) std::cin >> a[i];
    lib::fenwick_tree<lib::algebraic::addition<i64>> sum(ALL(a));

    REP(q) {
        int l, r; std::cin >> l >> r;
        print(sum(l, r).fold());
    }
}

