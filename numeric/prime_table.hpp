#pragma once


#include <vector>

#include "internal/types.hpp"


namespace lib {


template<class container = std::vector<bool>> struct is_prime : container {
    is_prime(const internal::size_t max) : container(max+1, true) {
        (*this)[0] = (*this)[1] = false;
        for(internal::size_t p=2; p*p<=max; p++) if((*this)[p]) {
            for(internal::size_t i=p*p; i<=max; i+=p) (*this)[i] = false;
        }
    }
};

template<class T, class container = std::vector<T>> struct prime_table : container {
  protected:
    std::vector<bool> is_prime;

  public:
    prime_table() {}
    prime_table(const T max) : is_prime(max+1, true) {
        is_prime[0] = is_prime[1] = false;
        FOR(p, T{2}, max) {
            if(is_prime[p]) {
                for(T i=p*p; i<=max; i+=p) is_prime[i] = false;
                this->emplace_back(p);
            }
        }
    }
    inline bool operator()(const T index) const {
        return is_prime[index];
    }
};


} // namespace lib
