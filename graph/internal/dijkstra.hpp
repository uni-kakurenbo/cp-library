#pragma once


#include <queue>
#include <utility>
#include <functional>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"

#include "numeric/limits.hpp"

#include "structure/graph.hpp"
#include "internal/auto_holder.hpp"


template<class Graph>
template<class Dist, class Prev>
void lib::internal::graph_impl::mixin<Graph>::shortest_path_with_cost(
    const node_type& s, Dist *const dist, Prev *const prev,
    const node_type& unreachable, const node_type& root
) const noexcept(NO_EXCEPT) {
    using state = std::pair<cost_type, node_type>;
    std::priority_queue<state, std::vector<state>, std::greater<state>> que;

    dist->assign(this->size(), lib::numeric_limits<cost_type>::arithmetic_infinity());
    if constexpr(!std::same_as<Prev, std::nullptr_t>) prev->assign(this->size(), unreachable);

    que.emplace(0, s), dist->operator[](s) = 0;
    if constexpr(!std::same_as<Prev, std::nullptr_t>) prev->operator[](s) = root;

    while(!que.empty()) {
        const auto [d, u] = que.top(); que.pop();

        if(dist->operator[](u) < d) continue;

        ITR(e, this->operator[](u)) {
            const node_type v = e.to; const auto next = d + e.cost;

            if(dist->operator[](v) <= next) continue;

            dist->operator[](v) = next;
            if constexpr(!std::same_as<Prev, std::nullptr_t>) prev->operator[](v) = u;

            que.emplace(dist->operator[](v), v);
        }
    }
}

template<class Graph>
auto lib::internal::graph_impl::mixin<Graph>::shortest_path_with_cost(const typename Graph::node_type& s) const noexcept(NO_EXCEPT) {
    lib::auto_holder<node_type, cost_type> dist;
    this->shortest_path_with_cost(s, &dist);
    return dist;
}
