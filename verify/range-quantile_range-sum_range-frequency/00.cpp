#include <bits/stdc++.h>
#include "template.hpp"

#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "data_structure/wavelet_matrix.hpp"


signed main() {
    int n, k; std::cin >> n >> k;
    std::vector<ll> a(n); ITRR(v, a) std::cin >> v;
    lib::wavelet_matrix<ll> data(ALL(a));

    ll ans = LONG_LONG_MAX;
    FOR(i, 0, n-k) {
        int l = i, r = i + k;

        auto range = data.range(l, r);

        ll med = range.median();

        ll less_cost = med * range.count_under(med) - range.sum_under(med);
        ll over_or_cost = range.sum_over_or(med) - med * range.count_over_or(med);

        chmin(ans, less_cost + over_or_cost);
    }

    print(ans);
}
