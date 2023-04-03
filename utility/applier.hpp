#pragma once

#include <utility>
#include <algorithm>

namespace lib {

namespace internal {
    template<class T> T max(T a, T b) { return std::max(a, b); }
    template<class T> T max(T x) { return x; }
    template<class T> T max() { return  -std::numeric_limits<T>::min(); }

    template<class T> T min(T a, T b) { return std::min(a, b); }
    template<class T> T min(T x) { return x; }
    template<class T> T min() { return  std::numeric_limits<T>::max(); }
}

template<class T, T (*op)(T,T)> struct applier {
    using value_type = T;

  protected:
    T _v;

  public:
    applier(const T& v = T{}) : _v(v) {}
    template<class U> applier& operator<<=(U&& val) & noexcept {
        _v = op(_v, std::forward<U>(val));
        return *this;
    }
    inline T val() const { return _v; }
    inline operator T() const { return _v; }
};

template<class T> using maximum = applier<T,internal::max<T>>;
template<class T> using minimum = applier<T,internal::min<T>>;

}
