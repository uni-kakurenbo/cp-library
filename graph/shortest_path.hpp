#pragma once


#include <algorithm>


#include "graph/internal/bfs.hpp"
#include "graph/internal/01bfs.hpp"
#include "graph/internal/dijkstra.hpp"

#include "adaptor/vector.hpp"


namespace lib {


template<class Node, class Prev, class Res>
void restore_path(Node back, const Prev& prev, Res *const res) {
    res->clear();
    while(back >= 0) {
        res->emplace_back(back);
        back = prev[back];
    }

    std::ranges::reverse(*res);
}


template<class Node, class Prev, class Res = lib::vector<Node>>
lib::vector<Node> restore_path(Node back, const Prev& prev) {
    lib::vector<Node> res;
    restore_path(back, prev, &res);
    return res;
}


}
