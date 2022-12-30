#pragma once

#include <iterator>
#include <vector>
#include <algorithm>

#include "internal/types.hpp"

namespace lib {

template<class T, class container = std::vector<internal::size_t>>
struct compression : container {
  protected:
    std::vector<T> values;

  public:
    compression() {}
    template<class I> compression(const I first, const I last) {
        this->values.assign(first, last);
        sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());
        this->resize(std::distance(first, last));
        for(auto itr=begin(*this),val=values.begin(),e=first; e!=last; ++itr,++val,++e) {
            *itr = std::distance(values.begin(), std::lower_bound(values.begin(), values.end(), *e));
        }
    }
    inline T operator()(const internal::size_t val) const {
        return values[val];
    }
};

} // namespace lib
