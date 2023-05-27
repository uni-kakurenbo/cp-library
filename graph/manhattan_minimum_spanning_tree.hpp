#pragma once

#include <cassert>
#include <vector>
#include <map>
#include <tuple>
#include <numeric>
#include <algorithm>

#include <atcoder/dsu>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "graph.hpp"

namespace lib {


// TODO: Vector View
template <class I, class J = I, class cost_type = typename std::iterator_traits<I>::value_type, class size_type = internal::size_t>
std::vector<std::tuple<size_type,size_type,cost_type>> manhattan_mst_candidate_edges(
    const I x_first, const I x_last, const J y_first, const J y_last
) noexcept(DEV_ENV) {
    std::vector<cost_type> xs(x_first, x_last), ys(y_first, y_last);
    assert(xs.size() == ys.size());

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


template<class I, class J = I, class cost_type = typename std::iterator_traits<I>::value_type, class size_type = internal::size_t>
std::vector<std::tuple<size_type,size_type,cost_type>> manhattan_mst_edges(
    const I x_first, const I x_last, const J y_first, const J y_last,
    cost_type *const cost_sum = nullptr
) noexcept(DEV_ENV) {
    assert(std::distance(x_first, x_last) == std::distance(y_first, y_last));

    if(cost_sum) *cost_sum = 0;

    std::vector<std::tuple<size_type,size_type,cost_type>> res;
    atcoder::dsu uf(std::distance(x_first, x_last));

    ITR(u, v, w, (manhattan_mst_candidate_edges<I,J,cost_type,size_type>(x_first, x_last, y_first, y_last))) {
        if(not uf.same(u, v)) {
            uf.merge(u, v);
            res.emplace_back(u, v, w);
            if(cost_sum) *cost_sum += w;
        }
    }

    return res;
}


template<class edge_cost>
template <class I, class J, class cost_type, class size_type>
cost_type graph<edge_cost>::build_manhattan_mst(const I x_first, const I x_last, const J y_first, const J y_last) noexcept(DEV_ENV) {
    assert(std::distance(x_first, x_last) == std::distance(y_first, y_last));

    cost_type res = 0;

    ITR(u, v, w, (manhattan_mst_edges<I,J,cost_type,size_type>(x_first, x_last, y_first, y_last))) {
        this->add_edge_bidirectionally(u, v, w);
    }

    return res;
}


} // namespace lib
