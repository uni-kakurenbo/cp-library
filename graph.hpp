#pragma once

#include "template.hpp"
#include <utility>

template<class Cost = ll> struct Edge {
    int to; Cost cost;
    Edge(int t, Cost w) : to(t), cost(w) {}
    std::pair<int,Cost> _debug() { return { to, cost }; };
};
template <class T> using Graph = std::vector<std::vector<T>>;
