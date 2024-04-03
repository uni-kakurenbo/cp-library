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
#include "data_structure/red_black_tree.hpp"
#include "random/engine.hpp"
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "utility/timer.hpp"

signed main() {
    print("Hello World");

    lib::dynamic_sequence<lib::actions::make_full_t<lib::i64>, lib::red_black_tree_context<>> data;
    lib::map<lib::i64, lib::i64> corr;

    debug(data);

    lib::timer timer(10000);

    while(not timer.expired()) {
        lib::i64 i = lib::randi64();
        lib::i64 v = lib::randi64();
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
