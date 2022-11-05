#pragma once

#include <utility>
#include <algorithm>

namespace Internal {
    template<class T> T max(T a, T b) { return std::max(a, b); }
    template<class T> T min(T a, T b) { return std::min(a, b); }
}

template<class T, T (*op)(T,T)> struct Applier {
  protected:
    T _v;

  public:
    Applier(T&& v = T{}) : _v(v) {}
    template<class U> Applier& operator<<=(U&& val) & noexcept {
        _v = op(_v, std::forward<U>(val));
        return *this;
    }
    inline T val() const { return _v; }
};

template<class T> using Max = Applier<T,Internal::max<T>>;
template<class T> using Min = Applier<T,Internal::min<T>>;
