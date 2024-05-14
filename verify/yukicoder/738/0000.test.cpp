/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://yukicoder.me/problems/no/738"


#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "adaptor/valarray.hpp"
#include "data_structure/wavelet_matrix.hpp"

signed main() {
    uni::i32 n, k; input >> n >> k;
    uni::valarray<uni::i32> a(n); input >> a;
    uni::wavelet_matrix<uni::i64> data(a);

    uni::i64 ans = uni::INF64;

    FOR(i, 0, n-k) {
        auto range = data.subseq(i, k);
        // debug(range);

        uni::i64 med = range.median();

        uni::i64 costl = med * range.count_under(med) - range.sum_under(med);
        uni::i64 costr = range.sum_or_over(med) - med * range.count_or_over(med);

        uni::chmin(ans, costl + costr);
    }

    print(ans);
}
