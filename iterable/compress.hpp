#pragma once

#include <iterator>
#include <vector>
#include <algorithm>

template<class I, class T = typename std::iterator_traits<I>::value_type, class C = std::vector<T>>
struct Compress : C {
  protected:
    std::vector<T> values;

  public:
    Compress() {}
    Compress(const I first, const I last) {
        this->values.assign(first, last);
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());
        this->resize(std::distance(first, last));
        for(auto itr=this->begin(),val=values.begin(),e=first; e!=last; ++itr,++val,++e) {
            *itr = std::distance(values.begin(), std::lower_bound(values.begin(), values.end(), *e));
        }
    }
    inline T operator()(const T val) const {
        return values[val];
    }
};
