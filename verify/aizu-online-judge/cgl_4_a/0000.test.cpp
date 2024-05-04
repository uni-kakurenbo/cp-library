/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_4_A"

#include "snippet/fast_io.hpp"
#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "geometry/point.hpp"
#include "geometry/polygon.hpp"
#include "geometry/convex_hull.hpp"
#include "view/cyclic.hpp"

using point = uni::point<uni::i32>;

signed main() {
    uni::i32 n; input >> n;
    uni::polygon<point> poly(n); input >> poly;
    const auto ch = poly.convex_hull();
    const auto view = ch | uni::views::cyclic;

    print(ch.size());

    const auto min = std::ranges::min(
        ch,
        [](auto& p0, auto& p1) { return std::make_pair(p0.y(), p0.x()) < std::make_pair(p1.y(), p1.x()); }
    );

    auto itr = view.begin();

    while(*itr != min) ++itr;

    REP(ch.size()) print(*(itr++));
}
