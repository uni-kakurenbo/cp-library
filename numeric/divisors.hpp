#pragma once

namespace Lib {

template<class T> struct Divisors : std::vector<T> {
    Divisors() {}
    Divisors(const T k) {
        this->clear();

        for(T i=1; i*i<=k; ++i) {
            if(k%i == 0) {
                this->emplace_back(i);
                if(i*i < k) this->emplace_back(k/i);
            }
        }
    }
};

} // namespace Lib
