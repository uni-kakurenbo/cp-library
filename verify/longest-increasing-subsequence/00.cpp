// #include <bits/stdc++.h>
#include "template.hpp"

#include "iterable/longest_increasing_subsequence.hpp"

signed main() {
    int n; std::cin >> n;
    std::vector<int> a(n); input >> a;
    lib::lis<true,int> lis(ALL(a));

    debug(lis);
    print(lis.size(), lis.indices);
}
