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
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "adaptor/set.hpp"
#include "data_structure/dynamic_set.hpp"
#include "data_structure/treap.hpp"
#include "action/null.hpp"
#include "random/engine.hpp"
#include "utility/timer.hpp"

signed main() {
    print("Hello World");

    uni::dynamic_set<uni::i64> data;
    uni::set<uni::i64> corr;

    debug(data);

    uni::timer timer(10000);

    while(!timer.expired()) {
        uni::i32 t = uni::randi64() % 2;
        uni::i64 v = (uni::randi64() % 2) || data.empty() ? uni::randi64() : data[uni::randi64() % data.size()].val();
        debug(t, v);

        if(t == 0) {
            corr.insert(v);
            data.insert<true>(v);
        }

        if(t == 1) {
            corr.erase(v);
            data.erase(v);
        }

        debug(corr, data);

        bool ok = true;
        {
            uni::i32 i = 0;
            ITR(v, corr) {
                ok &= data[i++].val() == v;
            }
        }
        if(!ok) {
            assert(false);
        }
    }
}
