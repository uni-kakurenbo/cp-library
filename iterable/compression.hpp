#pragma once

#include <iterator>
#include <vector>
#include <algorithm>

namespace Lib {

template<class T, class Container = std::vector<T>>
struct Compression : Container {
  protected:
    std::vector<T> values;

  public:
    Compression() {}
    template<class I> Compression(const I first, const I last) {
        this->values.assign(first, last);
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());
        this->resize(std::distance(first, last));
        for(auto itr=begin(*this),val=values.begin(),e=first; e!=last; ++itr,++val,++e) {
            *itr = std::distance(values.begin(), std::lower_bound(values.begin(), values.end(), *e));
        }
    }
    inline T operator()(const T val) const {
        return values[val];
    }
};

} // namespace Lib
