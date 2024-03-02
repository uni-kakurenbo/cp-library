#pragma once


#include <functional>
#include <utility>
#include <type_traits>


#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/limits.hpp"


namespace lib {

namespace internal {

namespace boundary_seeker_impl {


template<class T>
struct integal {
  protected:
    std::function<bool(T)> validate;

  public:
    integal(std::function<bool(T)> validate) noexcept(NO_EXCEPT) : validate(validate) {}

    template<const bool REVERSE = false>
    T bound(const T _ok, const T _ng) const noexcept(NO_EXCEPT) {
        T ok = _ok, ng = _ng;
        if constexpr(REVERSE) std::swap(ng, ok);
        while(std::abs(ok-ng) > 1) {
            T mid = ng + (ok - ng) / 2;
            (this->validate(mid) ? ok : ng) = mid;
        }
        return ok;
    }

    template<const bool INVERT = false>
    T bound(const T ok) const noexcept(NO_EXCEPT) {
        T ng = INVERT ? -1 : 1;
        while(this->validate(ok + ng)) ng += ng;
        return this->bound(ok, ok + ng);
    }

    template<const bool REVERSE = false>
    T bound() const noexcept(NO_EXCEPT) {
        return this->bound<REVERSE>(
            REVERSE ?
            numeric_limits<T>::arithmetic_infinity():
            numeric_limits<T>::arithmetic_negative_infinity()
        );
    }

    template<const bool REVERSE = false>
    T bound_or(const T ok, const T ng, const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<REVERSE>(ok, ng);
        return this->validate(res) ? res : proxy;
    }

    template<const bool INVERT = false>
    T bound_or(const T ok, const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<INVERT>(ok);
        return this->validate(res) ? res : proxy;
    }

    template<const bool REVERSE = false>
    T bound_or(const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<REVERSE>();
        return this->validate(res) ? res : proxy;
    }
};

template<class T>
struct floating_point {
  protected:
    std::function<bool(T)> validate;

  public:
    floating_point(std::function<bool(T)> validate) noexcept(NO_EXCEPT) : validate(validate) {}

    template<const bool REVERSE = false, const internal::size_t ITERATIONS = 200>
    T bound(const T _ok, const T _ng) const noexcept(NO_EXCEPT) {
        T ok = _ok, ng = _ng;
        if constexpr(REVERSE) std::swap(ng, ok);
        REP(ITERATIONS) {
            T mid = ng + (ok - ng) / 2;
            (this->validate(mid) ? ok : ng) = mid;
        }
        return ok;
    }

    template<const bool REVERSE = false, const internal::size_t ITERATIONS = 200>
    T bound(const T ok) const noexcept(NO_EXCEPT) {
        T ng = ok + REVERSE ? -1 : 1;
        while(this->validate(ng)) ng += REVERSE ? -ng : ng;
        return this->bound<false,ITERATIONS>(ok, ng);
    }

    template<const bool REVERSE = false, const internal::size_t ITERATIONS = 200>
    T bound() const noexcept(NO_EXCEPT) {
        return this->bound<REVERSE,ITERATIONS>(
            REVERSE ?
            numeric_limits<T>::arithmetic_infinity() :
            numeric_limits<T>::arithmetic_negative_infinity()
        );
    }

    template<const bool REVERSE = false, const internal::size_t ITERATIONS = 200>
    T bound_or(const T ok, const T ng, const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<REVERSE,ITERATIONS>(ok, ng);
        return this->validate(res) ? res : proxy;
    }

    template<const bool REVERSE = false, const internal::size_t ITERATIONS = 200>
    T bound_or(const T ok, const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<REVERSE,ITERATIONS>(ok);
        return this->validate(res) ? res : proxy;
    }

    template<const bool REVERSE = false, const internal::size_t ITERATIONS = 200>
    T bound_or(const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<REVERSE,ITERATIONS>();
        return this->validate(res) ? res : proxy;
    }
};

template<class, bool>
struct seeker {};

template<class T>
struct seeker<T,true> : integal<T> {
    using integal<T>::integal;
};

template<class T>
struct seeker<T,false> : floating_point<T> {
    using floating_point<T>::floating_point;
};


} // namespace boundary_seeker_impl

} // namespace internal


template<class T>
using boundary_seeker = internal::boundary_seeker_impl::seeker<T,std::is_integral_v<T>>;


} // namespace lib
