/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include <cassert>
#include <ranges>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adaptor/io.hpp"
#include "random/adaptor.hpp"
#include "numeric/subset_enumerator.hpp"

signed main() {
  print("Hello World");

  REP(n, 25U) {
    uni::unordered_set<uni::u32> st;

    auto enumerator = uni::subset_enumerator(n);

    ITR(v, enumerator) {
      assert(uni::is_subset_of(v, n));
      st.emplace(v);
    }

    ITR(v, enumerator | std::views::reverse) {
      assert(st.contains(v));
    }

    assert(st.ssize() == enumerator.size());
  }
}
