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

        ll med = data.kth_smallest(l, r, k/2);

        ll less_cost = med * data.count<lib::conditions::less_than>(l, r, med) - data.sum(l, r, 0, med);
        ll or_over_cost = data.sum(l, r, med, INT_MAX) - med * data.count<lib::conditions::geq>(l, r, med);

        chmin(ans, less_cost + or_over_cost);
    }

    print(ans);
}
