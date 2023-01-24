/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
#include <bits/stdc++.h>
using namespace std;

#include "template.hpp"
/* #endregion */

#include "data_structure/wavelet_matrix.hpp"

signed main() {
    int n, m, k; cin >> n >> m >> k;
    vector<int> a(n), b(m); input >> a >> b;

    lib::wavelet_matrix<int>::compressed wm(ALL(a));

    ITR(p, b) {
        auto itr = wm.next_element(p);

        int s = itr == wm.end() ? 0 : k - abs(p - *itr);
        int t = itr == wm.begin() ? 0 : k - abs(p - *prev(itr));

        print(max({ 0, s, t }));
    }

    return 0;
}
