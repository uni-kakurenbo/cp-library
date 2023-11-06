/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://atcoder.jp/contests/abc141/tasks/abc141_e"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "adapter/string.hpp"
#include "hash/sequence_hasher.hpp"

signed main() {
    int n; std::cin >> n;
    lib::string s; std::cin >> s;

    lib::sequence_hasher hash(ALL(s));

    int ans = 0;
    REP(i, s.size()) REP(j, i, (int)s.size()) {
        while(i + ans < j and j + ans < n) {
            if(hash.subseq(i, ans+1) != hash.subseq(j, ans+1)) break;
            ++ans;
        }
    }

    print(ans);

    return 0;
}
