#pragma once


#include <functional>
#include <utility>
#include <type_traits>


#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/limits.hpp"


namespace uni {

namespace internal {

namespace boundary_seeker_impl {


template<class T>
struct integal {
  protected:
    std::function<bool(T)> _validate;

  public:
    integal(std::function<bool(T)> validate) noexcept(NO_EXCEPT) : _validate(validate) {}

    template<bool INVERT = false>
    T bound(const T _ok, const T _ng) const noexcept(NO_EXCEPT) {
        T ok = _ok, ng = _ng;
        if constexpr(INVERT) std::swap(ng, ok);

        while(std::abs(ok - ng) > 1) {
            T mid = ng + (ok - ng) / 2;
            (INVERT ^ this->_validate(mid) ? ok : ng) = mid;
        }

        return ok;
    }

    template<bool REVERSE = false, bool INVERT = false>
    T bound(const T ok) const noexcept(NO_EXCEPT) {
        assert(INVERT ^ this->_validate(ok));

        T ng = REVERSE ? -1 : 1;
        while(INVERT ^ this->_validate(ok + ng)) ng += ng;
        return this->bound<INVERT>(ok, ok + ng);
    }

    template<bool REVERSE = false, bool INVERT = false>
    T bound() const noexcept(NO_EXCEPT) {
        return this->bound<INVERT>(
            REVERSE ?
            numeric_limits<T>::arithmetic_infinity() / 2 :
            numeric_limits<T>::arithmetic_negative_infinity() / 2
        );
    }

    template<bool INVERT = false>
    T bound_or(const T ok, const T ng, const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<INVERT>(ok, ng);
        return this->_validate(res) ? res : proxy;
    }

    template<bool REVERSE = false, bool INVERT = false>
    T bound_or(const T ok, const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<REVERSE, INVERT>(ok);
        return this->_validate(res) ? res : proxy;
    }

    template<bool REVERSE = false, bool INVERT = false>
    T bound_or(const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<REVERSE, INVERT>();
        return this->_validate(res) ? res : proxy;
    }
};

template<class T>
struct floating_point {
  protected:
    std::function<bool(T)> _validate;

  public:
    floating_point(std::function<bool(T)> validate) noexcept(NO_EXCEPT) : _validate(validate) {}

    template<bool INVERT = false, internal::size_t ITERATIONS = 200>
    T bound(const T _ok, const T _ng) const noexcept(NO_EXCEPT) {
        T ok = _ok, ng = _ng;
        if constexpr(INVERT) std::swap(ng, ok);

        REP(ITERATIONS) {
            T mid = ng + (ok - ng) / 2;
            (INVERT ^ this->_validate(mid) ? ok : ng) = mid;
        }
        return ok;
    }

    template<bool REVERSE = false, bool INVERT = false, internal::size_t ITERATIONS = 200>
    T bound(const T ok) const noexcept(NO_EXCEPT) {
        assert(INVERT ^ this->_validate(ok));

        T ng = REVERSE ? -1 : 1;
        while(INVERT ^ this->_validate(ok + ng)) ng += ng;
        return this->bound<INVERT>(ok, ok + ng);
    }

    template<bool REVERSE = false, bool INVERT = false, internal::size_t ITERATIONS = 200>
    T bound() const noexcept(NO_EXCEPT) {
        return this->bound<INVERT, ITERATIONS>(
            REVERSE ?
            numeric_limits<T>::arithmetic_infinity() / 2 :
            numeric_limits<T>::arithmetic_negative_infinity() / 2
        );
    }

    template<bool INVERT = false, internal::size_t ITERATIONS = 200>
    T bound_or(const T ok, const T ng, const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<INVERT, ITERATIONS>(ok, ng);
        return this->_validate(res) ? res : proxy;
    }

    template<bool REVERSE = false, bool INVERT = false, internal::size_t ITERATIONS = 200>
    T bound_or(const T ok, const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<REVERSE, INVERT, ITERATIONS>(ok);
        return this->_validate(res) ? res : proxy;
    }

    template<bool REVERSE = false, bool INVERT = false, internal::size_t ITERATIONS = 200>
    T bound_or(const T proxy) const noexcept(NO_EXCEPT) {
        const T res = this->bound<REVERSE, INVERT, ITERATIONS>();
        return this->_validate(res) ? res : proxy;
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


} // namespace uni
