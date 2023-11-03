#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_1_A&lang=ja"

#include <bits/stdc++.h>
#include "template.hpp"
#include "numeric/fast_prime.hpp"

signed main() {
    int n; std::cin >> n;
    print << n << ": ";
    print(lib::factorize(n));
}
