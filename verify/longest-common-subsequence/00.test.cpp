#include <bits/stdc++.h>
#include "template.hpp"
#include "iterable/longest_common_subsequence.hpp"

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_10_C"

signed main() {
    int q; std::cin >> q;
    REP(q) {
        string x, y; std::cin >> x >> y;
        print(lib::lcs_sizes(ALL(x), ALL(y)).back().back());
    }
}
