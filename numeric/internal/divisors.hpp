#pragma once


#include <cassert>
#include <vector>
#include <ranges>
#include <algorithm>


#include "numeric/internal/factorize.hpp"
#include "numeric/modular/modint_interface.hpp"


namespace uni {

namespace internal {


//Thanks to: https://github.com/NyaanNyaan/library/blob/master/prime/fast-factorize.hpp
template<modint_family Small, modint_family Large, class R>
void divisors(const u64 n, R *const res) noexcept(NO_EXCEPT) {
    if(n == 0) return;

    std::vector<u64> facts; factorize<Small, Large>(n, &facts);
    std::ranges::sort(facts);

    std::vector<std::pair<u64, int>> v;
    for(auto &p : facts) {
        if(v.empty() || v.back().first != p) {
            v.emplace_back(p, 1);
        } else {
            v.back().second++;
        }
    }

    using value_type = std::ranges::range_value_t<R>;

    const auto size = std::ranges::ssize(v);
    auto f = [&](auto rc, int i, value_type x) noexcept(NO_EXCEPT) -> void {
        if(i == size) {
            res->push_back(x);
            return;
        }
        for(int j = v[i].second; ; --j) {
            rc(rc, i + 1, x);
            if(j == 0) break;
            x *= static_cast<value_type>(v[i].first);
        }
    };
    f(f, 0, 1);
}


} // namespace internal

} // namespace uni
