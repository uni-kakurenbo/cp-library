#define PROBLEM "https://atcoder.jp/contests/abc141/tasks/abc141_e"

#include <bits/stdc++.h>
#include "template.hpp"
#include "hash/sequence_hasher.hpp"
#include "numeric/boundary_seeker.hpp"

// https://atcoder.jp/contests/abc141/tasks/abc141_e
signed main() {
    int n; cin >> n;
    string s; cin >> s;
    lib::sequence_hasher hash(ALL(s));

    auto valid = [&](int m) -> bool {
        lib::unordered_map<u64,int> mp;
        FORD(i, n-m) mp[hash.subseq(i, m)] = i;
        FORD(i, n-m) {
            u64 h = hash.subseq(i, m);
            if(not mp.count(h)) continue;
            if(mp[h] <= i - m) return true;
        }
        return false;
    };

    lib::boundary_seeker<int> seeker(valid);
    print(seeker.bound(0, n));

    return 0;
}
