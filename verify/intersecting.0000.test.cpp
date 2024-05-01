/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_2_B"

#include "snippet/fast_io.hpp"
#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "geometry/point.hpp"
#include "geometry/segment.hpp"

using point = uni::point<uni::ld>;

signed main() {
    uni::i32 q; input >> q;
    REP(q) {
        uni::segment<point> s0, s1; input >> s0 >> s1;
        print(uni::intersecting(s0, s1));
    }
}
