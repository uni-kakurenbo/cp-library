#pragma once


#include <vector>


#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adapter/valarray.hpp"
#include "adapter/vector.hpp"


namespace lib {


template<class container = valarray<bool>> struct prime_flags : container {
    prime_flags(const internal::size_t max) noexcept(NO_EXCEPT) : container(max+1, true) {
        (*this)[0] = (*this)[1] = false;
        for(internal::size_t p=2; p*p<=max; p++) if((*this)[p]) {
            for(internal::size_t i=p*p; i<=max; i+=p) (*this)[i] = false;
        }
    }
};

template<class T, class container = vector<T>> struct prime_sieve : container {
  protected:
    std::vector<bool> is_prime;

  public:
    prime_sieve() noexcept(NO_EXCEPT) {}
    prime_sieve(const T max) noexcept(NO_EXCEPT) : is_prime(max+1, true) {
        is_prime[0] = is_prime[1] = false;
        FOR(p, T{2}, max) {
            if(is_prime[p]) {
                for(T i = mul_overflow(p,p).value_or(max+1); i<=max; i+=p) is_prime[i] = false;
                this->emplace_back(p);
            }
        }
    }
    inline bool operator()(const T index) const noexcept(NO_EXCEPT) {
        return is_prime[index];
    }
};


} // namespace lib
