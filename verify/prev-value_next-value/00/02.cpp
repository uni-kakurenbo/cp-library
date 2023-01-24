#include <bits/stdc++.h>
#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "data_structure/wavelet_matrix.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n); ITRR(v, a) std::cin >> v;

    lib::wavelet_matrix<ll> data(ALL(a));
    // debug(data);

    int q; std::cin >> q;
    REP(q) {
        int l, r, x; std::cin >> l >> r >> x; ++r;

        auto itr = data(l, r).lower_bound(x);

        std::cout << (
            std::min(
                std::abs((itr == data(l, r).end() ? INT64_MAX : *itr) - x),
                std::abs((itr == data(l, r).begin() ? INT64_MAX : *std::prev(itr)) - x)
            )
        ) << "\n";
    }
}
