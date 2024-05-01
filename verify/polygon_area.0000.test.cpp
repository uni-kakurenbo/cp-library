/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_3_A"
#define ERROR 1e-20

#include "snippet/fast_io.hpp"
#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "geometry/point.hpp"
#include "geometry/polygon.hpp"

using point = uni::point<uni::ld>;

signed main() {
    uni::i32 n; input >> n;
    uni::polygon<point> poly(n); input >> poly;
    print(poly.area());
}
