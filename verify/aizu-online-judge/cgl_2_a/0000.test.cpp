/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_2_A"

#include "snippet/fast_io.hpp"
#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "geometry/point.hpp"
#include "geometry/line.hpp"

using point = uni::point<uni::ld>;

signed main() {
    uni::i32 q; input >> q;
    REP(q) {
        uni::line<point> l0, l1; input >> l0 >> l1;
        if(uni::parallel(l0, l1)) print(1);
        else if(uni::orthogonal(l0, l1)) print(2);
        else print(0);
    }
}
