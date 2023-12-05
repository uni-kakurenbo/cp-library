#pragma once


#include <cassert>
#include <vector>
#include <ranges>


#include "adapter/vector.hpp"
#include "numeric/internal/factorize.hpp"


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


using internal::fast_factorize_impl::divisors;


} // namespace lib
