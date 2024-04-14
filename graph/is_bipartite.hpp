#pragma once

#include <vector>
#include <queue>
#include <stack>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "structure/graph.hpp"


template<class Graph>
bool lib::internal::graph_impl::mixin<Graph>::is_bipartite() const noexcept(NO_EXCEPT) {
    valarray<std::int8_t> color(0, this->vertices());
    this->is_bipartite(&color);
    return true;
}


template<class Graph>
template<class Colors>
bool lib::internal::graph_impl::mixin<Graph>::is_bipartite(Colors *const color) const noexcept(NO_EXCEPT) {
    color->assign(this->vertices(), 0);

    REP(s, this->vertices()) {
        if(color->operator[](s) != 0) continue;

        std::stack<size_type> stack;
        stack.push(s);
        color->operator[](s) = 1;

        while(not stack.empty()) {
            auto v = stack.top(); stack.pop();
            auto c = color->operator[](v);

            ITR(nv, this->operator[](v)) {
                if(color->operator[](nv) == c) return false;
                if(color->operator[](nv) == 0) {
                    color->operator[](nv) = -c;
                    stack.push(nv);
                }
            }
        }
    }

    return true;
}
