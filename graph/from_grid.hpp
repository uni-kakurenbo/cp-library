#pragma once

#include "grid.hpp"
#include "graph.hpp"

template<class edge_cost>
template<class G, class U>
void lib::graph<edge_cost>::from_grid(const G &grid, U available) {
    this->clear();
    this->resize(grid.height() * grid.width());

    REP(i, grid.height()) REP(j, grid.width()) {
        if(grid(i, j) != available) continue;
        if(i+1 < grid.height() and grid(i+1, j) == available) {
            this->template add_edge<graph<edge_cost>::edge_type::undirected>(grid.id(i, j), grid.id(i+1, j));
        }
        if(j+1 < grid.width() and grid(i, j+1) == available) {
            this->template add_edge<graph<edge_cost>::edge_type::undirected>(grid.id(i, j), grid.id(i, j+1));
        }
    }
}
