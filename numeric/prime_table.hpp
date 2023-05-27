#pragma once


#include <vector>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace lib {


template<class container = std::vector<bool>> struct prime_flags : container {
    prime_flags(const internal::size_t max) noexcept(DEV_ENV) : container(max+1, true) {
        (*this)[0] = (*this)[1] = false;
        for(internal::size_t p=2; p*p<=max; p++) if((*this)[p]) {
            for(internal::size_t i=p*p; i<=max; i+=p) (*this)[i] = false;
        }
    }
};

template<class T, class container = std::valarray<T>> struct prime_table : container {
  protected:
    std::vector<bool> is_prime;

  public:
    prime_table() noexcept(DEV_ENV) {}
    prime_table(const T max) noexcept(DEV_ENV) : is_prime(max+1, true) {
        is_prime[0] = is_prime[1] = false;
        FOR(p, T{2}, max) {
            if(is_prime[p]) {
                for(T i=p*p; i<=max; i+=p) is_prime[i] = false;
                this->emplace_back(p);
            }
        }
    }
    inline bool operator()(const T index) const noexcept(DEV_ENV) {
        return is_prime[index];
    }
};


} // namespace lib
