/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
#include <bits/stdc++.h>

#include "template.hpp"
/* #endregion */

void solve();

signed main() {
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        DEBUG("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

void solve() {

    const lib::string ABRACADABRA = "ABRACADABRA";
    debug(lib::z_array(std::begin(ABRACADABRA), std::end(ABRACADABRA)));
    // lib::vector<lib::internal::size_t> { 11, 0, 0, 1, 0, 1, 0, 4, 0, 0, 1 }

    const lib::string DIJKSTRA = "Program testing can be a very effective way to show the presence of bugs, but is hopelessly inadequate for showing their absence.";

    debug(lib::find<lib::string>(DIJKSTRA, "ce"));
    // lib::vector<lib::string::iterator> { begin + 62, begin + 126 }

    debug(lib::replace<lib::string,lib::replacement_policy::insert_sync>(DIJKSTRA, "e", "E__"));
    // Program tE__sting can bE__ a vE__ry E__ffE__ctivE__ way to show thE__ prE__sE__ncE__ of bugs, but is hopE__lE__ssly inadE__quatE__ for showing thE__ir absE__ncE__.

    debug(lib::replace<lib::string,lib::replacement_policy::overwrite_sync>(DIJKSTRA, "e", "E__"));
    // Program tE__ing can bE__ vE__ E__E__ivE__ay to show thE__rE__ncE__f bugs, but is hopE__ssly inadE__atE__or showing thE__ absE__E_`

    debug(lib::replace<lib::string,lib::replacement_policy::overwrite_async>(DIJKSTRA, "e", "E__"));
    // Program tE__ing can bE__ vE__ E__E__ivE__ay to show thE__rE_E__E__f bugs, but is hopE_E__ly inadE__atE__or showing thE__ absE__E_

}
