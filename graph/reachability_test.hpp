#pragma once


#include <ranges>
#include <limits>


#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "adaptor/vector.hpp"

#include "structure/graph.hpp"
#include "graph/topological_sort.hpp"

#include "view/chunk.hpp"
#include "view/enumerate.hpp"


namespace uni {


template<class Graph>
template<std::ranges::sized_range R>
auto uni::internal::graph_impl::mixin<Graph>::test_reachability(R&& queries) const noexcept(NO_EXCEPT) {
    using impl_type = u128;
    constexpr auto CHUNK_SIZE = std::numeric_limits<impl_type>::digits;

    std::vector<bool> res(std::ranges::size(queries));

    uni::vector<node_type> vs;
    this->sort_topologically(&vs);

    debug(queries);

    ITR(i, qs, queries | uni::views::chunk(CHUNK_SIZE) | uni::views::enumerate) {
        debug(qs);
        std::vector<impl_type> bits(this->size());

        ITR(j, p, qs | uni::views::enumerate) {
            bits[p.first] = uni::set_bit(bits[p.first], j);
        }

        ITR(v, vs) ITR(nv, this->operator[](v)) {
            bits[nv] |= bits[v];
        }

        ITR(j, p, qs | uni::views::enumerate) {
            res[i * CHUNK_SIZE + j] = uni::bit(bits[p.second], j);
        }
    }

    return res;
}


} // namespace uni
