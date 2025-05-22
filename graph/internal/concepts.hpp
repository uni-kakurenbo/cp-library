#pragma once


#include <ranges>
#include <concepts>


namespace uni {


namespace internal {


template<class Range, class Graph>
concept topological_sortable_with =
    std::same_as<std::ranges::range_value_t<Range>, typename Graph::node_type> &&
    requires (Range *const r, typename Graph::node_type v) {
        r->clear();
        r->push_back(v);
        std::ranges::ssize(*r);
    };


}


}
