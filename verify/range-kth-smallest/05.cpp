#include <bits/stdc++.h>
#include "template.hpp"
#include "iterable/compression.hpp"
#include "data_structure/adapter/set.hpp"
#include "data_structure/fenwick_tree.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<int> a(n); input >> a;
    lib::compression<int> comp(ALL(a));
    lib::multiset_adapter<lib::fenwick_tree> set;
    ITR(v, comp) set.insert(v);

    REP(q) {

    }
}
