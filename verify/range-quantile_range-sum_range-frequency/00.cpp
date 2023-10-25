#include <iostream>
#include "utility/functional.hpp"
#include "snippet/iterations.hpp"
#include "data_structure/wavelet_matrix.hpp"

signed main() {
    int n, k; std::cin >> n >> k;
    std::vector<unsigned> a(n); for(auto&& v : a) std::cin >> v;
    lib::wavelet_matrix data(a);

    long ans = lib::INF64;
    FOR(i, 0, n-k) {
        auto range = data.subseq(i, k);
        // debug(range);

        long med = range.median();

        long costl = med * range.count_under(med) - range.sum_under(med);
        long costr = range.sum_or_over(med) - med * range.count_or_over(med);

        lib::chmin(ans, costl + costr);
    }

    std::cout << ans << "\n";
}
