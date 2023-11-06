#pragma once


#include <queue>

#include "snippet/iterations.hpp"
#include "snippet/internal/types.hpp"

#include "internal/dev_env.hpp"

#include "numeric/limits.hpp"
#include "adapter/valarray.hpp"

#include "structure/graph.hpp"
#include "internal/auto_holder.hpp"


template<class Graph>
template<class Cost, class Dist, class Prev>
void lib::internal::graph_impl::mixin<Graph>::distances_without_cost(
    const node_type& s, Dist *const dist, Prev *const prev,
    const node_type& unreachable, const node_type& root
) const noexcept(NO_EXCEPT) {
    dist->assign(this->size(), lib::numeric_limits<Cost>::arithmetic_infinity());
    if constexpr(not std::is_same_v<Prev,std::nullptr_t>) prev->assign(this->size(), unreachable);

    std::queue<node_type> que;
    que.push(s), dist->operator[](s) = 0;
    if constexpr(not std::is_same_v<Prev,std::nullptr_t>) prev->operator[](s) = root;

    while(not que.empty()) {
        const node_type v = que.front(); que.pop();
        ITR(nv, this->operator[](v)) {
            if(dist->operator[](nv.to) < lib::numeric_limits<Cost>::arithmetic_infinity()) { continue; }

            dist->operator[](nv.to) = dist->operator[](v) + 1;
            if constexpr(not std::is_same_v<Prev,std::nullptr_t>) prev->operator[](nv.to) = v;

            que.push(nv.to);
        }
    }
}

template<class Graph>
template<class Cost>
lib::auto_holder<typename lib::internal::graph_impl::mixin<Graph>::node_type,Cost>
lib::internal::graph_impl::mixin<Graph>::distances_without_cost(const node_type s) const noexcept(NO_EXCEPT) {
    lib::auto_holder<typename lib::internal::graph_impl::mixin<Graph>::node_type,Cost> dist;
    this->distances_without_cost<Cost>(s, &dist);
    return dist;
}
