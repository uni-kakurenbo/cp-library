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
#include "adapter/map.hpp"
#include "numeric/boundary_seeker.hpp"
#include "hash/sequence_hasher.hpp"

signed main() {
    int n; std::cin >> n;
    lib::string s; std::cin >> s;
    lib::sequence_hasher hash(ALL(s));

    auto valid = [&](int m) -> bool {
        lib::unordered_map<lib::u64,int> mp;
        FORD(i, n-m) mp[hash.subseq(i, m)] = i;
        FORD(i, n-m) {
            lib::u64 h = hash.subseq(i, m);
            if(not mp.count(h)) continue;
            if(mp[h] <= i - m) return true;
        }
        return false;
    };

    lib::boundary_seeker<int> seeker(valid);
    print(seeker.bound(0, n));

    return 0;
}
