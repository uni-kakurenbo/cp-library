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

#include "internal/endian.hpp"

void solve() {
    {
        const auto endian = lib::internal::discern_endian();
        if(endian == lib::internal::endian::big) {
            print("Big-endian");
        }
        else if(endian == lib::internal::endian::little) {
            print("Little-endian");
        }
        else print("Unknown");
    }

    {
        const auto endian = lib::internal::discern_endian();
        if(endian == lib::internal::endian::big) {
            print("Big-endian");
        }
        else if(endian == lib::internal::endian::little) {
            print("Little-endian");
        }
        else print("Unknown");
    }
}
