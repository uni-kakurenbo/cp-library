#pragma once


#include <vector>
#include <map>
#include <tuple>
#include <numeric>
#include <algorithm>

#include <atcoder/dsu>

#include "internal/dev_assert.hpp"
#include "snippet/iterations.hpp"
#include "internal/types.hpp"

#include "graph.hpp"


namespace lib {


// TODO: Vector View
template <class cost_type, class size_type = internal::size_t>
std::vector<std::tuple<size_type,size_type,cost_type>> manhattan_mst_candidate_edges(
    std::vector<cost_type> xs, std::vector<cost_type> ys
) {
    dev_assert(xs.size() == ys.size());

    std::vector<size_type> indices(xs.size());
    std::iota(ALL(indices), 0);

    std::vector<std::tuple<size_type,size_type,cost_type>> res;

    REP(2) {
        REP(2) {
            std::sort(ALL(indices), [&](const auto i, const auto j) { return xs[i] + ys[i] < xs[j] + ys[j]; });

            std::map<cost_type,size_type> scan;
            ITR(i, indices) {
                for(auto itr = scan.lower_bound(-ys[i]); itr!=scan.end(); itr=scan.erase(itr)) {
                    const auto j = itr->second;
                    if(xs[i] - xs[j] < ys[i] - ys[j]) break;
                    res.emplace_back(i, j, std::abs(xs[i] - xs[j]) + std::abs(ys[i] - ys[j]));
                }
                scan[-ys[i]] = i;
            }

            std::swap(xs, ys);
        }
        ITRR(x, xs) x *= -1;
    }

    std::sort(ALL(res), [&](const auto& p, const auto& q) { return std::get<2>(p) < std::get<2>(q); });

    return res;
}


template<class cost_type, class size_type = internal::size_t>
std::vector<std::tuple<size_type,size_type,cost_type>> manhattan_mst_edges(
    const std::vector<cost_type>& xs, const std::vector<cost_type>& ys,
    cost_type *const cost_sum = nullptr
) {
    dev_assert(xs.size() == ys.size());

    if(cost_sum) *cost_sum = 0;

    std::vector<std::tuple<size_type,size_type,cost_type>> res;
    atcoder::dsu uf(xs.size());

    ITR(u, v, w, (manhattan_mst_candidate_edges<cost_type,size_type>(xs, ys))) {
        if(not uf.same(u, v)) {
            uf.merge(u, v);
            res.emplace_back(u, v, w);
            if(cost_sum) *cost_sum += w;
        }
    }

    return res;
}


template<class E>
template <class cost_type, class size_type>
cost_type graph<E>::build_manhattan_mst(const std::vector<cost_type>& xs, const std::vector<cost_type>& ys) {
    dev_assert(xs.size() == ys.size());

    cost_type res = 0;

    ITR(u, v, w, (manhattan_mst_edges<cost_type,size_type>(xs, ys))) {
        this->add_edge_bidirectionally(u, v, w);
    }

    return res;
}


} // namespace lib
