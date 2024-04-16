#pragma once

#include "internal/dev_env.hpp"

#include "structure/grid.hpp"
#include "structure/graph.hpp"

template<class Graph>
template<bool REV, class G, class U>
void uni::internal::graph_impl::mixin<Graph>::parse_grid(const G &grid, U available) noexcept(NO_EXCEPT) {
    this->clear();
    this->resize(grid.height() * grid.width());

    REP(i, grid.height()) REP(j, grid.width()) {
        if(REV ^ (grid(i, j) != available)) continue;
        if(i+1 < grid.height() and (REV ^ (grid(i+1, j) == available))) {
            this->template add_edge_bidirectionally(grid.id(i, j), grid.id(i+1, j));
        }
        if(j+1 < grid.width() and (REV ^ (grid(i, j+1) == available))) {
            this->template add_edge_bidirectionally(grid.id(i, j), grid.id(i, j+1));
        }
    }
}
