/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1549"

#include <bits/stdc++.h>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/valarray.hpp"
#include "data_structure/wavelet_matrix.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n); ITRR(v, a) std::cin >> v;

    lib::wavelet_matrix<lib::ll> data(a);
    // debug(data);

    int q; std::cin >> q;
    REP(q) {
        int l, r, x; std::cin >> l >> r >> x; ++r;

        auto prv = data(l, r).prev_element(x);
        auto nxt = data(l, r).next_element(x);
        // if(prv != data(l, r).end()) assert(*prv == data(l, r).prev(x));
        // if(nxt != data(l, r).end()) assert(*nxt == data(l, r).next(x));

        std::cout << (
            std::min(
                std::abs((prv == data(l, r).end() ? INT64_MAX : *prv) - x),
                std::abs((nxt == data(l, r).end() ? INT64_MAX : *nxt) - x)
            )
        ) << "\n";
    }
}