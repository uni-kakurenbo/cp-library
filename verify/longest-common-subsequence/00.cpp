#include <bits/stdc++.h>
// #include "template.hpp"
#include "iterable/longest_common_subsequence.hpp"

signed main() {
    int q; std::cin >> q;
    REP(q) {
        std::string x, y; std::cin >> x >> y;
        std::cout << lib::lcs_sizes(ALL(x), ALL(y)).back().back() << "\n";
    }
}
