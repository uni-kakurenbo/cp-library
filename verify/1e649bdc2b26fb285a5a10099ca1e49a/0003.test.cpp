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
#include "adapter/io.hpp"
#include "adapter/map.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "data_structure/treap.hpp"
#include "action/null.hpp"
#include "random/xorshift.hpp"
#include "utility/timer.hpp"

signed main() {
    print("Hello World");

    lib::dynamic_sequence<lib::i64> data;
    lib::map<lib::i64, lib::i64> corr;

    debug(data);

    lib::timer timer(1000);

    while(not timer.expired()) {
        lib::i64 i = lib::rand64();
        lib::i64 v = lib::rand64();
        i /= 2;
        debug(i, v);

        corr[i] = v;
        data[i] = v;

        bool ok = true;
        ITR(i, v, corr) {
            ok &= data[i].val() == v;
        }
        debug(corr, data);
        if(!ok) {
            assert(false);
        }
    }
}
