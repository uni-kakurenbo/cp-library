#include <iostream>

#include "template/standard.hpp"
#include "data_structure/kth_element.hpp"

//https://yukicoder.me/problems/no/649

signed main() {
    int q, k; std::cin >> q >> k;
    kth_element<ll> kth(k);
    LOOP(q) {
        int type; std::cin >> type;
        if(type == 1) {
            ll v; std::cin >> v;
            kth.push(v);
        }
        if(type == 2) {
            if(!kth.has()) {
                std::cout << -1 << ln;
                continue;
            }
            std::cout << kth.get() << ln;
            kth.pop();
        }
    }

    return 0;
}
