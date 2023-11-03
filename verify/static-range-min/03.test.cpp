#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include <bits/stdc++.h>
#include "template.hpp"

#include "data_structure/segment_tree.hpp"
#include "data_structure/range_action/range_max.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    valarray<int> a(n); input >> a;
    a *= -1;
    lib::segment_tree<lib::actions::range_max<int>> max(a);

    REP(q) {
        int l, r; std::cin >> l >> r;
        print(-max(l, r).fold().val());
    }
}
