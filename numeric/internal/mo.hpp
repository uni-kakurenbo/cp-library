#pragma once


#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <concepts>
#include <ranges>
#include <bit>


#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adaptor/valarray.hpp"
#include "adaptor/vector.hpp"

#include "numeric/bit.hpp"
#include "numeric/hilbert_order.hpp"
#include "numeric/arithmetic.hpp"


namespace lib {

template<class R, class EF, class PF, class EB = EF, class PB = PF>
class interval_plannner {
    using size_type = internal::size_t;

  public:
    EF _expand_front; EB _expand_back;
    PF _contract_front; PB _contract_back;
    R _evaluate;

    using evaluator_result_t = std::invoke_result_t<R>;

    interval_plannner(
        EF expand_front, EB expand_back,
        PF contract_front, PB contract_back,
        R evaluate
    ) noexcept(NO_EXCEPT)
      : _expand_front(expand_front), _expand_back(expand_back),
        _contract_front(contract_front), _contract_back(contract_back),
        _evaluate(evaluate)
    {}

    interval_plannner(EF expand, PF contract, R evaluate) noexcept(NO_EXCEPT)
      : interval_plannner(expand, expand, contract, contract, evaluate)
    {}


    template<
        std::input_iterator QI, std::sentinel_for<QI> QS,
        std::output_iterator<evaluator_result_t> RI
    >
    void scan(const QI query_first, const QS query_last, const RI res_first) noexcept(NO_EXCEPT) {
        const auto q = std::ranges::distance(query_first, query_last);

        size_type n = 0;
        for(auto query=query_first; query!=query_last; ++query) {
            chmax(n, std::ranges::max(query->first, query->second));
        }
        n = std::bit_ceil(lib::to_unsigned(n));

        std::vector<i64> orders(q);
        {
            auto query = query_first;
            auto order = orders.begin();
            for(; query!=query_last; ++query, ++order) {
                *order = hilbert_order<i64>(n, *query);
            }
        }

        std::vector<size_type> indices(q); std::iota(ALL(indices), 0);
        std::ranges::sort(
            indices,
            [&orders](const size_type i, const size_type j) { return orders[i] < orders[j]; }
        );

        size_type l = 0, r = 0;
        ITR(i, indices) {
            while(l > query_first[i].first) _expand_front(--l);
            while(r < query_first[i].second) _expand_back(r++);
            while(l < query_first[i].first) _contract_front(l++);
            while(r > query_first[i].second) _contract_back(--r);
            res_first[i] = _evaluate();
        }
    }

    template<std::input_iterator QI, std::sentinel_for<QI> QS>
    auto scan(const QI query_first, const QS query_last) noexcept(NO_EXCEPT) {
        valarray<evaluator_result_t> res(std::ranges::distance(query_first, query_last));
        this->scan(query_first, query_last, std::ranges::begin(res));
        return res;
    }

    template<std::ranges::input_range Q>
    auto scan(const Q& queries) noexcept(NO_EXCEPT) {
        valarray<evaluator_result_t> res(std::ranges::distance(queries));
        this->scan(std::ranges::begin(queries), std::ranges::end(queries), std::ranges::begin(res));
        return res;
    }
};


} // namespace lib
