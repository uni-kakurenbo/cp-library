#pragma once


#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <type_traits>

#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/bit.hpp"

#include "utility/hilbert_order.hpp"


namespace lib {

template<class R, class EF, class PF, class EB = EF, class PB = PF>
class interval_plannner {
  private:
    using size_t = internal::size_t;

  public:
    const EF& expand_front; const EB& expand_back;
    const PF& contract_front; const PB& contract_back;
    const R& evaluate;

    interval_plannner(
        const EF& expand_front, const EB& expand_back,
        const PF& contract_front, const PB& contract_back,
        const R& evaluate
    ) noexcept(NO_EXCEPT)
      : expand_front(expand_front), expand_back(expand_back),
        contract_front(contract_front), contract_back(contract_back),
        evaluate(evaluate)
    {}

    interval_plannner(const EF& expand, const PF& contract, const R& evaluate) noexcept(NO_EXCEPT)
      : interval_plannner(expand, expand, contract, contract, evaluate)
    {}


    template<class QI, class RI>
    void scan(const QI query_first, const QI query_last, const RI res_first) noexcept(NO_EXCEPT) {
        const size_t q = std::distance(query_first, query_last);

        size_t n = 0;
        for(auto query=query_first; query!=query_last; ++query) {
            chmax(n, std::max(query->first, query->second));
        }
        n = 1 << bit_width<std::make_unsigned_t<size_t>>(n);

        std::vector<i64> orders(q);
        {
            auto query = query_first;
            auto order = orders.begin();
            for(; query!=query_last; ++query, ++order) {
                *order = hilbert_order<i64>(n, *query);
            }
        }

        std::vector<size_t> indices(q); std::iota(ALL(indices), 0);
        std::sort(ALL(indices), [&orders](const size_t i, const size_t j) { return orders[i] < orders[j]; });

        size_t l = 0, r = 0;
        ITR(i, indices) {
            while(l > query_first[i].first) expand_front(--l);
            while(r < query_first[i].second) expand_back(r++);
            while(l < query_first[i].first) contract_front(l++);
            while(r > query_first[i].second) contract_back(--r);
            res_first[i] = evaluate();
        }
    }

    template<class QI>
    auto scan(const QI query_first, const QI query_last) noexcept(NO_EXCEPT) {
        std::vector<std::invoke_result_t<R>> res(std::distance(query_first, query_last));
        this->scan(query_first, query_last, res.begin());
        return res;
    }

    template<class Q>
    auto scan(const Q queries) noexcept(NO_EXCEPT) {
        std::vector<std::invoke_result_t<R>> res(std::distance(std::begin(queries), std::end(queries)));
        this->interval_scan(std::begin(queries), std::end(queries), res.begin());
        return res;
    }
};


} // namespace lib
