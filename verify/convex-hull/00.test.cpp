/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://atcoder.jp/contests/abc286/tasks/abc286_ex"

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "adapter/vector.hpp"
#include "geometry/point.hpp"
#include "geometry/convex_full.hpp"

signed main() {
    using point = lib::point<lib::i64>;

    int n; std::cin >> n;
    lib::vector<point> poly(n+2); input >> poly;
    auto s = poly.end()[-2], t = poly.end()[-1];

    auto hull = lib::convex_hull(ALL(poly));
    int m = (int)hull.size();

    int p = -1, q = -1;
    REP(i, m) {
        if(hull[i] == s) p = i;
        if(hull[i] == t) q = i;
    }

    if(p == -1 or q == -1) {
        print(lib::distance<lib::ld>(s, t));
        return 0;
    }

    lib::ld d0 = 0, d1 = 0;
    for(int i=p; i!=q; i=(i+1)%m) {
        d0 += lib::distance<lib::ld>(hull[i], hull[(i+1)%m]);
    }
    for(int i=q; i!=p; i=(i+1)%m) {
        d1 += lib::distance<lib::ld>(hull[i], hull[(i+1)%m]);
    }

    print(std::min(d0, d1));

    return 0;
}
