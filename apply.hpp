#include <utility>
#include <algorithm>

namespace Internal {
    template<class T> T max(T a, T b) { return std::max(a, b); }
    template<class T> T min(T a, T b) { return std::min(a, b); }
}

template<class T, T (*op)(T,T)> struct Apply {
    Apply(T&& v) : _v(v) {}
    template<class U> Apply& operator<<=(U&& val) & noexcept {
        _v = op(_v, std::forward<U>(val));
        return *this;
    }
    T val() const { return _v; }

  private:
    T _v;
};

template<class T> using Max = Apply<T,Internal::max<T>>;
template<class T> using Min = Apply<T,Internal::min<T>>;
