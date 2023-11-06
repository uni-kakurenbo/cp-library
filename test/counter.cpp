#include <iostream>
#include <vector>

#include "template/standard.hpp"
#include "iterable/compressed.hpp"
#include "iterable/counter.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n);
    REP(i, n) std::cin >> a[i];
    compressed comp(ALL(a));
    counter counter(ALL(a)), comped_counter(ALL(comp));
    debug(counter, comped_counter);
    return 0;
}
