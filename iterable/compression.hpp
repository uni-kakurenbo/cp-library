#pragma once

#include <iterator>
#include <vector>
#include <map>
#include <algorithm>

#include "internal/types.hpp"

namespace lib {

template<class T, class container = std::vector<internal::size_t>>
struct compression : container {
    using size_type = internal::size_t;

  protected:
    std::vector<T> values;

  public:
    explicit compression() {}
    template<class I> compression(const I first, const I last) {
        this->values.assign(first, last);
        std::sort(this->values.begin(), this->values.end());
        this->values.erase(std::unique(this->values.begin(), this->values.end()), this->values.end());
        this->resize(std::distance(first, last));
        for(auto itr=std::begin(*this),val=this->values.begin(),e=first; e!=last; ++itr,++val,++e) {
            *itr = this->rank(*e);
        }
    }

    inline size_type rank(const T& val) const {
        return std::distance(this->values.begin(), std::lower_bound(this->values.begin(), this->values.end(), val));
    }

    inline T val() const { return this->values[val]; }
    inline T operator()(const internal::size_t val) const { return this->values[val]; }
};

} // namespace lib
