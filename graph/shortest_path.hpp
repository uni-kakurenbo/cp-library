#pragma once


#include <algorithm>


#include "graph/internal/bfs.hpp"
#include "graph/internal/01bfs.hpp"
#include "graph/internal/dijkstra.hpp"

#include "adaptor/vector.hpp"


namespace uni {


template<class Node, class Prev, class Res>
void restore_path(Node back, const Prev& prev, Res *const res) {
    res->clear();
    while(back >= 0) {
        res->emplace_back(back);
        back = prev[back];
    }

    std::ranges::reverse(*res);
}


template<class Node, class Prev, class Res = uni::vector<Node>>
uni::vector<Node> restore_path(Node back, const Prev& prev) {
    uni::vector<Node> res;
    restore_path(back, prev, &res);
    return res;
}


}
