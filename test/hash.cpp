#include <bits/stdc++.h>
#include "template.hpp"
#include "hash.hpp"

// https://atcoder.jp/contests/abc141/tasks/abc141_e

signed main() {
    int n; std::cin >> n;
    std::string s; std::cin >> s;

    std::string str0 = "ABC", str1 = "DEF", str2 = "ABCDEF";
    debug(lib::hasher(ALL(str0)).concat_front(ALL(str1)).get());
    debug(lib::hasher(ALL(str2)).get());

    int ans = 0;
    lib::hasher hash(ALL(s));
    debug(lib::hasher<>::base);
    REP(i, s.size()) REP(j, i, (int)s.size()) {
        while(i + ans < j and j + ans < n) {
            if(hash(i, i+ans+1) != hash(j, j+ans+1)) break;
            ++ans;
        }
    }

    printf("%d\n", ans);
    return 0;
}
