/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://atcoder.jp/contests/abc242/tasks/abc242_b"

#include "snippet/fast_io.hpp"
#include "adapter/io.hpp"
#include "adapter/string.hpp"
#include "iterable/applied.hpp"

signed main() {
    lib::string s; input >> s;
    print(lib::sorted(s));
}
