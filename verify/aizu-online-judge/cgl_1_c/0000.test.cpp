/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_1_C"

#include "snippet/fast_io.hpp"
#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "geometry/point.hpp"
#include "geometry/arrow.hpp"

using point = uni::point<uni::ld>;

signed main() {
    uni::arrow<point> r; input >> r;
    debug(r);
    uni::i32 q; input >> q;
    REP(q) {
        point p; input >> p;

        const auto relation = r.relation(p);
        switch (relation) {
            case uni::positional_relation::clockwise: print("CLOCKWISE"); break;
            case uni::positional_relation::counter_clockwise: print("COUNTER_CLOCKWISE"); break;
            case uni::positional_relation::backward: print("ONLINE_BACK"); break;
            case uni::positional_relation::forward: print("ONLINE_FRONT"); break;
            case uni::positional_relation::on:
            case uni::positional_relation::in:
                print("ON_SEGMENT"); break;
            default: break;
        }
    }
}
