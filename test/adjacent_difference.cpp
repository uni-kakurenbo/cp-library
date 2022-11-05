#include <iostream>
#include <vector>

#include "template.hpp"
#include "iterable/adjacent_difference.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n);
    REP(i, n) std::cin >> a[i];
    AdjacentDifference diff(ALL(a));
    debug(diff);
    return 0;
}
