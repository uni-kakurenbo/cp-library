/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://yukicoder.me/problems/no/649"

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/kth_element.hpp"


signed main() {
    uni::i32 q, k; input >> q >> k, --k;

    uni::kth_element<uni::i64> data(k);

    REP(q) {
        uni::i32 t; input >> t;
        if(t == 1) {
            uni::i64 v; input >> v;
            data.push(v);
        }
        if(t == 2) {
            if(data.has()) {
                print(data.value());
                data.pop();
            }
            else {
                print(-1);
            }
        }
    }
}
