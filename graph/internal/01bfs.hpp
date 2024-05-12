#pragma once


#include <deque>
#include <utility>
#include <functional>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/concepts.hpp"

#include "numeric/limits.hpp"
#include "adaptor/valarray.hpp"

#include "structure/graph.hpp"
#include "internal/auto_holder.hpp"


template<class Graph>
template<uni::internal::item_or_convertible_range<typename Graph::node_type> Source, class Dist, class Prev>
void uni::internal::graph_impl::mixin<Graph>::shortest_path_with_01cost(
    Source&& s, Dist *const dist, Prev *const prev,
    const node_type& unreachable, const node_type& root
) const noexcept(NO_EXCEPT) {
    std::deque<node_type> que;

    dist->assign(this->size(), uni::numeric_limits<cost_type>::arithmetic_infinity());
    if constexpr(!std::is_same_v<Prev, std::nullptr_t>) prev->assign(this->size(), unreachable);

    if constexpr(std::ranges::range<Source>) {
        ITR(v, s) {
            que.push_back(v), dist->operator[](v) = 0;
            if constexpr(!std::is_same_v<Prev, std::nullptr_t>) prev->operator[](v) = root;
        }
    }
    else {
        que.push_back(s), dist->operator[](s) = 0;
        if constexpr(!std::is_same_v<Prev, std::nullptr_t>) prev->operator[](s) = root;
    }

    while(!que.empty()) {
        const auto u = que.front(); que.pop_front();
        const cost_type d = dist->operator[](u);

        ITR(e, (*this)[u]) {
            const node_type v = e.to; const auto cost = e.cost;

            if(dist->operator[](v) <= d + cost) continue;

            dist->operator[](v) = d + cost;
            if constexpr(!std::is_same_v<Prev, std::nullptr_t>) prev->operator[](v) = u;

            if(cost) que.push_back(v);
            else que.push_front(v);
        }
    }
}

template<class Graph>
template<uni::internal::item_or_convertible_range<typename Graph::node_type> Source>
auto uni::internal::graph_impl::mixin<Graph>::shortest_path_with_01cost(Source&& s) const noexcept(NO_EXCEPT) {
    uni::auto_holder<typename uni::internal::graph_impl::mixin<Graph>::node_type, cost_type> dist;
    this->shortest_path_with_01cost(std::forward<Source>(s), &dist);
    return dist;
}
