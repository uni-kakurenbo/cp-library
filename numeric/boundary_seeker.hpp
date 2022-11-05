#pragma once

#include <functional>
#include <utility>

#include "internal/develop/dev_assert.hpp"

template<class T> struct BoundarySeeker {
  protected:
    std::function<bool(T)> validate;

  public:
    BoundarySeeker(std::function<bool(T)> validate) : validate(validate) {}

    template<const bool REVERSE = false>
    T bound(const T _ok, const T _ng) const {
        T ok = _ok, ng = _ng;
        if constexpr(REVERSE) std::swap(ng, ok);
        while(abs(ok-ng) > 1) {
            T mid = ng + (ok - ng) / 2;
            (this->validate(mid) ? ok : ng) = mid;
        }
        return ok;
    }

    template<const bool REVERSE = false>
    T bound(const T ok) const {
        T ng = ok;
        while(this->validate(ng)) ng += REVERSE ? -ng : ng;
        return this->bound(ok, ng);
    }

    template<const bool REVERSE = false>
    T bound_or(const T ok, const T ng, const T proxy) const {
        const T res = this->bound<REVERSE>(ok, ng);
        return this->validate(res) ? res : proxy;
    }

    template<const bool REVERSE = false>
    T bound_or(const T ok, const T proxy) const {
        const T res = this->bound<REVERSE>(ok);
        return this->validate(res) ? res : proxy;
    }
};
