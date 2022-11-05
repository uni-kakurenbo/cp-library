#include <iostream>
#include <vector>

#include "template.hpp"
#include "iterable/compression.hpp"
#include "iterable/counter.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n);
    REP(i, n) std::cin >> a[i];
    Compression comp(ALL(a));
    Counter counter(ALL(a)), comped_counter(ALL(comp));
    debug(counter, comped_counter);
    return 0;
}
