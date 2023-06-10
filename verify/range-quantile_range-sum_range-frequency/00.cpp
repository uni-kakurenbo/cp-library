// #include <bits/stdc++.h>
// #include "template.hpp"
#include <iostream>
#include "adapter/io.hpp"
#include "utility/functional.hpp"
#include "snippet/iterations.hpp"
#include "data_structure/wavelet_matrix.hpp"

signed main() {
    int n, k; input >> n >> k;
    std::vector<long> a(n); input >> a;
    lib::wavelet_matrix<long> data(ALL(a));

    long ans = lib::INF64;
    FOR(i, 0, n-k) {
        auto range = data.subseq(i, k);
        // debug(range);

        long med = range.median();

        long costl = med * range.count_under(med) - range.sum_under(med);
        long costr = range.sum_or_over(med) - med * range.count_or_over(med);

        lib::chmin(ans, costl + costr);
    }

    print(ans);
}
