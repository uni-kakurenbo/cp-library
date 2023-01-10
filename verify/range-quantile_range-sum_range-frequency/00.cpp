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
        auto range = data.subseq(i, k);
        debug(range);

        ll med = range.median();

        ll costl = med * range.count_under(med) - range.sum_under(med);
        ll costr = range.sum_over_or(med) - med * range.count_over_or(med);

        chmin(ans, costl + costr);
    }

    print(ans);
}
