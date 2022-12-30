#include <iostream>
#include <vector>

#include "template.hpp"
#include "iterable/accumulation.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n);
    REP(i, n) std::cin >> a[i];
    accumulation pre_sum(ALL(a), 0LL);
    debug(pre_sum);
    debug(pre_sum(1, 2));
    debug(pre_sum(0, -1));
    return 0;
}
