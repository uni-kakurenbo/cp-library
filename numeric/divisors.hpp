#pragma once

namespace lib {

template<class T> struct divisors : std::vector<T> {
    divisors() {}
    divisors(const T k) {
        this->clear();

        for(T i=1; i*i<=k; ++i) {
            if(k%i == 0) {
                this->emplace_back(i);
                if(i*i < k) this->emplace_back(k/i);
            }
        }
    }
};

} // namespace lib
