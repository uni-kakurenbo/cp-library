#include <iostream>
#include <vector>

#include "template/standard.hpp"
#include "iterable/adjacent_difference.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n);
    REP(i, n) std::cin >> a[i];
    adjacent_difference diff(ALL(a));
    debug(diff);
    return 0;
}
