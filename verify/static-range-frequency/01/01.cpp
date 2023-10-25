#include <bits/stdc++.h>
#include "template.hpp"
#include "data_structure/wavelet_matrix.hpp"
#include "constants.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n); input >> a;

    lib::compressed_wavelet_matrix data(a);
    debug(data);

    int q; std::cin >> q;
    REP(q) {
        int l, r, x; std::cin >> l >> r >> x; --l, --r;
        print(data.range<lib::interval_notation::closed>(l, r).count(x));
    }
}
