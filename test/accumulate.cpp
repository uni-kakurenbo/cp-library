#include <iostream>
#include <vector>

#include "template.hpp"
#include "numeric/accumulate.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n);
    REP(i, n) std::cin >> a[i];
    Accumulate pre_sum(ALL(a), 0LL);
    debug(pre_sum);
    return 0;
}
