#include <iostream>
#include "template.hpp"
#include "numeric/quotient_enumerator.hpp"

#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_quotients"

int main() {
    i64 n; std::cin >> n;

    lib::quotient_enumerator qe(n);

    print(qe.size());
    for(auto itr = qe.rbegin(); itr != qe.rend(); ++itr) {
        print << std::get<0>(*itr) << " ";
    }

    print();
}
