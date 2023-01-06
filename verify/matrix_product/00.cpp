#include <bits/stdc++.h>
#include <atcoder/modint>
#include "template.hpp"
#include "numeric/matrix.hpp"

signed main() {
    int n, m, k; std::cin >> n >> m >> k;
    lib::matrix<atcoder::modint998244353> a(n, m), b(m, k);
    input >> a >> b;
    print(a*b);
}
