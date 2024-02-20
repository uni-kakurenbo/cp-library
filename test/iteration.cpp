/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
#include <bits/stdc++.h>

#include "template/standard.hpp"
/* #endregion */

void solve();

signed main() {
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        debug_("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

void solve() {

REP(30) debug("a");
REP(i, 30) debug(i);
REP(i, 5, 30) debug(i);
REP(i, 5, 29, 3UL) debug(i);

REPD(i, 30) debug(i);
REPD(i, 5, 30) debug(i);
REPD(i, 5, 29, 3UL) debug(i);


FOR(i, 30) debug(i);
FOR(i, 5, 30) debug(i);
FOR(i, 5, 29, 3UL) debug(i);

FORD(i, 30) debug(i);
FORD(i, 5, 30) debug(i);
FORD(i, 5, 29, 3UL) debug(i);


vector<int> a = { 1, 2, 3 };
REP(i, ALL(a)) debug(*i);

}
