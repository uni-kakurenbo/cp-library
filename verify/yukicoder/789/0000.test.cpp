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
#include "data_structure/dynamic_segment_tree.hpp"
#include "action/range_sum.hpp"

signed main() {
    uni::i32 n; input >> n;

    uni::dynamic_segment_tree<uni::actions::range_sum<uni::i64>> sum(1'000'000'001);

    uni::i64 ans = 0;

    REP(n) {
        uni::i32 t; input >> t;
        if(t == 0) {
            uni::i32 i, x; input >> i >> x;
            sum[i] += x;
        }
        if(t == 1) {
            uni::i32 l, r; input >> l >> r; ++r;
            ans += sum(l, r).fold().val();
            debug(ans);
        }

        debug(ans);
        debug(sum.dump_rich());
    }

    print(ans);
}
