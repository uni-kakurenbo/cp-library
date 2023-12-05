#pragma once

// #include "../inner/inner_math.hpp"
// #include "../misc/rng.hpp"

#include <cassert>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <random>
#include <ranges>


#include "snippet/internal/types.hpp"

#include "internal/dev_env.hpp"

#include "numeric/internal/primality_test.hpp"
#include "numeric/modint.hpp"

#include "random/xorshift.hpp"

#include "adapter/set.hpp"
#include "adapter/map.hpp"
#include "adapter/vector.hpp"


#include <atcoder/modint>

namespace lib {

namespace internal {

//Thanks to: https://github.com/NyaanNyaan/library/blob/master/prime/fast-factorize.hpp
namespace fast_factorize_impl {


constexpr vector<i64> divisors(const i64 n) noexcept(NO_EXCEPT) {
    assert(n >= 0);
    if(n == 0) return {};

    std::vector<std::pair<i64, i64>> v;
    for(auto &p : factorize(n)) {
        if(v.empty() || v.back().first != p) {
            v.emplace_back(p, 1);
        } else {
            v.back().second++;
        }
    }

    vector<i64> res;
    auto f = [&](auto rc, int i, i64 x) noexcept(NO_EXCEPT) -> void {
        if(i == static_cast<int>(v.size())) {
            res.push_back(x);
            return;
        }
        for(int j = static_cast<int>(v[i].second);; --j) {
            rc(rc, i + 1, x);
            if(j == 0)
                break;
            x *= v[i].first;
        }
    };

    f(f, 0, 1);
    std::ranges::sort(res);

    return res;
}


} // namespace fast_factorize_impl

} // namespace internal

} // namespace lib
