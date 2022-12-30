#pragma once

namespace lib {

namespace Monoids {

template<class T> struct IMonoid {
    virtual IMonoid& operator+=(const IMonoid&) = 0;
    inline IMonoid operator+(const IMonoid& other) const {
        auto res = *this;
        return res += other;
    }
};


}

}
