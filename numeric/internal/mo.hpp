#pragma once


#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <concepts>
#include <ranges>


#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adapter/valarray.hpp"
#include "adapter/vector.hpp"

#include "numeric/bit.hpp"
#include "numeric/hilbert_order.hpp"


namespace lib {

template<class R, class EF, class PF, class EB = EF, class PB = PF>
class interval_plannner {
    using size_type = internal::size_t;

  public:
    EF expand_front; EB expand_back;
    PF contract_front; PB contract_back;
    R evaluate;

    using evaluator_result_t = std::invoke_result_t<R>;

    interval_plannner(
        EF expand_front, EB expand_back,
        PF contract_front, PB contract_back,
        R evaluate
    ) noexcept(NO_EXCEPT)
      : expand_front(expand_front), expand_back(expand_back),
        contract_front(contract_front), contract_back(contract_back),
        evaluate(evaluate)
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
        n = 1 << bit_width<std::make_unsigned_t<size_type>>(n);

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
            while(l > query_first[i].first) expand_front(--l);
            while(r < query_first[i].second) expand_back(r++);
            while(l < query_first[i].first) contract_front(l++);
            while(r > query_first[i].second) contract_back(--r);
            res_first[i] = evaluate();
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
