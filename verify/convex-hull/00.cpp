/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
// #include <bits/stdc++.h>

#include "template.hpp"
/* #endregion */

#include "geometry/convex_full.hpp"

signed main() {
    using point = lib::point<i64>;

    int n; cin >> n;
    vector<point> poly(n+2); input >> poly;
    auto s = poly.end()[-2], t = poly.end()[-1];
    debug(poly);

    auto hull = lib::convex_hull(ALL(poly));
    int m = (int)hull.size();
    debug(m, hull);

    int p = -1, q = -1;
    REP(i, m) {
        if(hull[i] == s) p = i;
        if(hull[i] == t) q = i;
    }
    debug(p, q);

    if(p == -1 or q == -1) {
        print(lib::distance<ld>(s, t));
        return 0;
    }

    ld d0 = 0, d1 = 0;
    for(int i=p; i!=q; i=(i+1)%m) {
        d0 += lib::distance<ld>(hull[i], hull[(i+1)%m]);
    }
    for(int i=q; i!=p; i=(i+1)%m) {
        d1 += lib::distance<ld>(hull[i], hull[(i+1)%m]);
    }

    print(std::min(d0, d1));

    return 0;
}
