/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>

#include "action/null.hpp"
#include "adaptor/io.hpp"
#include "adaptor/map.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "data_structure/treap.hpp"
#include "random/engine.hpp"
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "utility/timer.hpp"

signed main() {
    print("Hello World");

    uni::dynamic_sequence<uni::actions::make_full_t<uni::i64>> data;
    uni::map<uni::i64, uni::i64> corr;

    debug(data);

    uni::timer timer(10000);

    while(!timer.expired()) {
        uni::i64 i = uni::randi64();
        uni::i64 v = uni::randi64();
        i /= 2;
        debug(i, v);

        corr[i] = v;
        data[i] = v;

        debug(corr, data);

        bool ok = true;
        ITR(i, v, corr) {
            ok &= data[i].val() == v;
        }
        if(!ok) {
            assert(false);
        }
    }
}
