#pragma once


#include <vector>

#include "internal/dev_env.hpp"
#include "adapter/vector.hpp"

namespace lib {


template<class T>
vector<T> divisors_sieve(const T k) noexcept(NO_EXCEPT) {
    vector<T> res;

    for(T i=1; i*i<=k; ++i) {
        if(k%i == 0) {
            res.emplace_back(i);
            if(i*i < k) res.emplace_back(k/i);
        }
    }

    res.sort();

    return res;
}


} // namespace lib
