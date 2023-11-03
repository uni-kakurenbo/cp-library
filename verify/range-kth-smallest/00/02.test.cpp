#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"

#include <bits/stdc++.h>
#include "template.hpp"
#include "data_structure/wavelet_matrix.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<u32> a(n); input >> a;

    lib::wavelet_matrix<u32>::compressed data(a);
    debug(data);

    REP(q) {
        int l, r, k; std::cin >> l >> r >> k;
        print(data.range(l, r).kth_smallest(k));
    }
}
