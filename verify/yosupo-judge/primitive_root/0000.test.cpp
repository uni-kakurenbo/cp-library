/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */


#define PROBLEM "https://judge.yosupo.jp/problem/primitive_root"

#include "sneaky/enforce_int128_enable.hpp"

#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "numeric/fast_prime.hpp"

signed main() {
    uni::i32 q; input >> q;
    REP(q) {
        uni::u64 p; input >> p;
        print(uni::primitive_root(p));
    }
}
