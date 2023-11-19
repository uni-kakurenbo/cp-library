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

#include "adapter/vector.hpp"

#include "structure/graph.hpp"
#include "graph/spanning_tree.hpp"


namespace lib {


// TODO: Vector View
template <
    std::input_iterator I0, std::input_iterator I1,
    std::sentinel_for<I0> S0, std::sentinel_for<I1> S1
>
auto manhattan_mst_candidate_edges(
    I0 x_first, S0 x_last, I1 y_first, S1 y_last
) noexcept(NO_EXCEPT) {
    using size_type = internal::size_t;
    using cost_type = std::common_type_t<std::iter_value_t<I0>, std::iter_value_t<I1>>;

    std::vector<cost_type> xs(x_first, x_last), ys(y_first, y_last);
    assert(xs.size() == ys.size());

    std::vector<size_type> indices(xs.size());
    std::iota(ALL(indices), 0);

    vector<std::tuple<size_type,size_type,cost_type>> res;

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



template <
    std::input_iterator I0, std::input_iterator I1,
    std::sentinel_for<I0> S0, std::sentinel_for<I1> S1
>
auto manhattan_mst_edges(
    I0 x_first, S0 x_last, I1 y_first, S1 y_last,
    std::common_type_t<std::iter_value_t<I0>, std::iter_value_t<I1>> *const cost_sum = nullptr
) noexcept(NO_EXCEPT) {
    using cost_type = std::common_type_t<std::iter_value_t<I0>, std::iter_value_t<I1>>;
    using size_type = internal::size_t;

    assert(std::distance(x_first, x_last) == std::distance(y_first, y_last));

    if(cost_sum) *cost_sum = 0;

    vector<std::tuple<size_type,size_type,cost_type>> res;
    atcoder::dsu uf(static_cast<int>(std::ranges::distance(x_first, x_last)));

    ITR(u, v, w, (manhattan_mst_candidate_edges<I0,I1>(x_first, x_last, y_first, y_last))) {
        if(not uf.same(u, v)) {
            uf.merge(u, v);
            res.emplace_back(u, v, w);
            if(cost_sum) *cost_sum += w;
        }
    }

    return res;
}


template<class Graph>
template<
    std::input_iterator I0, std::input_iterator I1,
    std::sentinel_for<I0> S0, std::sentinel_for<I1> S1
>
typename Graph::cost_type internal::graph_impl::mixin<Graph>::build_manhattan_mst(
    I0 x_first, S0 x_last, I1 y_first, S1 y_last
) noexcept(NO_EXCEPT)
{
    using cost_type = typename Graph::cost_type;
    using size_type = internal::size_t;

    assert(std::ranges::distance(x_first, x_last) == std::ranges::distance(y_first, y_last));

    cost_type res = 0;

    ITR(u, v, w, (manhattan_mst_edges<I0,I1,S0,S1,cost_type,size_type>(x_first, x_last, y_first, y_last))) {
        this->add_edge_bidirectionally(u, v, w);
    }

    return res;
}


} // namespace lib
