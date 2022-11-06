#pragma once

#include <vector>

#include "internal/types.hpp"

namespace Lib {

template<class Container = std::vector<bool>> struct IsPrime : Container {
    IsPrime(const Internal::Size max) : Container(max+1, true) {
        (*this)[0] = (*this)[1] = false;
        for(Internal::Size p=2; p*p<=max; p++) if((*this)[p]) {
            for(Internal::Size i=p*p; i<=max; i+=p) (*this)[i] = false;
        }
    }
};

template<class T, class Container = std::vector<T>> struct Primes : Container {
  protected:
    std::vector<bool> is_prime;

  public:
    Primes() {}
    Primes(const T max) : is_prime(max+1, true) {
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

} // namespace Lib
