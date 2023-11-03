#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum"

#include <bits/stdc++.h>
#include "template.hpp"
#include "iterable/accumulation.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<int> a(n); input >> a;
    lib::accumulation<ll> sum(ALL(a));
    debug(sum);

    REP(q) {
        int l, r; std::cin >> l >> r;
        print(sum(l, r));
    }
}
