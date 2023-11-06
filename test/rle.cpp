// /*
//  * @uni_kakurenbo
//  * https://github.com/uni-kakurenbo/competitive-programming-workspace
//  *
//  * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
//  */
// /* #language C++ GCC */
// /* #region template */
// #include <bits/stdc++.h>
// using namespace std;

// #include "template/standard.hpp"
//
// output_adapter _print;
// #define print _print
// /* #endregion */

// #include "iterable/rle.hpp"

// // https://atcoder.jp/contests/abc259/tasks/abc259_c

// signed main() {
//     string s, t; cin >> s >> t;
//     run_length p(ALL(s)), q(ALL(t));
//     debug(p, q);

//     if(p.size() != q.size()){
//         print("No");
//         return 0;
//     }

//     REP(i, size(p)) {
//         if(p[i].$F != q[i].$F || ( p[i].$S != q[i].$S && (p[i].$S > q[i].$S || p[i].$S < 2))) {
//             print("No");
//             return 0;
//         }
//     }
//     print("Yes");

//     return 0;
// }
