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

// https://atcoder.jp/contests/abc284/tasks/abc284_f

#include "hash/sequence_hasher.hpp"

signed main() {
    int n; cin >> n;
    string s; cin >> s;
    string t = s; reverse(ALL(t));

    lib::sequence_hasher hash0(ALL(s)), hash1(ALL(t));

    int ans_i = -1;

    FOR(i, 0, n) {
        if(hash1(0, n-i) + hash1(2*n-i, 2*n) == hash0(i, i+n)){
            ans_i = i;
            break;
        }
    }

    if(ans_i != -1){
        print(s.substr(0, ans_i) + s.substr(ans_i+n, n-ans_i));
        print(ans_i);

        return 0;
    }

    print(-1);

    return 0;
}
