/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_C"
#define ERROR 1e-8

#include "snippet/fast_io.hpp"
#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "geometry/point.hpp"
#include "geometry/triangle.hpp"


using point = uni::point<uni::ld>;

signed main() {
    point p0, p1, p2; input >> p0 >> p1 >> p2;
    uni::triangle<point> triangle(p0, p1, p2);
    const auto circumcircle = triangle.circumcircle();
    print(circumcircle.center(), circumcircle.radius());
}
