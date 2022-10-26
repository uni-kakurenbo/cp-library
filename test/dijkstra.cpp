#include <iostream>
#include <utility>

#include "template.hpp"
#include "apply.hpp"
#include "graph.hpp"
#include "graph/dijkstra.hpp"

signed main() {
    int n, m; std::cin >> n >> m;
    std::vector<ll> h(n); ITRR(v, h) std::cin >> v;

    Graph<Edge<ll>> G(n);
    LOOP(m) {
        int u, v; std::cin >> u >> v; --u, --v;
        if(h[v] > h[u]) std::swap(u, v);
        G[v].emplace_back(u, h[u]-h[v]), G[u].emplace_back(v, 0);
    }
    debug(G);

    std::vector<ll> res(n, INF64);
    dijkstra(G, 0, &res);

    debug(res);

    Max<ll> ans = 0;
    REP(i, n) ans <<= h[0] - h[i] - res[i];

    std::cout << ans << ln;

    return 0;
}
