#include <bits/stdc++.h>
#include "template.hpp"
#include "data_structure/wavelet_matrix.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<unsigned> a(n); input >> a;

    lib::wavelet_matrix<unsigned>::compressed data(a);
    debug(data);

    REP(q) {
        int l, r, x; std::cin >> l >> r >> x;
        print(data(l, r).count(x));
    }
}
