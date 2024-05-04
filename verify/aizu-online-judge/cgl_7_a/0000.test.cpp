/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_A"

#include "snippet/fast_io.hpp"
#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "geometry/circle.hpp"
#include "geometry/point.hpp"


using point = uni::point<uni::ld>;

signed main() {
    uni::circle<point> c0, c1;

    {
        point c; uni::ld r; input >> c >> r;
        c0 = { c, r };
    }
    {
        point c; uni::ld r; input >> c >> r;
        c1 = { c, r };
    }

    print(uni::count_common_tangent(c0, c1));
}
