#pragma once


#include <queue>
#include <ranges>

#include "snippet/iterations.hpp"
#include "snippet/internal/types.hpp"

#include "internal/dev_env.hpp"

#include "numeric/limits.hpp"
#include "adaptor/valarray.hpp"

#include "structure/graph.hpp"
#include "internal/auto_holder.hpp"


template<class Graph>
template<uni::internal::item_or_convertible_range<typename Graph::node_type> Source, class Dist, class Prev>
void uni::internal::graph_impl::mixin<Graph>::shortest_path_without_cost(
    Source&& s, Dist *const dist, Prev *const prev,
    const node_type& unreachable, const node_type& root
) const noexcept(NO_EXCEPT) {
    dist->assign(this->size(), uni::numeric_limits<cost_type>::arithmetic_infinity());
    if constexpr(!std::is_same_v<Prev, std::nullptr_t>) prev->assign(this->size(), unreachable);

    std::queue<node_type> que;

    if constexpr(std::ranges::range<Source>) {
        ITR(v, s) {
            que.push(v), dist->operator[](v) = 0;
            if constexpr(!std::is_same_v<Prev, std::nullptr_t>) prev->operator[](v) = root;
        }
    }
    else {
        que.push(s), dist->operator[](s) = 0;
        if constexpr(!std::is_same_v<Prev, std::nullptr_t>) prev->operator[](s) = root;
    }

    while(!que.empty()) {
        const node_type v = que.front(); que.pop();
        ITR(nv, this->operator[](v)) {
            if(dist->operator[](nv.to) < uni::numeric_limits<cost_type>::arithmetic_infinity()) { continue; }

            dist->operator[](nv.to) = dist->operator[](v) + 1;
            if constexpr(!std::is_same_v<Prev, std::nullptr_t>) prev->operator[](nv.to) = v;

            que.push(nv.to);
        }
    }
}

template<class Graph>
template<uni::internal::item_or_convertible_range<typename Graph::node_type> Source>
auto uni::internal::graph_impl::mixin<Graph>::shortest_path_without_cost(Source&& s) const noexcept(NO_EXCEPT) {
    uni::auto_holder<node_type, cost_type> dist;
    this->shortest_path_without_cost(std::forward<Source>(s), &dist);
    return dist;
}
