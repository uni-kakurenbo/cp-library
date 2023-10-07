#pragma once


#include "graph/internal/bfs.hpp"
#include "graph/internal/01bfs.hpp"
#include "graph/internal/dijkstra.hpp"

#include "adapter/vector.hpp"


namespace lib {


template<class Node, class Prev, class Res>
void restore_shortest_path(Node v, const Prev& prev, Res *const res) {
    res->clear();
    while(v >= 0) {
        res->emplace_back(v);
        v = prev[v];
    }
}


template<class Node, class Prev, class Res = lib::vector<Node>>
lib::vector<Node> restore_shortest_path(Node v, const Prev& prev) {
    lib::vector<Node> res;
    restore_shortest_path(v, prev, &res);
    return res;
}


}
