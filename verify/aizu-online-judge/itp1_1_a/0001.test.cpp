/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "include/all.hpp"

static_assert(std::same_as<uni::algebraic::make_opposite_t<uni::algebraic::affine<std::nullptr_t>>, uni::algebraic::opposite<uni::algebraic::affine<std::nullptr_t>>>);
static_assert(std::same_as<uni::algebraic::make_opposite_t<uni::algebraic::make_opposite_t<uni::algebraic::affine<std::nullptr_t>>>, uni::algebraic::affine<std::nullptr_t>>);
static_assert(std::same_as<uni::algebraic::make_opposite_t<uni::algebraic::addition<std::nullptr_t>>, uni::algebraic::addition<std::nullptr_t>>);

signed main() {
    print("Hello World");
}
