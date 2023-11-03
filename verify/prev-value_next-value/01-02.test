/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
#include "template.hpp"
/* #endregion */

#define PROBLEM "https://atcoder.jp/contests/joig2023-open/tasks/joig2023_c"
#define IGNORE

#include "data_structure/wavelet_matrix.hpp"

signed main() {
    int n, m, k; cin >> n >> m >> k;
    vector<int> a(n), b(m); input >> a >> b;

    lib::wavelet_matrix<int> wm(ALL(a));

    ITR(p, b) {
        auto itr = wm.next_element(p);

        int s = itr == wm.end() ? 0 : k - abs(p - *itr);
        int t = itr == wm.begin() ? 0 : k - abs(p - *std::prev(itr));

        print(std::max({ 0, s, t }));
    }

    return 0;
}
