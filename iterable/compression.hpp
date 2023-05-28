#pragma once


#include <iterator>
#include <vector>
#include <map>
#include <algorithm>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace lib {

template<class T = i64, class container = std::vector<internal::size_t>>
struct compression : container {
    using size_type = internal::size_t;

  protected:
    std::vector<T> values;

  public:
    explicit compression() noexcept(NO_EXCEPT) {}
    template<class I> compression(const I first, const I last) noexcept(NO_EXCEPT) {
        this->values.assign(first, last);
        std::sort(this->values.begin(), this->values.end());
        this->values.erase(std::unique(this->values.begin(), this->values.end()), this->values.end());
        this->resize(std::distance(first, last));
        for(auto itr=std::begin(*this),val=this->values.begin(),e=first; e!=last; ++itr,++val,++e) {
            *itr = this->rank(*e);
        }
    }

    inline size_type rank(const T& val) const noexcept(NO_EXCEPT) {
        return std::distance(this->values.begin(), std::lower_bound(this->values.begin(), this->values.end(), val));
    }
    inline size_type rank2(const T& val) const noexcept(NO_EXCEPT) {
        return std::distance(this->values.begin(), std::upper_bound(this->values.begin(), this->values.end(), val)) - 1;
    }

    inline T value(const size_type rank) const noexcept(NO_EXCEPT) { return this->values[rank]; }
    inline T operator()(const internal::size_t val) const noexcept(NO_EXCEPT) { return this->values[val]; }
};

} // namespace lib
