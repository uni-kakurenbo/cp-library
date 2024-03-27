/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://yukicoder.me/problems/no/789"


#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/persistent_dynamic_segment_tree.hpp"
#include "action/range_sum.hpp"

signed main() {
    int n; input >> n;

    lib::persistent_dynamic_segment_tree<lib::actions::range_sum<lib::i64>> sum(1'000'000'001);

    lib::i64 ans = 0;

    REP(n) {
        int t; input >> t;
        if(t == 0) {
            int i, x; input >> i >> x;
            sum[i] += x;
        }
        if(t == 1) {
            int l, r; input >> l >> r; ++r;
            ans += sum(l, r).fold().val();
            debug(ans);
        }

        debug(ans);
        debug(sum.dump_rich());
    }

    print(ans);
}
