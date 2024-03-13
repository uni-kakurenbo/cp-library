#pragma once

#include <vector>
#include <queue>
#include <stack>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "structure/graph.hpp"

template<class Graph>
bool lib::internal::graph_impl::mixin<Graph>::is_bipartite() const noexcept(NO_EXCEPT) {
    using color_type = std::int8_t;

    std::valarray<color_type> color(0, this->vertices());

    REP(s, this->vertices()) {
        if(color[s] != 0) continue;

        std::stack<size_type> stack;
        stack.push(s);
        color[s] = 1;

        while(not stack.empty()) {
            auto v = stack.top(); stack.pop();
            auto c = color[v];

            ITR(nv, this->operator[](v)) {
                if(color[nv] == c) return false;
                if(color[nv] == 0) {
                    color[nv] = -c;
                    stack.push(nv);
                }
            }
        }
    }

    return true;
}
