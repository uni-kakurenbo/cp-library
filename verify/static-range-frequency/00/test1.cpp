#include <bits/stdc++.h>
/* #expanded [template.hpp] */
/* #expanded [snippet/aliases.hpp] */
#include <cstdint>
#include <utility>
#include <vector>
/* #expanded [internal/dev_env.hpp] */
#ifdef LOCAL_JUDGE
constexpr bool DEV_ENV = true;
constexpr bool NO_EXCEPT = false;
#else
constexpr bool DEV_ENV = false;
constexpr bool NO_EXCEPT = true;
#endif
/* [internal/dev_env.hpp] */
/* #expanded [snippet/internal/types.hpp] */
#include <cstdint>
namespace lib {
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;
#ifdef __GNUC__
using i128 = __int128_t;
using u128 = __uint128_t;
#endif
using uint = unsigned;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
} // namespace lib
/* [snippet/internal/types.hpp] */
#define until(...) while(!(__VA_ARGS__))
#define ALL(x) std::begin((x)), std::end((x))
#define RALL(x) std::rbegin((x)), std::rend((x))
#define $F first
#define $S second
namespace lib {
template <class T> using spair = std::pair<T, T>;
}
namespace std {
using bit_reference = std::vector<bool>::reference;
bit_reference operator|=(bit_reference a, const bool b) noexcept(NO_EXCEPT) {
    return a = a | b;
}
bit_reference operator&=(bit_reference a, const bool b) noexcept(NO_EXCEPT) {
    return a = a & b;
}
} // namespace std
/* [snippet/aliases.hpp] */
/* #expanded [snippet/iterations.hpp] */
/* #expanded [snippet/internal/overload.hpp] */
#define $OVERLOAD2(arg0, arg1, cmd, ...) cmd
#define $OVERLOAD3(arg0, arg1, arg2, cmd, ...) cmd
#define $OVERLOAD4(arg0, arg1, arg2, arg3, cmd, ...) cmd
#define $OVERLOAD5(arg0, arg1, arg2, arg3, arg4, cmd, ...) cmd
/* [snippet/internal/overload.hpp] */
#define LOOP(n) REPI($_, (n))
#define REPI(i, n) for(int i = 0, i##_length = (n); i < i##_length; ++i)
#define REPF(i, l, r) for(auto i = (l), i##_last = (r); i < i##_last; ++i)
#define REPIS(i, l, r, s)                                                      \
    for(auto i = (l), i##_last = (r); i < i##_last; i += (s))
#define REPR(i, n) for(auto i = (n); --i >= 0;)
#define REPB(i, l, r) for(auto i = (r), i##_last = (l); --i >= i##_last;)
#define REPRS(i, l, r, s)                                                      \
    for(auto i = (r), i##_last = (l); (i -= (s)) >= i##_last;)
#define REP(...) $OVERLOAD4(__VA_ARGS__, REPIS, REPF, REPI, LOOP)(__VA_ARGS__)
#define REPD(...) $OVERLOAD4(__VA_ARGS__, REPRS, REPB, REPR)(__VA_ARGS__)
#define FORO(i, n) for(int i = 0, i##_last = (n); i <= i##_last; ++i)
#define FORI(i, l, r) for(auto i = (l), i##_last = (r); i <= i##_last; ++i)
#define FORIS(i, l, r, s)                                                      \
    for(auto i = (l), i##_last = (r); i <= i##_last; i += (s))
#define FORRO(i, n) for(auto i = (n); i >= 0; --i)
#define FORR(i, l, r) for(auto i = (r), i##_last = (l); i >= i##_last; --i)
#define FORRS(i, l, r, s)                                                      \
    for(auto i = (r), i##_last = (l); i >= i##_last; i -= (s))
#define FOR(...) $OVERLOAD4(__VA_ARGS__, FORIS, FORI, FORO)(__VA_ARGS__)
#define FORD(...) $OVERLOAD4(__VA_ARGS__, FORRS, FORR, FORRO)(__VA_ARGS__)
#define ITR1(e0, v) for(const auto &e0 : (v))
#define ITRP1(e0, v) for(auto e0 : (v))
#define ITRR1(e0, v) for(auto &e0 : (v))
#define ITR2(e0, e1, v) for(const auto [e0, e1] : (v))
#define ITRP2(e0, e1, v) for(auto [e0, e1] : (v))
#define ITRR2(e0, e1, v) for(auto &[e0, e1] : (v))
#define ITR3(e0, e1, e2, v) for(const auto [e0, e1, e2] : (v))
#define ITRP3(e0, e1, e2, v) for(auto [e0, e1, e2] : (v))
#define ITRR3(e0, e1, e2, v) for(auto &[e0, e1, e2] : (v))
#define ITR4(e0, e1, e2, e3, v) for(const auto [e0, e1, e2, e3] : (v))
#define ITRP4(e0, e1, e2, e3, v) for(auto [e0, e1, e2, e3] : (v))
#define ITRR4(e0, e1, e2, e3, v) for(auto &[e0, e1, e2, e3] : (v))
#define ITR(...) $OVERLOAD5(__VA_ARGS__, ITR4, ITR3, ITR2, ITR1)(__VA_ARGS__)
#define ITRP(...)                                                              \
    $OVERLOAD5(__VA_ARGS__, ITRP4, ITRP3, ITRP2, ITRP1)(__VA_ARGS__)
#define ITRR(...)                                                              \
    $OVERLOAD5(__VA_ARGS__, ITRR4, ITRR3, ITRR2, ITRR1)(__VA_ARGS__)
/* [snippet/iterations.hpp] */
/* #expanded [snippet/fast_io.hpp] */
#include <iostream>
#ifdef __GNUC__
__attribute__((constructor)) inline void fast_io() noexcept(NO_EXCEPT) {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr);
}
#else
inline void fast_io() noexcept(NO_EXCEPT) {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr);
}
#endif
/* [snippet/fast_io.hpp] */
/* #expanded [snippet/using.hpp] */
#include <algorithm>
#include <bitset>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
/* #expanded [utilities.hpp] */
/* #expanded [numeric/int128.hpp] */
#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <string>
template <class C, class S>
std::basic_istream<C, S> &operator>>(std::basic_istream<C, S> &in,
                                     lib::i128 &v) noexcept(NO_EXCEPT) {
    std::string str;
    in >> str;
    v = 0;
    bool negative = (str[0] == '-');
    REP(d, std::next(str.begin(), negative), str.end()) {
        assert(std::isdigit(*d));
        v = v * 10 + *d - '0';
    }
    if(negative)
        v *= -1;
    return in;
}
template <class C, class S>
std::basic_ostream<C, S> &operator<<(std::basic_ostream<C, S> &out,
                                     lib::i128 v) noexcept(NO_EXCEPT) {
    if(v == 0)
        return out << 0;
    if(v < 0)
        out << '-', v *= -1;
    std::string str;
    while(v > 0)
        str += v % 10 + '0', v /= 10;
    std::reverse(str.begin(), str.end());
    return out << str;
}
/* [numeric/int128.hpp] */
/* #expanded [geometry.hpp] */
/* #expanded [geometry/basic.hpp] */
/* #expanded [geometry/point.hpp] */
#include <cmath>
#include <complex>
#include <iostream>
#include <utility>
namespace lib {
template <class T> struct point : protected std::pair<T, T> {
  public:
    constexpr point() {}
    constexpr point(const T &x, const T &y) noexcept(NO_EXCEPT)
        : std::pair<T, T>(x, y) {}
    template <class U>
    constexpr point(const point<U> &p) noexcept(NO_EXCEPT)
        : point(p.x(), p.y()){};
    template <class U>
    constexpr point(point<U> &&p) noexcept(NO_EXCEPT) : point(p.x(), p.y()){};
    template <class U>
    constexpr point &operator=(const point<U> &p) & noexcept(NO_EXCEPT) {
        this->x() = p.x(), this->y() = p.y();
        return *this;
    };
    template <class U>
    constexpr point &operator=(point<U> &&p) && noexcept(NO_EXCEPT) {
        this->x() = p.x(), this->y() = p.y();
        return *this;
    };
    inline T &x() noexcept(NO_EXCEPT) { return this->first; }
    inline T &y() noexcept(NO_EXCEPT) { return this->second; }
    inline const T &x() const noexcept(NO_EXCEPT) { return this->first; }
    inline const T &y() const noexcept(NO_EXCEPT) { return this->second; }
    inline constexpr point &operator+=(const point &v) noexcept(NO_EXCEPT) {
        this->x() += v.x(), this->y() += v.y();
        return *this;
    }
    inline constexpr point &operator-=(const point &v) noexcept(NO_EXCEPT) {
        this->x() -= v.x(), this->y() -= v.y();
        return *this;
    }
    friend inline constexpr point
    operator+(point a, const point &b) noexcept(NO_EXCEPT) {
        return a += b;
    }
    friend inline constexpr point
    operator-(point a, const point &b) noexcept(NO_EXCEPT) {
        return a -= b;
    }
    friend inline constexpr point
    operator*(const point &a, const point &b) noexcept(NO_EXCEPT) {
        return a.x() * b.x() + a.y() * b.y();
    }
    friend inline constexpr bool
    operator==(const point &a, const point &b) noexcept(NO_EXCEPT) {
        return a.x() == b.x() && a.y() == b.y();
    }
    friend inline constexpr bool
    operator!=(const point &a, const point &b) noexcept(NO_EXCEPT) {
        return a.x() != b.x() or a.y() != b.y();
    }
    friend inline constexpr bool operator<(const point &a,
                                           const point &b) noexcept(NO_EXCEPT) {
        return a.x() != b.x() ? a.x() < b.x() : a.y() < b.y();
    }
    friend inline constexpr bool operator>(const point &a,
                                           const point &b) noexcept(NO_EXCEPT) {
        return a.x() != b.x() ? a.x() > b.x() : a.y() > b.y();
    }
    friend inline constexpr bool
    operator<=(const point &a, const point &b) noexcept(NO_EXCEPT) {
        return !(a > b);
    }
    friend inline constexpr bool
    operator>=(const point &a, const point &b) noexcept(NO_EXCEPT) {
        return !(a < b);
    }
    std::pair<T, T> _debug() const noexcept(NO_EXCEPT) {
        return {this->x(), this->y()};
    }
};
} // namespace lib
template <class T> inline constexpr T std::abs(const lib::point<T> &v) {
    T x = v.x(), y = v.y(), n = std::max(std::abs(x), std::abs(y));
    if(n == 0)
        return 0;
    x /= n, y /= n;
    return n * std::sqrt(x * x + y * y);
}
namespace lib {
template <class U, class T>
inline constexpr U distance(const point<T> &a, const point<T> &b) {
    return std::abs(point<U>(a - b));
}
template <class T>
inline constexpr T cross(point<T> a, point<T> b, const point<T> &o = {}) {
    a -= o, b -= o;
    return a.x() * b.y() - a.y() * b.x();
}
} // namespace lib
template <class T, class C, class S>
std::basic_istream<C, S> &operator>>(std::basic_istream<C, S> &in,
                                     lib::point<T> &v) {
    T x, y;
    in >> x >> y;
    v = {x, y};
    return in;
}
/* [geometry/point.hpp] */
/* [geometry/basic.hpp] */
/* [geometry.hpp] */
/* #expanded [numeric/arithmetic.hpp] */
#include <algorithm>
#include <atcoder/math.hpp>
#include <cassert>
#include <cstring>
#include <string>
/* #expanded [internal/types.hpp] */
#include <cstdint>
namespace lib {
namespace internal {
using size_t = std::int32_t;
using int128_t = __int128_t;
using uint128_t = __uint128_t;
} // namespace internal
} // namespace lib
/* [internal/types.hpp] */
/* #expanded [numeric/internal/number_base.hpp] */
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
namespace lib {
template <std::size_t B = 2, class T>
std::string base_n_string(T v) noexcept(NO_EXCEPT) {
    constexpr char CHARS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static_assert(0 < B and B <= std::strlen(CHARS));
    assert(0 <= v);
    std::string res;
    while(v > 0) {
        res += CHARS[v % B];
        v /= B;
    }
    std::reverse(ALL(res));
    return res;
}
template <class T>
std::string base_n_string(T v, std::size_t b) noexcept(NO_EXCEPT) {
    constexpr char CHARS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    assert(1 < b && b <= std::strlen(CHARS));
    assert(0 <= v);
    std::string res;
    while(v > 0) {
        res += CHARS[v % b];
        v /= b;
    }
    std::reverse(ALL(res));
    return res;
}
template <class T>
std::vector<T> base_n_vector(T v, std::size_t b = 2) noexcept(NO_EXCEPT) {
    assert(1 < b);
    assert(0 <= v);
    std::vector<T> res;
    while(v > 0) {
        res.push_back(v % b);
        v /= b;
    }
    std::reverse(ALL(res));
    return res;
}
} // namespace lib
/* [numeric/internal/number_base.hpp] */
namespace lib {
template <class T> T sqrt_floor(const T x) noexcept(NO_EXCEPT) {
    T ok = 0, ng = x / 2 + 2;
    while(ng - ok > 1) {
        T mid = (ok + ng) / 2;
        (x / mid < mid ? ng : ok) = mid;
    }
    return ok;
}
template <class T> T sqrt_ceil(const T x) noexcept(NO_EXCEPT) {
    T ok = 0, ng = x / 2 + 2;
    while(ng - ok > 1) {
        T mid = (ok + ng) / 2;
        (x <= (mid - 1) * (mid - 1) ? ng : ok) = mid;
    }
    return ok;
}
template <class T> T nPr(const T n, const T r) noexcept(NO_EXCEPT) {
    assert(0 <= n);
    assert(0 <= r);
    if(n < r)
        return 0;
    T res = 1;
    REP(i, r) res *= n - i;
    return res;
}
template <class T> T nCr(const T n, const T r) noexcept(NO_EXCEPT) {
    assert(0 <= n);
    assert(0 <= r);
    if(n == r)
        return 1;
    if(n < r)
        return 0;
    if(n < r * 2)
        r = n - r;
    T p = 1, q = 1;
    REP(i, r) p *= n - i, q *= r - i;
    return p / q;
}
template <class T, class U> T pow(T x, U n) noexcept(NO_EXCEPT) {
    T res = 1;
    while(n > 0) {
        if(n & 1)
            res *= x;
        x *= x;
        n >>= 1;
    }
    return res;
}
using atcoder::crt;
using atcoder::inv_mod;
using atcoder::pow_mod;
template <class T, class U, class V>
inline bool mul_over(const T x, const U y, const V s) noexcept(NO_EXCEPT) {
    if(x >= s)
        return true;
    if(x >= 0 && y >= 0)
        return x > s / y;
    if(x <= 0 && y <= 0)
        return x < s / y;
    return false;
}
template <class T, class U, class V>
inline bool mul_under(const T x, const U y, const V s) noexcept(NO_EXCEPT) {
    if(x <= s)
        return true;
    if(x >= 0 && y <= 0)
        return x > s / y;
    if(x <= 0 && y >= 0)
        return x < s / y;
    return false;
}
} // namespace lib
/* [numeric/arithmetic.hpp] */
/* #expanded [numeric/matrix.hpp] */
#include <cassert>
#include <vector>
/* #expanded [grid.hpp] */
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
namespace lib {
namespace internal {
namespace grid_impl {
template <class T> struct interface {};
template <class T> struct container_base : virtual interface<T> {
  private:
    size_t _h, _w;

  protected:
    inline void _validate_index(__attribute__((unused)) const size_t i,
                                __attribute__((unused)) const size_t j) const
        noexcept(NO_EXCEPT) {
        assert(0 <= i and i < this->height());
        assert(0 <= j and j < this->width());
    }
    inline size_t _positivize_row_index(const size_t x) const
        noexcept(NO_EXCEPT) {
        return x < 0 ? this->height() + x : x;
    }
    inline size_t _positivize_col_index(const size_t x) const
        noexcept(NO_EXCEPT) {
        return x < 0 ? this->width() + x : x;
    }

  public:
    container_base() noexcept(NO_EXCEPT) = default;
    container_base(const size_t _h, const size_t _w) noexcept(NO_EXCEPT)
        : _h(_h), _w(_w) {}
    virtual void resize(const size_t h,
                        const size_t w) noexcept(NO_EXCEPT) /*override*/ {
        this->_h = h, this->_w = w;
    }
    inline size_t height() const noexcept(NO_EXCEPT) /*override*/ {
        return this->_h;
    }
    inline size_t width() const noexcept(NO_EXCEPT) /*override*/ {
        return this->_w;
    }
    inline size_t id(const size_t i, const size_t j) const
        noexcept(NO_EXCEPT) /*override*/ {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return _i * this->width() + _j;
    }
};
template <class T, class Row = std::vector<T>, class base = std::vector<Row>>
struct container : base, container_base<T>, virtual interface<T> {
    container(const size_t n = 0) noexcept(NO_EXCEPT) : container(n, n) {}
    container(const size_t h, const size_t w,
              const T &val = T{}) noexcept(NO_EXCEPT)
        : base(h, Row(w, val)), container_base<T>(h, w) {}
    container(const std::initializer_list<Row> init_list) noexcept(NO_EXCEPT)
        : base(init_list) {
        const size_t rows = std::distance(ALL(init_list));
        const size_t first_cols = init_list.begin()->size();
        if constexpr(DEV_ENV) {
            ITR(init_row, init_list)
            assert((size_t)init_row.size() == first_cols);
        }
        this->container_base<T>::resize(rows, first_cols);
    }
    inline void assign(const container &source) noexcept(NO_EXCEPT) {
        this->resize(source.height(), source.width());
        this->base::assign(ALL(source));
    }
    inline void assign(const size_t h, const size_t w,
                       const T &val = T{}) noexcept(NO_EXCEPT) /*override*/ {
        this->container_base<T>::resize(h, w);
        this->base::resize(h);
        ITRR(row, *this) row.assign(w, val);
    }
    inline void resize(const size_t h,
                       const size_t w) noexcept(NO_EXCEPT) /*override*/ {
        this->container_base<T>::resize(h, w);
        this->base::resize(h);
        ITRR(row, *this) row.resize(w);
    }
    inline T &operator()(const size_t i,
                         const size_t j) noexcept(NO_EXCEPT) /*override*/ {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }
    inline const T &operator()(const size_t i, const size_t j) const
        noexcept(NO_EXCEPT) /*override*/ {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }
};
template <class T, class base = std::vector<T>>
struct unfolded_container : base, container_base<T>, virtual interface<T> {
    unfolded_container(size_t n = 0) noexcept(NO_EXCEPT)
        : unfolded_container(n, n) {}
    unfolded_container(const size_t h, const size_t w,
                       const T &val = T{}) noexcept(NO_EXCEPT)
        : base(h * w, val), container_base<T>(h, w) {}
    unfolded_container(std::initializer_list<std::initializer_list<T>>
                           init_list) noexcept(NO_EXCEPT) {
        const size_t rows = std::distance(init_list.begin(), init_list.end());
        const size_t first_cols = init_list.begin()->size();
        this->resize(rows, first_cols);
        for(auto index = 0, itr = init_list.begin(), itr_end = init_list.end();
            itr != itr_end; ++itr) {
            assert((size_t)itr->size() == first_cols);
            for(auto v = itr->begin(), v_end = itr->end(); v != v_end; ++v)
                (*this)[index++] = *v;
        }
    }
    inline void assign(const unfolded_container &source) noexcept(NO_EXCEPT) {
        this->resize(source.height(), source.width());
        this->base::assign(ALL(source));
    }
    inline void assign(const size_t h, const size_t w,
                       const T &val = T{}) noexcept(NO_EXCEPT) /*override*/ {
        this->container_base<T>::resize(h, w);
        this->base::assign(h * w, val);
    }
    inline void resize(const size_t h,
                       const size_t w) noexcept(NO_EXCEPT) /*override*/ {
        this->container_base<T>::resize(h, w);
        this->base::resize(h * w);
    }
    inline T &operator()(const size_t i,
                         const size_t j) noexcept(NO_EXCEPT) /*override*/ {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        return (*this)[this->id(_i, _j)];
    }
    inline const T &operator()(const size_t i, const size_t j) const
        noexcept(NO_EXCEPT) /*override*/ {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        return (*this)[this->id(_i, _j)];
    }
};
} // namespace grid_impl
template <class T, class container>
struct grid_core : container, virtual grid_impl::interface<T> {
    using container::container;
    enum class invert_direction { vertical, horizontal };
    enum class rotate_direction { counter_clockwise, clockwise };
    template <class U = T, class Stream = std::istream>
    void inline read(Stream *const ist = &std::cin) noexcept(NO_EXCEPT) {
        REP(i, this->height()) REP(j, this->width()) {
            U val;
            *ist >> val;
            (*this)(i, j) = val;
        }
    }
    template <invert_direction DIRECTION = invert_direction::vertical>
    inline grid_core &invert() noexcept(NO_EXCEPT) {
        grid_core res(this->height(), this->width());
        REP(i, this->height()) REP(j, this->width()) {
            if constexpr(DIRECTION == invert_direction::vertical) {
                res(i, j) = (*this)(this->height() - i - 1, j);
            } else {
                res(i, j) = (*this)(i, this->width() - j - 1);
            }
        }
        this->assign(res);
        return *this;
    }
    template <rotate_direction DIRECTION = rotate_direction::clockwise>
    inline grid_core &rotate(const size_t k) noexcept(NO_EXCEPT) {
        grid_core res = *this;
        REP(i, k) { res = res.rotate<DIRECTION>(); }
        this->assign(res);
        return *this;
    }
    template <rotate_direction DIRECTION = rotate_direction::clockwise>
    inline grid_core &rotate() noexcept(NO_EXCEPT) {
        grid_core res(this->width(), this->height());
        REP(i, this->width()) REP(j, this->height()) {
            if constexpr(DIRECTION == rotate_direction::clockwise) {
                res(i, j) = (*this)(this->height() - j - 1, i);
            } else {
                res(i, j) = (*this)(j, this->width() - i - 1);
            }
        }
        this->assign(res);
        return *this;
    }
    inline grid_core &transpose() noexcept(NO_EXCEPT) {
        grid_core res(this->width(), this->height());
        REP(i, this->width()) REP(j, this->height()) {
            res(i, j) = (*this)(j, i);
        }
        this->assign(res);
        return *this;
    }
};
} // namespace internal
template <class T, class Row = std::vector<T>, class base = std::vector<Row>>
using grid =
    internal::grid_core<T, internal::grid_impl::container<T, Row, base>>;
template <class T, class base = std::vector<T>>
using unfolded_grid =
    internal::grid_core<T, internal::grid_impl::unfolded_container<T, base>>;
} // namespace lib
/* [grid.hpp] */
/* #expanded [adapter/valarray.hpp] */
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <valarray>
namespace lib {
template <class T> struct valarray : std::valarray<T> {
    using size_type = internal::size_t;
    using iterator = T *;
    using const_iterator = const T *;

  protected:
    inline bool
    _validate_index_in_right_open([[maybe_unused]] const size_type p) const
        noexcept(NO_EXCEPT) {
        return 0 <= p and p < this->size();
    }
    inline bool
    _validate_index_in_closed([[maybe_unused]] const size_type p) const
        noexcept(NO_EXCEPT) {
        return 0 <= p and p <= this->size();
    }
    inline bool
    _validate_rigth_open_interval([[maybe_unused]] const size_type l,
                                  [[maybe_unused]] const size_type r) const
        noexcept(NO_EXCEPT) {
        return 0 <= l and l <= r and r <= this->size();
    }
    inline size_type _positivize_index(const size_type p) const
        noexcept(NO_EXCEPT) {
        return p < 0 ? this->size() + p : p;
    }

  public:
    valarray() noexcept(NO_EXCEPT) {}
    valarray(const size_type length, const T &val = T{}) noexcept(NO_EXCEPT)
        : std::valarray<T>(std::forward<const T>(val), length) {}
    template <class I, typename std::iterator_traits<I>::value_type * = nullptr>
    valarray(const I first, const I last) noexcept(NO_EXCEPT)
        : std::valarray<T>(first, last) {}
    template <class U>
    valarray(const std::initializer_list<U> &init) noexcept(NO_EXCEPT)
        : valarray(std::begin(init), std::end(init)) {}
    inline size_type size() const noexcept(NO_EXCEPT) {
        return this->std::valarray<T>::size();
    }
    inline void reserve(const size_type) noexcept(NO_EXCEPT) { /* do nothing */
    }
    template <class I, typename std::iterator_traits<I>::value_type * = nullptr>
    inline void assign(const I first, const I last) noexcept(NO_EXCEPT) {
        this->resize(std::distance(first, last));
        std::copy(first, last, begin(*this));
    }
    inline void assign(const size_type length,
                       const T &val = T{}) noexcept(NO_EXCEPT) {
        this->std::valarray<T>::resize(length, val);
    }
    inline void resize(const size_type length,
                       const T &val = T{}) noexcept(NO_EXCEPT) {
        std::valarray<T> temp = *this;
        this->assign(length, val);
        std::move(std::begin(temp),
                  std::min(std::end(temp), std::next(std::begin(temp), length)),
                  std::begin(*this));
    }
    inline const T &operator[](size_type pos) const noexcept(NO_EXCEPT) {
        pos = this->_positivize_index(pos),
        assert(this->_validate_index_in_right_open(pos));
        return this->std::valarray<T>::operator[](pos);
    }
    inline T &operator[](size_type pos) noexcept(NO_EXCEPT) {
        pos = this->_positivize_index(pos),
        assert(this->_validate_index_in_right_open(pos));
        return this->std::valarray<T>::operator[](pos);
    }
    inline const T &back() const noexcept(NO_EXCEPT) {
        return *std::prev(this->end());
    }
    inline T &back() { return *std::prev(this->end()); }
    inline const T &front() const noexcept(NO_EXCEPT) { return *this->begin(); }
    inline T &front() { return *this->begin(); }
    inline const T *begin() const noexcept(NO_EXCEPT) {
        return this->size() ? std::addressof((*this)[0]) : nullptr;
    }
    inline T *begin() {
        return this->size() ? std::addressof((*this)[0]) : nullptr;
    }
    inline const T *end() const noexcept(NO_EXCEPT) {
        if(auto n = this->size()) {
            return std::addressof((*this)[0]) + n;
        } else {
            return nullptr;
        }
    }
    inline T *end() {
        if(auto n = this->size()) {
            return std::addressof((*this)[0]) + n;
        } else {
            return nullptr;
        }
    }
};
} // namespace lib
/* [adapter/valarray.hpp] */
namespace lib {
namespace internal {
namespace matrix_impl {
template <class T> struct interface : virtual grid_impl::interface<T> {};
} // namespace matrix_impl
template <class T, class base>
struct matrix_core : base, virtual matrix_impl::interface<T> {
    using base::base;
    static inline matrix_core
    identity(const size_t n, const T &&val = {1}) noexcept(NO_EXCEPT) {
        matrix_core res(n);
        REP(i, n) res(i, i) = val;
        return res;
    }
    inline size_t rows() const noexcept(NO_EXCEPT) /*override*/ {
        return this->height();
    }
    inline size_t cols() const noexcept(NO_EXCEPT) /*override*/ {
        return this->width();
    }
    inline size_t square() const noexcept(NO_EXCEPT) /*override*/ {
        return this->rows() == this->cols();
    }
    template <class U>
    inline matrix_core &operator+=(const U rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs;
        return *this;
    }
    template <class... U>
    inline matrix_core &
    operator+=(const matrix_core<U...> rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs(i, j);
        return *this;
    }
    template <class U>
    inline matrix_core operator+(const U rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) += rhs;
    }
    template <class U>
    inline matrix_core &operator-=(const U rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs;
        return *this;
    }
    template <class... U>
    inline matrix_core &
    operator-=(const matrix_core<U...> rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs(i, j);
        return *this;
    }
    template <class U>
    inline matrix_core operator-(const U rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) -= rhs;
    }
    template <class... U>
    inline matrix_core
    operator*(const matrix_core<U...> rhs) noexcept(NO_EXCEPT) {
        assert(this->cols() == rhs.rows());
        matrix_core res(this->rows(), rhs.cols());
        REP(i, this->rows()) REP(j, rhs.cols()) REP(k, this->cols()) {
            res(i, j) += (*this)(i, k) * rhs(k, j);
        }
        return res;
    }
    template <class U>
    inline matrix_core operator*(const U rhs) noexcept(NO_EXCEPT) {
        matrix_core res(*this);
        REP(i, res.rows()) REP(j, res.cols()) res(i, j) *= rhs;
        return res;
    }
    template <class U>
    inline matrix_core &operator*=(const U rhs) noexcept(NO_EXCEPT) {
        matrix_core res = *this * rhs;
        this->assign(res);
        return *this;
    }
    template <class U>
    inline matrix_core &operator/=(const U rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) /= rhs;
        return *this;
    }
    template <class U>
    inline matrix_core operator/(const U rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) /= rhs;
    }
    template <class U>
    inline matrix_core &operator%=(const U rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) %= rhs;
        return *this;
    }
    template <class U>
    inline matrix_core operator%(const U rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) %= rhs;
    }
    inline matrix_core pow(ll p) noexcept(NO_EXCEPT) {
        assert(this->square());
        matrix_core x = *this, res = matrix_core::Identity(this->rows());
        while(p > 0) {
            if(p & 1)
                res *= x;
            x *= x;
            p >>= 1;
        }
        return res;
    }
};
} // namespace internal
template <class T, class base = grid<T>>
using matrix = internal::matrix_core<T, base>;
template <class T>
using valmatrix = internal::matrix_core<T, unfolded_grid<T, valarray<T>>>;
template <class T>
using unfolded_matrix = internal::matrix_core<T, unfolded_grid<T>>;
} // namespace lib
/* [numeric/matrix.hpp] */
/* #expanded [numeric/modint.hpp] */
#include <atcoder/modint>
#include <cassert>
#include <cstdint>
namespace lib {
template <int _mod> using static_modint = atcoder::static_modint<_mod>;
using modint998244353 = atcoder::modint998244353;
using modint1000000007 = atcoder::modint1000000007;
template <int id> using dynamic_modint = atcoder::dynamic_modint<id>;
using modint = atcoder::modint;
template <int> struct dynamic_modint_64bit;
using modint64 = dynamic_modint_64bit<-1>;
} // namespace lib
namespace lib {
template <int id> struct dynamic_modint_64bit : atcoder::internal::modint_base {
  private:
    using mint = dynamic_modint_64bit;
    using int64_t = std::int64_t;
    using uint64_t = std::uint64_t;
    using uint128_t = internal::uint128_t;

  protected:
    static uint64_t _mod;
    static uint64_t r;
    static uint64_t n2;
    static uint64_t get_r() noexcept(NO_EXCEPT) {
        uint64_t res = _mod;
        for(int64_t i = 0; i < 5; ++i)
            res *= 2 - _mod * res;
        return res;
    }
    static uint64_t reduce(const uint128_t &b) noexcept(NO_EXCEPT) {
        return (b + uint128_t(uint64_t(b) * uint64_t(-r)) * _mod) >> 64;
    }

  public:
    static uint64_t mod() noexcept(NO_EXCEPT) { return _mod; }
    static void set_mod(const uint64_t m) noexcept(NO_EXCEPT) {
        assert(m < (1UL << 63));
        assert((m & 1) == 1);
        _mod = m;
        n2 = -static_cast<uint128_t>(m) % m;
        r = get_r();
        assert(r * _mod == 1);
    }
    uint64_t _val;
    dynamic_modint_64bit() noexcept(NO_EXCEPT) : _val(0) {}
    dynamic_modint_64bit(const std::int64_t b) noexcept(NO_EXCEPT)
        : _val(this->reduce((static_cast<uint128_t>(b) + this->_mod) *
                            this->n2)){};
    mint &operator+=(const mint &b) noexcept(NO_EXCEPT) {
        if(static_cast<int64_t>(_val += b._val - 2 * _mod) < 0)
            this->_val += 2 * this->_mod;
        return *this;
    }
    mint &operator-=(const mint &b) noexcept(NO_EXCEPT) {
        if(static_cast<int64_t>(this->_val -= b._val) < 0)
            this->_val += 2 * this->_mod;
        return *this;
    }
    mint &operator*=(const mint &b) noexcept(NO_EXCEPT) {
        this->_val = reduce(static_cast<uint128_t>(this->_val) * b._val);
        return *this;
    }
    mint &operator/=(const mint &b) noexcept(NO_EXCEPT) {
        *this *= b.inv();
        return *this;
    }
    mint operator+(const mint &b) const noexcept(NO_EXCEPT) {
        return mint(*this) += b;
    }
    mint operator-(const mint &b) const noexcept(NO_EXCEPT) {
        return mint(*this) -= b;
    }
    mint operator*(const mint &b) const noexcept(NO_EXCEPT) {
        return mint(*this) *= b;
    }
    mint operator/(const mint &b) const noexcept(NO_EXCEPT) {
        return mint(*this) /= b;
    }
    bool operator==(const mint &b) const noexcept(NO_EXCEPT) {
        return (this->_val >= this->_mod ? this->_val - this->_mod
                                         : this->_val) ==
               (b._val >= this->_mod ? b._val - this->_mod : b._val);
    }
    bool operator!=(const mint &b) const noexcept(NO_EXCEPT) {
        return (this->_val >= this->_mod ? this->_val - this->_mod
                                         : this->_val) !=
               (b._val >= this->_mod ? b._val - this->_mod : b._val);
    }
    mint operator-() const noexcept(NO_EXCEPT) {
        return mint{} - static_cast<mint>(*this);
    }
    mint pow(uint128_t n) const noexcept(NO_EXCEPT) {
        mint res(1), mul(*this);
        while(n > 0) {
            if(n & 1)
                res *= mul;
            mul *= mul;
            n >>= 1;
        }
        return res;
    }
    mint inv() const noexcept(NO_EXCEPT) { return this->pow(this->_mod - 2); }
    uint64_t val() const noexcept(NO_EXCEPT) {
        uint64_t res = this->reduce(this->_val);
        return res >= this->_mod ? res - this->_mod : res;
    }
};
template <int id>
typename dynamic_modint_64bit<id>::uint64_t dynamic_modint_64bit<id>::_mod;
template <int id>
typename dynamic_modint_64bit<id>::uint64_t dynamic_modint_64bit<id>::r;
template <int id>
typename dynamic_modint_64bit<id>::uint64_t dynamic_modint_64bit<id>::n2;
} // namespace lib
/* [numeric/modint.hpp] */
/* #expanded [constants.hpp] */
#include <cstdint>
#include <limits>
#include <utility>
namespace lib {
i32 INF32 = (std::numeric_limits<i32>::max() >> 1) - 1;
i64 INF64 = (std::numeric_limits<i64>::max() >> 1) - 1;
constexpr char LN = '\n';
constexpr char SPC = ' ';
constexpr std::pair<int, int> DIRS4[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
constexpr std::pair<int, int> DIRS8[] = {{-1, 0}, {-1, 1}, {0, 1},  {1, 1},
                                         {1, 0},  {1, -1}, {0, -1}, {-1, -1}};
enum class comp : std::int8_t {
    equal_to,
    not_equal_to,
    equals = equal_to,
    eq = equal_to,
    under,
    over,
    or_under,
    or_over,
    less = under,
    more = over,
    less_than = under,
    more_than = over,
    not_less_than = or_over,
    not_more_than = or_under,
    leq = or_under,
    geq = or_over
};
enum class interval : std::int8_t {
    right_open,
    left_open,
    open,
    closed,
};
} // namespace lib
/* [constants.hpp] */
/* #expanded [multi_container.hpp] */
#include <array>
#include <cassert>
#include <vector>
/* #expanded [internal/exception.hpp] */
namespace lib {
namespace internal {
template <class... T> constexpr bool EXCEPTION = false;
template <const int T> constexpr bool EXCEPTION_INT = false;
} // namespace internal
} // namespace lib
/* [internal/exception.hpp] */
namespace lib {
namespace internal {
namespace multi_container_impl {
template <class container> struct base : container {
    using container::container;

  protected:
    inline void _validate_index(__attribute__((unused))
                                const internal::size_t index) const
        noexcept(NO_EXCEPT) {
        assert(0 <= index and index < (internal::size_t)this->size());
    }
    inline internal::size_t _positivize_index(const internal::size_t x) const
        noexcept(NO_EXCEPT) {
        return x < 0 ? this->size() + x : x;
    }
};
} // namespace multi_container_impl
} // namespace internal
template <class T, const unsigned int RANK,
          template <class...> class container = valarray>
struct multi_container
    : internal::multi_container_impl::base<
          container<multi_container<T, RANK - 1, container>>> {
    using internal::multi_container_impl::base<
        container<multi_container<T, RANK - 1, container>>>::base;
    template <class Head, class... Tail>
    multi_container(const Head head, const Tail... tail) noexcept(NO_EXCEPT)
        : internal::multi_container_impl::base<
              container<multi_container<T, RANK - 1, container>>>(
              head, multi_container<T, RANK - 1, container>(tail...)) {
        static_assert(std::is_integral_v<Head>, "size must be integral");
    }
    template <class Head, class... Tail>
    T &operator()(const Head _head, const Tail... tail) noexcept(NO_EXCEPT) {
        static_assert(std::is_integral_v<Head>, "index must be integral");
        const internal::size_t head = this->_positivize_index(_head);
        this->_validate_index(head);
        return (*this)[head](tail...);
    }
    template <class Head, class... Tail>
    const T &operator()(const Head _head, const Tail... tail) const
        noexcept(NO_EXCEPT) {
        static_assert(std::is_integral_v<Head>, "index must be integral");
        const internal::size_t head = this->_positivize_index(_head);
        this->_validate_index(head);
        return (*this)[head](tail...);
    }
};
template <class T, template <class...> class container>
struct multi_container<T, 1, container>
    : internal::multi_container_impl::base<container<T>> {
    using internal::multi_container_impl::base<container<T>>::base;
    template <class... Args>
    multi_container(const Args &...args) noexcept(NO_EXCEPT)
        : internal::multi_container_impl::base<container<T>>(args...) {}
    T &operator()(const internal::size_t _index) noexcept(NO_EXCEPT) {
        const internal::size_t index = this->_positivize_index(_index);
        this->_validate_index(index);
        return (*this)[index];
    }
    const T &operator()(const internal::size_t _index) const
        noexcept(NO_EXCEPT) {
        const internal::size_t index = this->_positivize_index(_index);
        this->_validate_index(index);
        return (*this)[index];
    }
};
template <class T, template <class...> class container>
struct multi_container<T, 0, container> {
    static_assert(internal::EXCEPTION<T>,
                  "invalid rank: 0, should be 1 or more");
};
} // namespace lib
/* [multi_container.hpp] */
/* #expanded [graph.hpp] */
#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>
/* #expanded [data_structure/disjoint_set_union.hpp] */
#include <algorithm>
#include <cassert>
#include <vector>
namespace lib {
struct dsu {
    using size_type = internal::size_t;

  private:
    size_type _n, _group_count;
    mutable std::vector<size_type> _parent_or_size;

  public:
    dsu() noexcept(NO_EXCEPT) : _n(0) {}
    explicit dsu(const size_type n) noexcept(NO_EXCEPT)
        : _n(n), _group_count(n), _parent_or_size(n, -1) {}
    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n; }
    inline size_type group_count() const noexcept(NO_EXCEPT) {
        return this->_group_count;
    }
    inline size_type merge(const size_type a,
                           const size_type b) noexcept(NO_EXCEPT) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        size_type x = this->leader(a), y = this->leader(b);
        if(x == y)
            return x;
        --this->_group_count;
        if(-this->_parent_or_size[x] < -this->_parent_or_size[y])
            std::swap(x, y);
        this->_parent_or_size[x] += this->_parent_or_size[y];
        this->_parent_or_size[y] = x;
        return x;
    }
    inline bool same(const size_type a, const size_type b) const
        noexcept(NO_EXCEPT) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        return this->leader(a) == this->leader(b);
    }
    inline size_type leader(const size_type a) const noexcept(NO_EXCEPT) {
        assert(0 <= a && a < _n);
        if(_parent_or_size[a] < 0)
            return a;
        return _parent_or_size[a] = this->leader(_parent_or_size[a]);
    }
    inline size_type size(const size_type a) const noexcept(NO_EXCEPT) {
        assert(0 <= a && a < _n);
        return -_parent_or_size[this->leader(a)];
    }
    inline std::vector<std::vector<size_type>> groups() const
        noexcept(NO_EXCEPT) {
        std::vector<size_type> leader_buf(_n), group_size(_n);
        for(size_type i = 0; i < _n; i++) {
            leader_buf[i] = this->leader(i);
            group_size[leader_buf[i]]++;
        }
        std::vector<std::vector<size_type>> result(_n);
        for(size_type i = 0; i < _n; i++) {
            result[i].reserve(group_size[i]);
        }
        for(size_type i = 0; i < _n; i++) {
            result[leader_buf[i]].push_back(i);
        }
        result.erase(std::remove_if(result.begin(), result.end(),
                                    [&](const std::vector<size_type> &v) {
                                        return v.empty();
                                    }),
                     result.end());
        return result;
    }
};
} // namespace lib
/* [data_structure/disjoint_set_union.hpp] */
namespace lib {
namespace internal {
namespace graph_impl {
template <class cost_t, class size_type> struct edge {
  private:
    inline static internal::size_t unique() noexcept(NO_EXCEPT) {
        static internal::size_t id = 0;
        return id++;
    }

  public:
    using cost_type = cost_t;
    const internal::size_t id = unique();
    const size_type from, to;
    const cost_t cost;
    edge(const size_type u, const size_type v,
         const cost_t w) noexcept(NO_EXCEPT)
        : from(u), to(v), cost(w) {}
    operator size_type() const noexcept(NO_EXCEPT) { return this->to; }
    inline size_type opposite(const size_type v) const noexcept(NO_EXCEPT) {
        if(this->from == v)
            return this->to;
        if(this->to == v)
            return this->from;
        assert(false);
    }
    std::tuple<size_type, size_type, cost_t> _debug() const
        noexcept(NO_EXCEPT) {
        return {from, to, cost};
    };
    friend bool operator==(const edge &lhs,
                           const edge &rhs) noexcept(NO_EXCEPT) {
        return lhs.id == rhs.id;
    }
    friend bool operator!=(const edge &lhs,
                           const edge &rhs) noexcept(NO_EXCEPT) {
        return lhs.id != rhs.id;
    }
};
} // namespace graph_impl
} // namespace internal
template <class C = ll>
struct graph
    : std::vector<
          std::vector<internal::graph_impl::edge<C, internal::size_t>>> {
    using size_type = internal::size_t;
    using cost_type = C;
    using edge_type = typename internal::graph_impl::edge<cost_type, size_type>;
    enum class edge_kind { undirected, directed };

  private:
    size_type _directed_edge_count = 0, _undirected_edge_count = 0;
    std::vector<edge_type> _edges;
    std::vector<size_type> _out_degs, _in_degs;

  protected:
    inline void _add_edge(const size_type u, const size_type v,
                          const cost_type w) noexcept(NO_EXCEPT) {
        this->operator[](u).emplace_back(u, v, w);
        ++_out_degs[u], ++_in_degs[v];
        ++this->_directed_edge_count;
    }

  public:
    explicit graph(const size_type n = 0) noexcept(NO_EXCEPT)
        : std::vector<std::vector<edge_type>>(n), _out_degs(n), _in_degs(n) {}
    inline void clear() noexcept(NO_EXCEPT) {
        this->std::vector<std::vector<edge_type>>::clear();
    }
    inline const auto &edges() const noexcept(NO_EXCEPT) {
        return this->_edges;
    }
    inline const auto &edge(const size_type k) const noexcept(NO_EXCEPT) {
        return this->_edges[k];
    }
    inline const auto &degrees() const noexcept(NO_EXCEPT) {
        return this->_in_degs;
    }
    inline const auto &degree(const size_type k) const noexcept(NO_EXCEPT) {
        return this->_in_degs[k];
    }
    inline const auto &in_degrees() const noexcept(NO_EXCEPT) {
        return this->_in_degs;
    }
    inline const auto &in_degree(const size_type k) const noexcept(NO_EXCEPT) {
        return this->_in_degs[k];
    }
    inline const auto &out_degrees() const noexcept(NO_EXCEPT) {
        return this->_out_degs;
    }
    inline const auto &out_degree(const size_type k) const noexcept(NO_EXCEPT) {
        return this->_out_degs[k];
    }
    inline size_type vertices() const noexcept(NO_EXCEPT) {
        return this->size();
    }
    inline size_type directed_edges_count() const noexcept(NO_EXCEPT) {
        return this->_directed_edge_count;
    }
    template <const edge_kind EDGE_TYPE = edge_kind::directed>
    inline void add_edge(const size_type u, const size_type v,
                         const cost_type w = 1) noexcept(NO_EXCEPT) {
        assert(0 <= u and u < this->vertices()),
            assert(0 <= v and v < this->vertices());
        this->_edges.emplace_back(u, v, w);
        this->_add_edge(u, v, w);
        if constexpr(EDGE_TYPE == edge_kind::undirected)
            this->_add_edge(v, u, w);
    }
    inline void
    add_edge_bidirectionally(const size_type u, const size_type v,
                             const cost_type w = 1) noexcept(NO_EXCEPT) {
        this->add_edge<edge_kind::undirected>(u, v, w);
    }
    template <bool WEIGHTED = false, bool ONE_ORIGIN = true,
              const edge_kind EDGE_TYPE = edge_kind::directed,
              class Stream = std::istream>
    void inline read(const size_type edges,
                     Stream *const ist = &std::cin) noexcept(NO_EXCEPT) {
        REP(edges) {
            size_type u, v;
            cost_type w = 1;
            *ist >> u >> v;
            if(ONE_ORIGIN)
                --u, --v;
            if(WEIGHTED)
                *ist >> w;
            this->add_edge<EDGE_TYPE>(u, v, w);
        }
    }
    template <bool WEIGHTED = false, bool ONE_ORIGIN = true,
              class Stream = std::istream>
    void inline read_bidirectionally(
        const size_type edges,
        Stream *const ist = &std::cin) noexcept(NO_EXCEPT) {
        REP(edges) {
            size_type u, v;
            cost_type w = 1;
            *ist >> u >> v;
            if(ONE_ORIGIN)
                --u, --v;
            if(WEIGHTED)
                *ist >> w;
            this->add_edge<edge_kind::undirected>(u, v, w);
        }
    }
    template <class cost_t = cost_type>
    inline void distances_without_cost(const size_type,
                                       std::vector<cost_t> *const) const
        noexcept(NO_EXCEPT);
    template <class cost_t = cost_type>
    inline std::vector<cost_t> distances_without_cost(const size_type) const
        noexcept(NO_EXCEPT);
    template <class cost_t = cost_type>
    inline void distances_with_01cost(const size_type,
                                      std::vector<cost_t> *const) const
        noexcept(NO_EXCEPT);
    template <class cost_t = cost_type>
    inline std::vector<cost_t> distances_with_01cost(const size_type) const
        noexcept(NO_EXCEPT);
    template <class cost_t = cost_type>
    inline void distances(const size_type, std::vector<cost_t> *const) const
        noexcept(NO_EXCEPT);
    template <class cost_t = cost_type>
    inline std::vector<cost_t> distances(const size_type) const
        noexcept(NO_EXCEPT);
    inline bool sort_topologically(std::vector<size_type> *const) const
        noexcept(NO_EXCEPT);
    inline bool sort_topologically() const noexcept(NO_EXCEPT);
    template <class>
    inline bool
    sort_topologically_with_priority(std::vector<size_type> *const) const
        noexcept(NO_EXCEPT);
    template <class>
    inline bool sort_topologically_with_priority() const noexcept(NO_EXCEPT);
    inline size_type minimum_paph_cover_size_as_dag() const noexcept(NO_EXCEPT);
    template <class cost_t = cost_type>
    inline cost_t minimum_spanning_tree(graph *const = nullptr) const
        noexcept(NO_EXCEPT);
    template <class cost_t = cost_type>
    inline cost_t maximum_spanning_tree(graph *const = nullptr) const
        noexcept(NO_EXCEPT);
    inline dsu components() const noexcept(NO_EXCEPT);
    template <bool = false, class G, class U = char>
    inline void from_grid(const G &, U = '.') noexcept(NO_EXCEPT);
    template <class I, class J = I, class distance_type = cost_type,
              class = internal::size_t>
    inline distance_type build_manhattan_mst(const I, const I, const J,
                                             const J) noexcept(NO_EXCEPT);
};
} // namespace lib
/* [graph.hpp] */
/* #expanded [string.hpp] */
#include <iterator>
#include <string>
namespace lib {
template <class I, class Res = std::string>
Res to_lower(const I first, const I last) noexcept(NO_EXCEPT) {
    Res res;
    res.reserve(std::distance(first, last));
    std::transform(first, last, std::back_inserter(res), ::tolower);
    return res;
}
template <class I, class Res = std::string>
Res to_uppwer(const I first, const I last) noexcept(NO_EXCEPT) {
    Res res;
    res.reserve(std::distance(first, last));
    std::transform(first, last, std::back_inserter(res), ::toupper);
    return res;
}
template <class Res = std::string>
Res to_lower(const std::string str) noexcept(NO_EXCEPT) {
    return to_lower<std::string::const_iterator, Res>(std::begin(str),
                                                      std::end(str));
}
template <class Res = std::string>
Res to_uppwer(const std::string str) noexcept(NO_EXCEPT) {
    return to_uppwer<std::string::const_iterator, Res>(std::begin(str),
                                                       std::end(str));
}
} // namespace lib
/* [string.hpp] */
/* #expanded [adapter/set.hpp] */
#include <iterator>
#include <optional>
#include <set>
#include <unordered_set>
/* #expanded [utility/functional.hpp] */
#include <functional>
#include <utility>
namespace lib {
namespace internal {
template <class T> constexpr T plus(const T a, const T b) {
    return std::plus<T>{}(a, b);
}
template <class T> constexpr T minus(const T a, const T b) {
    return std::minus<T>{}(a, b);
}
template <class T> constexpr T bitxor(const T a, const T b) { return a xor b; }
} // namespace internal
template <class T1, class T2> inline auto mod(T1 x, T2 r) {
    return (x % r + r) % r;
}
template <class T1, class T2> inline bool chmax(T1 &a, T2 b) {
    return (a < b ? a = b, true : false);
}
template <class T1, class T2> inline bool chmin(T1 &a, T2 b) {
    return (a > b ? a = b, true : false);
}
template <class T> inline constexpr T sign(const T x) {
    return (x > 0) - (x < 0);
}
template <class T, T FROM_MIN, T FROM_MAX, T TO_MIN, T TO_MAX>
inline constexpr T mapping(const T x) {
    return (x - FROM_MIN) * (TO_MAX - TO_MIN) / (FROM_MAX - FROM_MIN) + TO_MIN;
}
template <class T>
inline constexpr T mapping(const T x, const T from_min, const T from_max,
                           const T to_min, const T to_max) {
    return (x - from_min) * (to_max - to_min) / (from_max - from_min) + to_min;
}
} // namespace lib
/* [utility/functional.hpp] */
namespace lib {
namespace internal {
template <class set> struct set_wrapper : set {
    using set::set;
    using size_type = internal::size_t;
    inline size_type size() const noexcept(NO_EXCEPT) {
        return this->set::size();
    }
    inline bool
    contains(const typename set::key_type &key) noexcept(NO_EXCEPT) {
        return static_cast<bool>(this->count(key));
    }
    inline std::optional<typename set::iterator>
    remove(const typename set::key_type &key) noexcept(NO_EXCEPT) {
        const auto itr = this->find(key);
        if(itr == this->set::end())
            return {};
        return this->erase(itr);
    }
    inline auto min_element() noexcept(NO_EXCEPT) { return this->begin(); }
    inline auto max_element() noexcept(NO_EXCEPT) { return this->begin(); }
    inline auto min() noexcept(NO_EXCEPT) { return *this->begin(); }
    inline auto max() noexcept(NO_EXCEPT) { return *this->end(); }
    inline auto pop_min() noexcept(NO_EXCEPT) {
        this->erase(this->begin());
        return *this;
    }
    inline auto pop_max() noexcept(NO_EXCEPT) {
        this->erase(this->end());
        return *this;
    }
    inline auto next_element(const typename set::key_type &key,
                             const size_type _count = 0) noexcept(NO_EXCEPT) {
        size_type count = std::abs(_count);
        auto itr = this->lower_bound(key);
        const auto begin = this->begin(), end = this->end();
        if(itr == end)
            return this->end();
        if(itr == begin)
            return this->begin();
        while(count--) {
            if(_count < 0)
                if(itr-- == begin)
                    return this->begin();
            if(_count > 0)
                if(++itr == end)
                    return this->end();
        }
        return itr;
    }
    inline auto prev_element(const typename set::key_type &key,
                             const size_type _count = 0) noexcept(NO_EXCEPT) {
        size_type count = std::abs(_count);
        auto itr = this->upper_bound(key);
        const auto begin = this->begin(), end = this->end();
        if(itr == end)
            return this->end();
        if(itr-- == begin)
            return this->begin();
        while(count--) {
            if(_count < 0)
                if(itr-- == begin)
                    return this->begin();
            if(_count > 0)
                if(++itr == end)
                    return this->end();
        }
        return itr;
    }
    inline std::optional<typename set::value_type>
    next(const typename set::key_type &key,
         size_type count = 0) noexcept(NO_EXCEPT) {
        auto itr = this->lower_bound(key);
        const auto end = this->end();
        if(itr == end)
            return {};
        while(count--)
            if(++itr == end)
                return {};
        return {*itr};
    }
    inline std::optional<typename set::value_type>
    prev(const typename set::key_type &key,
         size_type count = 0) noexcept(NO_EXCEPT) {
        auto itr = this->upper_bound(key);
        const auto begin = this->begin();
        if(itr-- == begin)
            return {};
        while(count--)
            if(itr-- == begin)
                return {};
        return {*itr};
    }
    friend inline set_wrapper
    operator|(const set_wrapper s, const set_wrapper t) noexcept(NO_EXCEPT) {
        set_wrapper res;
        std::set_union(std::begin(s), std::end(s), std::begin(t), std::end(t),
                       std::inserter(res, std::begin(res)));
        return res;
    }
};
} // namespace internal
template <class... Args> using set = internal::set_wrapper<std::set<Args...>>;
template <class... Args>
using unordered_set = internal::set_wrapper<std::unordered_set<Args...>>;
template <class... Args>
using multiset = internal::set_wrapper<std::multiset<Args...>>;
template <class... Args>
using unordered_multiset =
    internal::set_wrapper<std::unordered_multiset<Args...>>;
} // namespace lib
/* [adapter/set.hpp] */
/* #expanded [adapter/vector.hpp] */
#include <algorithm>
#include <vector>
namespace lib {
template <class T> struct vector : std::vector<T> {
    using std::vector<T>::vector;
    inline auto &sort() noexcept(NO_EXCEPT) {
        std::sort(this->begin(), this->end());
        return *this;
    }
    template <class F> inline auto &sort(const F &&f) noexcept(NO_EXCEPT) {
        std::sort(this->begin(), this->end(), f);
        return *this;
    }
};
} // namespace lib
/* [adapter/vector.hpp] */
/* #expanded [utility/applier.hpp] */
#include <algorithm>
#include <utility>
namespace lib {
namespace internal {
template <class T> T max(T a, T b) { return std::max(a, b); }
template <class T> T max(T x) { return x; }
template <class T> T max() { return -std::numeric_limits<T>::min(); }
template <class T> T min(T a, T b) { return std::min(a, b); }
template <class T> T min(T x) { return x; }
template <class T> T min() { return std::numeric_limits<T>::max(); }
} // namespace internal
template <class T, T (*op)(T, T)> struct applier {
    using value_type = T;

  protected:
    T _v;

  public:
    applier(const T &v = T{}) : _v(v) {}
    template <class U> applier &operator<<=(U &&val) & noexcept {
        _v = op(_v, std::forward<U>(val));
        return *this;
    }
    inline T val() const { return _v; }
    inline operator T() const { return _v; }
};
template <class T> using maximum = applier<T, internal::max<T>>;
template <class T> using minimum = applier<T, internal::min<T>>;
} // namespace lib
/* [utility/applier.hpp] */
/* #expanded [utility/restrictor.hpp] */
#include <utility>
namespace lib {
template <class T, T INF, T SUP> struct static_restrictor {
    using restrictor = static_restrictor;

  protected:
    T _v;
    inline void _clamp() { this->_v = std::clamp(this->_v, INF, SUP); }
    inline restrictor &_assign(T v) {
        this->_v = std::clamp(v, INF, SUP);
        return *this;
    }

  public:
    static_restrictor(T v = T{}) : _v(v) {}
    inline T val() const { return this->_v; }
    restrictor &operator++() { return this->_assign(this->_v + 1); }
    restrictor &operator--() { return this->_assign(this->_v - 1); }
    restrictor operator++(int) {
        restrictor res = *this;
        *this ++;
        return res;
    }
    restrictor operator--(int) {
        restrictor res = *this;
        *this --;
        return res;
    }
    restrictor &operator+=(const restrictor &rhs) {
        return this->_assign(this->_v + rhs.val());
    }
    restrictor &operator-=(const restrictor &rhs) {
        return this->_assign(this->_v - rhs.val());
    }
    restrictor &operator*=(const restrictor &rhs) {
        if(mul_over(this->_v, rhs.val(), SUP))
            return this->_assign(SUP);
        if(mul_under(this->_v, rhs.val(), INF))
            return this->_assign(INF);
        return this->_assign(this->_v * rhs.val());
    }
    restrictor &operator/=(const restrictor &rhs) {
        return this->_assign(this->_v / rhs.val());
    }
    restrictor operator+() const { return *this; }
    restrictor operator-() const { return restrictor() - *this; }
    friend restrictor operator+(const restrictor &lhs, const restrictor &rhs) {
        return restrictor(lhs) += rhs;
    }
    friend restrictor operator-(const restrictor &lhs, const restrictor &rhs) {
        return restrictor(lhs) -= rhs;
    }
    friend restrictor operator*(const restrictor &lhs, const restrictor &rhs) {
        return restrictor(lhs) *= rhs;
    }
    friend restrictor operator/(const restrictor &lhs, const restrictor &rhs) {
        return restrictor(lhs) /= rhs;
    }
    friend bool operator==(const restrictor &lhs, const restrictor &rhs) {
        return lhs._v == rhs._v;
    }
    friend bool operator!=(const restrictor &lhs, const restrictor &rhs) {
        return lhs._v != rhs._v;
    }
    friend bool operator<(const restrictor &lhs, const restrictor &rhs) {
        return lhs._v < rhs._v;
    }
    friend bool operator>(const restrictor &lhs, const restrictor &rhs) {
        return lhs._v > rhs._v;
    }
    friend bool operator<=(const restrictor &lhs, const restrictor &rhs) {
        return lhs._v <= rhs._v;
    }
    friend bool operator>=(const restrictor &lhs, const restrictor &rhs) {
        return lhs._v >= rhs._v;
    }
};
} // namespace lib
namespace std {
template <class T, T INF, T SUP>
T std::abs(const lib::static_restrictor<T, INF, SUP> &v) {
    return std::abs(v.val());
}
} // namespace std
/* [utility/restrictor.hpp] */
/* #expanded [iterable/applied.hpp] */
#include <algorithm>
#include <iterator>
#include <vector>
namespace lib {
template <class I, class F,
          class C = std::vector<typename std::iterator_traits<I>::value_type>>
inline auto applied(const I first, const I last, F &&func) noexcept(NO_EXCEPT) {
    C res(first, last);
    func(std::begin(res), std::end(res));
    return res;
}
template <class V, class F, class C = V>
inline auto applied(V v, F &&func) noexcept(NO_EXCEPT) {
    return applied<typename V::iterator, F, C>(std::begin(v), std::end(v),
                                               func);
}
template <class I>
inline auto sorted(const I first, const I last) noexcept(NO_EXCEPT) {
    return applied(first, last, std::sort<I>);
}
template <class V> inline auto sorted(V v) noexcept(NO_EXCEPT) {
    return applied(v, std::sort<typename V::iterator>);
}
template <class I>
inline auto reversed(const I first, const I last) noexcept(NO_EXCEPT) {
    return applied(first, last, std::reverse<I>);
}
template <class V> inline auto reversed(V v) noexcept(NO_EXCEPT) {
    return applied(v, std::reverse<typename V::iterator>);
}
} // namespace lib
/* [iterable/applied.hpp] */
/* #expanded [iterable/accumulation.hpp] */
#include <cassert>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>
namespace lib {
template <class T = i64, class container = valarray<T>>
struct accumulation : container {
    using size_type = internal::size_t;

  protected:
    inline size_type _positivize_index(const size_type x) const
        noexcept(NO_EXCEPT) {
        return x < 0 ? std::size(*this) + x : x;
    }

  public:
    accumulation() noexcept(NO_EXCEPT) {}
    template <class I, class Operator = std::plus<T>>
    accumulation(const I first, const I last, const T head = T{},
                 const Operator op = std::plus<T>{}) noexcept(NO_EXCEPT) {
        this->assign(std::distance(first, last) + 1, {});
        std::exclusive_scan(first, last, std::begin(*this), head, op);
        if(this->size() > 1)
            *std::prev(this->end()) =
                op(*std::prev(std::end(*this), 2), *std::prev(last));
    }
    template <class Operaotr = std::minus<T>>
    inline T operator()(size_type left, size_type right,
                        Operaotr op = std::minus<T>{}) const
        noexcept(NO_EXCEPT) {
        left = _positivize_index(left), right = _positivize_index(right);
        assert(0 <= left and left <= right and
               right < (size_type)std::size(*this));
        return op((*this)[right], (*this)[left]);
    }
};
template <class T, class container = valarray<valarray<T>>,
          class Operator = std::plus<T>>
struct accumulation_2d : container {
    using size_type = internal::size_t;

  protected:
    inline size_type _positivize_index(const size_type x) const
        noexcept(NO_EXCEPT) {
        return x < 0 ? std::size(*this) + x : x;
    }
    Operator _op;

  public:
    accumulation_2d() noexcept(NO_EXCEPT) {}
    template <class I>
    accumulation_2d(const I first, const I last, const T head = T{},
                    const Operator op = std::plus<T>{}) noexcept(NO_EXCEPT)
        : _op(op) {
        const size_type h = std::distance(first, last);
        const size_type w = std::distance(std::begin(*first), std::end(*first));
        {
            auto row = first;
            this->assign(h + 1, head);
            (*this)[0].assign(w + 1, head);
            REP(i, h) {
                assert(w == std::distance(std::begin(*row), std::end(*row)));
                (*this)[i + 1].assign(w + 1, head);
                REP(j, w)(*this)[i + 1][j + 1] = first[i][j];
                ++row;
            }
        }
        FOR(i, 1, h)
        FOR(j, w)(*this)[i][j] = op((*this)[i][j], (*this)[i - 1][j]);
        FOR(i, h)
        FOR(j, 1, w)(*this)[i][j] = op((*this)[i][j], (*this)[i][j - 1]);
    }
    template <class Rev = std::minus<T>>
    inline T operator()(size_type a, size_type b, size_type c, size_type d,
                        const Rev rev = std::minus<T>{}) const
        noexcept(NO_EXCEPT) {
        a = _positivize_index(a), b = _positivize_index(b);
        c = _positivize_index(c), d = _positivize_index(d);
        assert(0 <= a and a <= b and b < (size_type)std::size(*this));
        assert(0 <= c and c <= d and d < (size_type)std::size((*this)[0]));
        return this->_op(
            rev((*this)[b][d], this->_op((*this)[a][d], (*this)[b][c])),
            (*this)[a][c]);
    }
    template <class Rev = std::minus<T>>
    inline T operator()(const std::pair<size_type, size_type> p,
                        const std::pair<size_type, size_type> q,
                        const Rev rev = std::minus<T>{}) const
        noexcept(NO_EXCEPT) {
        return this->operator()(p.first, p.second, q.first, q.second, rev);
    }
};
} // namespace lib
/* [iterable/accumulation.hpp] */
/* #expanded [iterable/operation.hpp] */
#include <iterator>
#include <numeric>
#include <string>
namespace lib {
template <class I>
std::string join(const I first, const I last,
                 const std::string &sep = "") noexcept(NO_EXCEPT) {
    std::ostringstream res;
    std::copy(
        first, last,
        std::ostream_iterator<typename std::iterator_traits<I>::value_type>(
            res, sep));
    return res.str();
}
template <class V>
std::string join(V &v, const std::string &sep = "") noexcept(NO_EXCEPT) {
    return join(std::begin(v), std::end(v), sep);
}
template <class I, class T = typename std::iterator_traits<I>::value_type>
T sum(const I first, const I second, const T &base = 0) noexcept(NO_EXCEPT) {
    return std::accumulate(first, second, base);
}
template <class V, class T = typename V::value_type>
auto sum(V &v, T base = 0) noexcept(NO_EXCEPT) {
    return sum(std::begin(v), std::end(v), base);
}
} // namespace lib
/* [iterable/operation.hpp] */
/* #expanded [iterable/mex.hpp] */
#include <algorithm>
#include <iterator>
namespace lib {
template <class I, class T = typename std::iterator_traits<I>::value_type>
T mex(const I first, const I last, const T &base = 0) noexcept(NO_EXCEPT) {
    std::vector<T> val(first, last);
    std::sort(val.begin(), val.end());
    val.erase(std::unique(val.begin(), val.end()), val.end());
    val.erase(val.begin(), std::lower_bound(val.begin(), val.end(), base));
    internal::size_t i = 0;
    while(i < (internal::size_t)val.size() and val[i] == T{i} + base)
        ++i;
    return T{i} + base;
}
template <class V, class T = typename V::value_type>
auto mex(const V v, const T &base = 0) noexcept(NO_EXCEPT) {
    return mex(v.begin(), v.end(), base);
}
} // namespace lib
/* [iterable/mex.hpp] */
/* [utilities.hpp] */
using lib::i32;
using lib::i64;
using lib::u32;
using lib::u64;
using std::array;
using std::bitset;
using std::cin;
using std::cout;
using std::map;
using std::max_element;
using std::min_element;
using std::pair;
using std::priority_queue;
using std::queue;
using std::reverse;
using std::sort;
using std::stack;
using std::string;
using std::tuple;
using std::unordered_map;
#ifdef __GNUC__
using lib::i128;
using lib::u128;
#endif
using lib::chmax;
using lib::chmin;
using lib::DIRS4;
using lib::DIRS8;
using lib::graph;
using lib::grid;
using lib::INF32;
using lib::INF64;
using lib::inv_mod;
using lib::join;
using lib::ld;
using lib::ll;
using lib::LN;
using lib::matrix;
using lib::maximum;
using lib::minimum;
using lib::modint;
using lib::modint1000000007;
using lib::modint64;
using lib::modint998244353;
using lib::multi_container;
using lib::multiset;
using lib::nCr;
using lib::nPr;
using lib::pow;
using lib::pow_mod;
using lib::reversed;
using lib::set;
using lib::sorted;
using lib::spair;
using lib::SPC;
using lib::uint;
using lib::ull;
using lib::unordered_multiset;
using lib::unordered_set;
using lib::valarray;
using lib::vector;
/* [snippet/using.hpp] */
#ifdef LOCAL_JUDGE
#include <debug>
#define debug(...)                                                             \
    debugger::debug(debugger::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
#define DEBUG(...)                                                             \
    do {                                                                       \
        debugger::DEBUG(nullptr,                                               \
                        "\033[3;35m#" + std::to_string(__LINE__) + "\033[m "); \
        debugger::DEBUG(__VA_ARGS__);                                          \
        debugger::DEBUG(nullptr, "\033[m\n");                                  \
    } while(0);
#else
#define debug(...) ({ ; })
#define DEBUG(...) ({ ; })
#endif
/* #expanded [adapter/input.hpp] */
#include <atcoder/modint>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
/* #expanded [internal/resolving_rank.hpp] */
namespace lib {
namespace internal {
template <int P> struct resolving_rank : resolving_rank<P - 1> {};
template <> struct resolving_rank<0> {};
} // namespace internal
} // namespace lib
/* [internal/resolving_rank.hpp] */
template <class source = std::istream> struct input_adapter {
  private:
    template <class T>
    auto _set(lib::internal::resolving_rank<3>,
              T *const val) noexcept(NO_EXCEPT)
        -> decltype(std::declval<source &>() >> *val, 0) {
        *this->in >> *val;
        return 0;
    }
    template <class T>
    auto _set(lib::internal::resolving_rank<2>,
              T *const val) noexcept(NO_EXCEPT)
        -> decltype(std::begin(*val), std::end(*val), 0) {
        (*this)(std::begin(*val), std::end(*val));
        return 0;
    }
    template <class T>
    auto _set(lib::internal::resolving_rank<1>,
              T *const val) noexcept(NO_EXCEPT)
        -> decltype(val->first, val->second, 0) {
        (*this)(&*val);
        return 0;
    }
    template <class T>
    auto _set(lib::internal::resolving_rank<0>,
              T *const val) noexcept(NO_EXCEPT)
        -> std::enable_if_t<atcoder::internal::is_modint<T>::value, int> {
        long long v;
        std::cin >> v;
        *val = {v};
        return 0;
    }

  protected:
    template <class T> source *set(T *const val) noexcept(NO_EXCEPT) {
        this->_set(lib::internal::resolving_rank<3>{}, val);
        return this->in;
    }

  public:
    using char_type = typename source::char_type;
    source *in;
    input_adapter(source *in = &std::cin) noexcept(NO_EXCEPT) : in(in) {}
    template <class T>
    inline input_adapter &operator>>(T &s) noexcept(NO_EXCEPT) {
        this->set(&s);
        return *this;
    }
    template <class T> inline T one() noexcept(NO_EXCEPT) {
        T val;
        *this >> val;
        return val;
    }
    template <class T>
    inline void operator()(T *const val) noexcept(NO_EXCEPT) {
        *this >> *val;
    }
    template <class T, class... Args>
    inline void operator()(T *const head,
                           Args *const... tail) noexcept(NO_EXCEPT) {
        *this >> *head;
        (*this)(tail...);
    }
    template <class I, class = typename std::iterator_traits<I>::value_type>
    inline void operator()(I first, I last) noexcept(NO_EXCEPT) {
        for(I itr = first; itr != last; ++itr)
            *this >> *itr;
    }
    template <class F, class S>
    inline void operator()(std::pair<F, S> *const p) noexcept(NO_EXCEPT) {
        *this >> p->first >> p->second;
    }
};
/* [adapter/input.hpp] */
/* #expanded [adapter/output.hpp] */
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
template <class destination = std::ostream, class Terminator = std::string,
          class Separator = std::string>
struct output_adapter {
  private:
    template <class T>
    auto _put(lib::internal::resolving_rank<2>,
              const T &val) noexcept(NO_EXCEPT)
        -> decltype(std::declval<destination &>() << val, 0) {
        *this->out << val;
        return 0;
    }
    template <class T>
    auto _put(lib::internal::resolving_rank<1>,
              const T &val) noexcept(NO_EXCEPT) -> decltype(val.val(), 0) {
        this->put(val.val());
        return 0;
    }
    template <class T>
    auto _put(lib::internal::resolving_rank<0>,
              const T &val) noexcept(NO_EXCEPT)
        -> decltype(std::begin(val), std::end(val), 0) {
        (*this)(std::begin(val), std::end(val), false);
        return 0;
    }

  protected:
    template <class T> destination *put(const T &val) noexcept(NO_EXCEPT) {
        this->_put(lib::internal::resolving_rank<2>{}, val);
        return this->out;
    }

  public:
    using char_type = typename destination::char_type;
    static constexpr auto sendl =
        std::endl<char_type, std::char_traits<char_type>>;
    destination *out;
    Terminator endline;
    Separator separator;
    output_adapter(destination *out = &std::cout, Terminator endline = "\n",
                   Separator separator = " ") noexcept
        : out(out), endline(endline), separator(separator) {
        *this << std::fixed << std::setprecision(20);
    }
    inline void seekp(const typename destination::off_type off,
                      const std::ios_base::seekdir dir =
                          std::ios_base::cur) noexcept(NO_EXCEPT) {
        this->out->seekp(off, dir);
    };
    template <class T>
    inline output_adapter &operator<<(const T &s) noexcept(NO_EXCEPT) {
        this->put(s);
        return *this;
    }
    template <class T = std::string>
    inline void operator()(const T &val = "") noexcept(NO_EXCEPT) {
        *this << val << this->endline;
    }
    template <class T, class... Args>
    inline void operator()(const T &head,
                           const Args &...tail) noexcept(NO_EXCEPT) {
        *this << head << this->separator;
        (*this)(tail...);
    }
    template <class I,
              class = typename std::iterator_traits<I>::iterator_category>
    inline void operator()(const I first, const I last,
                           const bool terminate = true) noexcept(NO_EXCEPT) {
        for(I itr = first; itr != last;) {
            *this << *itr;
            if(++itr == last) {
                if(terminate)
                    *this << this->endline;
            } else
                *this << this->separator;
        }
    }
    template <class T>
    inline void
    operator()(const std::initializer_list<T> vals) noexcept(NO_EXCEPT) {
        std::vector wrapped(vals.begin(), vals.end());
        (*this)(wrapped.begin(), wrapped.end());
    }
    template <class F, class S>
    inline void operator()(const std::pair<F, S> &p) noexcept(NO_EXCEPT) {
        (*this)(p.first, p.second);
    }
};
/* [adapter/output.hpp] */
input_adapter _input;
output_adapter _print;
#define input _input
#define print _print
/* [template.hpp] */
/* #expanded [data_structure/wavelet_matrix.hpp] */
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <limits>
#include <optional>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
/* #expanded [internal/iterator.hpp] */
#include <iterator>
namespace lib {
namespace internal {
template <class T> struct iterator_interface {
    using iterator_category = std::output_iterator_tag;
    using difference_type = size_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
};
template <class T> struct forward_iterator : iterator_interface<T> {
    using iterator_category = std::forward_iterator_tag;
};
template <class T>
struct bidirectional_iterator_interface : forward_iterator<T> {
    using iterator_category = std::bidirectional_iterator_tag;
};
template <class T>
struct random_access_iterator_base : bidirectional_iterator_interface<T> {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type =
        typename bidirectional_iterator_interface<T>::difference_type;

  public:
};
template <class T, class container>
struct container_iterator_interface : public random_access_iterator_base<T> {
    using difference_type =
        typename bidirectional_iterator_interface<T>::difference_type;

  protected:
    const container *const _ref;
    difference_type _pos;
    container_iterator_interface(const container *const ref,
                                 const difference_type &pos) noexcept(NO_EXCEPT)
        : _ref(ref), _pos(pos) {}

  public:
    inline const container *ref() const noexcept(NO_EXCEPT) {
        return this->_ref;
    }
    inline difference_type pos() const noexcept(NO_EXCEPT) {
        return this->_pos;
    }
    inline difference_type &pos() { return this->_pos; }
    inline container_iterator_interface &operator++() noexcept(NO_EXCEPT) {
        return ++this->pos(), *this;
    }
    inline container_iterator_interface &operator--() noexcept(NO_EXCEPT) {
        return --this->pos(), *this;
    }
    inline container_iterator_interface &
    operator+=(const difference_type count) noexcept(NO_EXCEPT) {
        return this->pos() += count, *this;
    }
    inline container_iterator_interface &
    operator-=(const difference_type count) noexcept(NO_EXCEPT) {
        return this->pos() -= count, *this;
    }
    inline difference_type
    operator-(const container_iterator_interface &other) const
        noexcept(NO_EXCEPT) {
        return this->pos() - other.pos();
    }
    inline bool operator<(const container_iterator_interface &other) const
        noexcept(NO_EXCEPT) {
        return *this - other < 0;
    }
    inline bool operator>(const container_iterator_interface &other) const
        noexcept(NO_EXCEPT) {
        return *this - other > 0;
    }
    inline bool operator<=(const container_iterator_interface &other) const
        noexcept(NO_EXCEPT) {
        return not(*this > other);
    }
    inline bool operator>=(const container_iterator_interface &other) const
        noexcept(NO_EXCEPT) {
        return not(*this < other);
    }
    inline bool operator!=(const container_iterator_interface &other) const
        noexcept(NO_EXCEPT) {
        return this->ref() != other.ref() or *this < other or *this > other;
    }
    inline bool operator==(const container_iterator_interface &other) const
        noexcept(NO_EXCEPT) {
        return not(*this != other);
    }
};
template <class I, std::enable_if_t<std::is_base_of_v<
                       random_access_iterator_base<typename I::value_type>, I>>
                       * = nullptr>
inline I
operator+(I itr, const typename I::difference_type count) noexcept(NO_EXCEPT) {
    return itr += count, itr;
}
template <class I, std::enable_if_t<std::is_base_of_v<
                       random_access_iterator_base<typename I::value_type>, I>>
                       * = nullptr>
inline I
operator-(I itr, const typename I::difference_type count) noexcept(NO_EXCEPT) {
    return itr -= count, itr;
}
} // namespace internal
} // namespace lib
/* [internal/iterator.hpp] */
/* #expanded [internal/range_reference.hpp] */
#include <cassert>
#include <iterator>
namespace lib {
namespace internal {
template <class Super> struct range_reference {
    using size_type = typename Super::size_type;
    using iterator = typename Super::iterator;

  protected:
    Super *const _super;
    const size_type _begin, _end;
    range_reference(Super *const super, const size_type begin,
                    const size_type end) noexcept(NO_EXCEPT)
        : _super(super), _begin(begin), _end(end) {}

  public:
    inline iterator begin() const noexcept(NO_EXCEPT) {
        return std::next(_super->begin(), this->_begin);
    }
    inline iterator end() const noexcept(NO_EXCEPT) {
        return std::next(_super->begin(), this->_end);
    }
    inline size_type size() const noexcept(NO_EXCEPT) {
        return this->_end - this->_begin;
    }

  protected:
    inline range_reference sub_range(size_type l, size_type r) const
        noexcept(NO_EXCEPT) {
        l = _super->_positivize_index(l), r = _super->_positivize_index(r);
        assert(0 <= l and l <= r and r <= this->size());
        return range_reference(_super, this->_begin + l, this->_begin + r);
    }

  public:
    template <lib::interval rng = lib::interval::right_open>
    inline range_reference range(const size_type l, const size_type r) const
        noexcept(NO_EXCEPT) {
        if constexpr(rng == lib::interval::right_open)
            return this->sub_range(l, r);
        if constexpr(rng == lib::interval::left_open)
            return this->sub_range(l + 1, r + 1);
        if constexpr(rng == lib::interval::open)
            return this->sub_range(l + 1, r);
        if constexpr(rng == lib::interval::closed)
            return this->sub_range(l, r + 1);
    }
    inline range_reference range() const noexcept(NO_EXCEPT) {
        return range_reference(this->_begin, this->_end);
    }
    inline range_reference operator()(const size_type l,
                                      const size_type r) const
        noexcept(NO_EXCEPT) {
        return this->sub_range(l, r);
    }
    inline range_reference subseq(const size_type p, const size_type c) const
        noexcept(NO_EXCEPT) {
        return this->sub_range(p, p + c);
    }
    inline range_reference subseq(const size_type p) const noexcept(NO_EXCEPT) {
        return this->sub_range(p, this->size());
    }
};
} // namespace internal
} // namespace lib
/* [internal/range_reference.hpp] */
/* #expanded [iterable/compression.hpp] */
#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
namespace lib {
template <class T = i64, class container = std::vector<internal::size_t>>
struct compression : container {
    using size_type = internal::size_t;

  protected:
    std::vector<T> values;

  public:
    explicit compression() noexcept(NO_EXCEPT) {}
    template <class I>
    compression(const I first, const I last) noexcept(NO_EXCEPT) {
        this->values.assign(first, last);
        std::sort(this->values.begin(), this->values.end());
        this->values.erase(
            std::unique(this->values.begin(), this->values.end()),
            this->values.end());
        this->resize(std::distance(first, last));
        for(auto itr = std::begin(*this), val = this->values.begin(), e = first;
            e != last; ++itr, ++val, ++e) {
            *itr = this->rank(*e);
        }
    }
    inline size_type rank(const T &val) const noexcept(NO_EXCEPT) {
        return std::distance(
            this->values.begin(),
            std::lower_bound(this->values.begin(), this->values.end(), val));
    }
    inline size_type rank2(const T &val) const noexcept(NO_EXCEPT) {
        return std::distance(this->values.begin(),
                             std::upper_bound(this->values.begin(),
                                              this->values.end(), val)) -
               1;
    }
    inline T value(const size_type rank) const noexcept(NO_EXCEPT) {
        return this->values[rank];
    }
    inline T operator()(const internal::size_t val) const noexcept(NO_EXCEPT) {
        return this->values[val];
    }
};
} // namespace lib
/* [iterable/compression.hpp] */
/* #expanded [numeric/bit.hpp] */
#include <cstdint>
#include <limits>
#include <type_traits>
namespace lib {
#define LIB_STATUC_ASSERT_UNSIGNED(T)                                          \
    static_assert(std::is_unsigned_v<T>, "only unsigned type is supported")
template <class T>
inline constexpr int countl_zero(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    using ull = unsigned long long;
    using ul = unsigned long;
    using u = unsigned;
    constexpr int DIGITS = std::numeric_limits<T>::digits;
    if(v == 0)
        return DIGITS;
    constexpr int DIGITS_ULL = std::numeric_limits<ull>::digits;
    constexpr int DIGITS_UL = std::numeric_limits<ul>::digits;
    constexpr int DIGITS_U = std::numeric_limits<u>::digits;
    if constexpr(DIGITS <= DIGITS_U)
        return __builtin_clz(v) - DIGITS_U + DIGITS;
    if constexpr(DIGITS <= DIGITS_UL)
        return __builtin_clzl(v) - DIGITS_UL + DIGITS;
    if constexpr(DIGITS <= DIGITS_ULL)
        return __builtin_clzll(v) - DIGITS_ULL + DIGITS;
    else {
        static_assert(DIGITS <= DIGITS_ULL << 1);
        constexpr ull MAX_ULL = std::numeric_limits<ull>::max();
        const ull high = v >> DIGITS_ULL;
        const ull low = v & MAX_ULL;
        if(high > 0)
            return __builtin_clzll(high) - (DIGITS_ULL << 1) + DIGITS;
        return __builtin_clzll(low) - DIGITS_ULL + DIGITS;
    }
}
template <class T>
inline constexpr int bit_width(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    return std::numeric_limits<T>::digits - countl_zero(v);
}
template <class T>
inline constexpr int highest_bit_pos(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    return bit_width(v) - 1;
}
template <class T> inline constexpr T bit_ceil(const T v) noexcept(NO_EXCEPT) {
    LIB_STATUC_ASSERT_UNSIGNED(T);
    if(v <= 1U)
        return 1;
    if constexpr(std::is_same_v<T, decltype(+v)>)
        return T{1} << bit_width<T>(v - 1);
    else {
        constexpr int d = std::numeric_limits<unsigned>::digits -
                          std::numeric_limits<T>::digits;
        return T(1U << bit_width<T>(v - 1) + d) >> d;
    }
}
#undef LIB_STATUC_ASSERT_UNSIGNED
} // namespace lib
/* [numeric/bit.hpp] */
/* #expanded [data_structure/bit_vector.hpp] */
#include <cstdint>
#include <immintrin.h>
#include <iterator>
#include <vector>
namespace lib {
struct bit_vector {
    using size_type = internal::size_t;

  private:
    static constexpr size_type w = 64;
    std::vector<std::uint64_t> _block;
    std::vector<size_type> _count;
    size_type _n, _zeros;

  public:
    bit_vector(const size_type _n = 0) noexcept(NO_EXCEPT) { this->init(_n); }
    template <class I>
    explicit bit_vector(const I first, const I last) noexcept(NO_EXCEPT)
        : bit_vector(std::distance(first, last)) {
        size_type pos = 0;
        for(auto itr = first; itr != last; ++pos, ++itr)
            if(*itr)
                this->set(pos);
    }
    template <class T>
    bit_vector(const std::initializer_list<T> &init_list) noexcept(NO_EXCEPT)
        : bit_vector(std::begin(init_list), std::end(init_list)) {}
    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n; }
    inline size_type zeros() const noexcept(NO_EXCEPT) { return this->_zeros; }
    inline size_type ones() const noexcept(NO_EXCEPT) {
        return this->_n - this->_zeros;
    }
    inline void set(const size_type k) noexcept(NO_EXCEPT) {
        this->_block[k / w] |= 1LL << (k % w);
    }
    inline bool get(const size_type k) const noexcept(NO_EXCEPT) {
        return std::uint32_t(this->_block[k / w] >> (k % w)) & 1U;
    }
    __attribute__((optimize("O3", "unroll-loops"))) void
    init(const int _n) noexcept(NO_EXCEPT) {
        this->_n = this->_zeros = _n;
        this->_block.resize(_n / w + 1, 0);
        this->_count.resize(this->_block.size(), 0);
    }
    __attribute__((target("popcnt"))) void build() noexcept(NO_EXCEPT) {
        for(auto k = 1UL; k < this->_block.size(); ++k)
            this->_count[k] =
                this->_count[k - 1] + _mm_popcnt_u64(this->_block[k - 1]);
        this->_zeros = this->rank0(this->_n);
    }
    __attribute__((target("bmi2,popcnt"))) inline size_type
    rank1(const size_type k) const noexcept(NO_EXCEPT) {
        return this->_count[k / w] +
               _mm_popcnt_u64(_bzhi_u64(this->_block[k / w], k % w));
    }
    inline size_type rank0(size_type k) const noexcept(NO_EXCEPT) {
        return k - this->rank1(k);
    }
    inline size_type rank(const bool bit, const size_type k) const
        noexcept(NO_EXCEPT) {
        return bit ? this->rank1(k) : this->rank0(k);
    }
    __attribute__((target("bmi2,popcnt"))) inline size_type
    select(const bool bit, const size_type rank) const noexcept(NO_EXCEPT) {
        if(!bit and rank > this->zeros()) {
            return this->_n + 1;
        }
        if(bit and rank > this->ones()) {
            return this->_n + 1;
        }
        size_type block_pos = 0;
        {
            size_type ng = -1, ok = this->_count.size();
            while(ok - ng > 1) {
                size_type mid = (ng + ok) / 2;
                size_type cnt = this->_count[mid];
                if(!bit)
                    cnt = mid * w - cnt;
                (cnt >= rank ? ok : ng) = mid;
            }
            block_pos = ok;
        }
        const size_type base_index = (block_pos - 1) * w;
        const size_type count = this->_count[block_pos - 1];
        const std::uint64_t block = this->_block[block_pos - 1];
        size_type ng = -1, ok = w;
        while(ok - ng > 1) {
            const size_type mid = (ok + ng) / 2;
            size_type r = count + _mm_popcnt_u64(_bzhi_u64(block, mid));
            if(!bit)
                r = base_index + mid - r;
            (r >= rank ? ok : ng) = mid;
        }
        return base_index + ok;
    }
    inline size_type select0(const size_type k) const noexcept(NO_EXCEPT) {
        return this->select(0, k);
    }
    inline size_type select1(const size_type k) const noexcept(NO_EXCEPT) {
        return this->select(1, k);
    }
    struct iterator
        : virtual internal::container_iterator_interface<bool, bit_vector> {
        iterator(const bit_vector *const ref,
                 const size_type pos) noexcept(NO_EXCEPT)
            : internal::container_iterator_interface<bool, bit_vector>(ref,
                                                                       pos) {}
        inline bool operator*() const noexcept(NO_EXCEPT) {
            return this->ref()->get(this->pos());
        }
    };
    inline iterator begin() const noexcept(NO_EXCEPT) {
        return iterator(this, 0);
    }
    inline iterator end() const noexcept(NO_EXCEPT) {
        return iterator(this, this->size());
    }
};
} // namespace lib
/* [data_structure/bit_vector.hpp] */
namespace lib {
namespace internal {
namespace wavelet_matrix_impl {
template <class T, class dict_type> struct base {
    using size_type = internal::size_t;

  private:
    size_type _n, _bits;
    std::vector<bit_vector> _index;
    std::vector<std::vector<T>> _sum;
    dict_type _first_pos;
    T _max = 0;

  public:
    base() {}
    template <class I> base(const I first, const I last) noexcept(NO_EXCEPT) {
        this->build(first, last);
    }
    template <class U>
    base(const std::initializer_list<U> &init_list) noexcept(NO_EXCEPT)
        : base(ALL(init_list)) {}
    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n; }
    inline size_type bits() const noexcept(NO_EXCEPT) { return this->_bits; }
    template <class I>
    __attribute__((optimize("O3"))) void
    build(const I first, const I last) noexcept(NO_EXCEPT) {
        this->_n = std::distance(first, last);
        this->_max = first == last ? -1 : *std::max_element(first, last);
        this->_bits = bit_width<std::make_unsigned_t<T>>(this->_max + 1);
        this->_index.assign(this->_bits, this->_n);
        std::vector<T> bit(first, last), nxt(this->_n);
        this->_sum.assign(this->_bits + 1, std::vector<T>(this->_n + 1));
        {
            size_type i = 0;
            for(auto itr = first; itr != last; ++i, ++itr) {
                this->_sum[this->_bits][i + 1] =
                    this->_sum[this->_bits][i] + *itr;
            }
        }
        REPD(h, this->_bits) {
            std::vector<size_type> vals;
            for(size_type i = 0; i < this->_n; ++i) {
                if((bit[i] >> h) & 1)
                    this->_index[h].set(i);
            }
            this->_index[h].build();
            std::array<typename std::vector<T>::iterator, 2> itrs{
                std::begin(nxt),
                std::next(std::begin(nxt), this->_index[h].zeros())};
            for(size_type i = 0; i < this->_n; ++i)
                *itrs[this->_index[h].get(i)]++ = bit[i];
            REP(i, this->_n) this->_sum[h][i + 1] = this->_sum[h][i] + nxt[i];
            std::swap(bit, nxt);
        }
        REPD(i, this->_n) this->_first_pos[bit[i]] = i;
    }

  protected:
    inline T get(const size_type k) const noexcept(NO_EXCEPT) {
        return this->_sum[this->_bits][k + 1] - this->_sum[this->_bits][k];
    }
    size_type select(const T &v, const size_type rank) const
        noexcept(NO_EXCEPT) {
        if(v > this->_max)
            return this->_n;
        if(this->_first_pos.count(v) == 0)
            return this->_n;
        size_type pos = this->_first_pos.at(v) + rank + 1;
        REP(h, this->_bits) {
            const bool bit = (v >> h) & 1;
            if(bit)
                pos -= this->_index[h].zeros();
            pos = this->_index[h].select(bit, pos);
        }
        return pos - 1;
    }
    inline T kth_smallest(size_type l, size_type r, size_type k) const
        noexcept(NO_EXCEPT) {
        T val = 0;
        for(size_type h = this->_bits - 1; h >= 0; --h) {
            size_type l0 = this->_index[h].rank0(l),
                      r0 = this->_index[h].rank0(r);
            if(k < r0 - l0) {
                l = l0, r = r0;
            } else {
                k -= r0 - l0;
                val |= T{1} << h;
                l += this->_index[h].zeros() - l0;
                r += this->_index[h].zeros() - r0;
            }
        }
        return val;
    }
    inline size_type kth_smallest_index(size_type l, size_type r,
                                        size_type k) const noexcept(NO_EXCEPT) {
        T val = 0;
        for(size_type h = this->_bits - 1; h >= 0; --h) {
            size_type l0 = this->_index[h].rank0(l),
                      r0 = this->_index[h].rank0(r);
            if(k < r0 - l0) {
                l = l0, r = r0;
            } else {
                k -= r0 - l0;
                val |= T{1} << h;
                l += this->_index[h].zeros() - l0;
                r += this->_index[h].zeros() - r0;
            }
        }
        size_type left = 0;
        REPD(h, this->_bits) {
            const bool bit = (val >> h) & 1;
            left = this->_index[h].rank(bit, left);
            if(bit)
                left += this->_index[h].zeros();
        }
        return this->select(val, l + k - left);
    }
    inline T kth_largest(const size_type l, const size_type r,
                         const size_type k) const noexcept(NO_EXCEPT) {
        return this->kth_smallest(l, r, r - l - k - 1);
    }
    inline size_type kth_largest_index(const size_type l, const size_type r,
                                       const size_type k) const
        noexcept(NO_EXCEPT) {
        return this->kth_smallest_index(l, r, r - l - k - 1);
    }
    inline std::pair<size_type, size_type>
    succ0(const size_type l, const size_type r, const size_type h) const
        noexcept(NO_EXCEPT) {
        return std::make_pair(this->_index[h].rank0(l),
                              this->_index[h].rank0(r));
    }
    inline std::pair<size_type, size_type>
    succ1(const size_type l, const size_type r, const size_type h) const
        noexcept(NO_EXCEPT) {
        size_type l0 = this->_index[h].rank0(l);
        size_type r0 = this->_index[h].rank0(r);
        size_type vals = this->_index[h].zeros();
        return std::make_pair(l + vals - l0, r + vals - r0);
    }
    T sum_in_range(const size_type l, const size_type r, const T &x, const T &y,
                   const T &cur, const size_type bit) const
        noexcept(NO_EXCEPT) {
        if(l == r)
            return 0;
        if(bit == -1) {
            if(x <= cur and cur < y)
                return cur * (r - l);
            return 0;
        }
        const T &nxt = (T{1} << bit) | cur;
        const T &ones = ((T{1} << bit) - 1) | nxt;
        if(ones < x or y <= cur)
            return 0;
        if(x <= cur and ones < y)
            return this->_sum[bit + 1][r] - this->_sum[bit + 1][l];
        const size_type l0 = this->_index[bit].rank0(l),
                        r0 = this->_index[bit].rank0(r);
        const size_type l1 = l - l0, r1 = r - r0;
        return this->sum_in_range(l0, r0, x, y, cur, bit - 1) +
               this->sum_in_range(this->_index[bit].zeros() + l1,
                                  this->_index[bit].zeros() + r1, x, y, nxt,
                                  bit - 1);
    }
    inline T sum_in_range(const size_type l, const size_type r, const T &x,
                          const T &y) const noexcept(NO_EXCEPT) {
        return this->sum_in_range(l, r, x, y, 0, this->_bits - 1);
    }
    inline T sum_under(const size_type l, const size_type r, const T &v) const
        noexcept(NO_EXCEPT) {
        return this->sum_in_range(l, r, 0, v);
    }
    inline T sum_over(const size_type l, const size_type r, const T &v) const
        noexcept(NO_EXCEPT) {
        return this->sum_in_range(l, r, v + 1, std::numeric_limits<T>::max());
    }
    inline T sum_or_under(const size_type l, const size_type r,
                          const T &v) const noexcept(NO_EXCEPT) {
        return this->sum_in_range(l, r, 0, v + 1);
    }
    inline T sum_or_over(const size_type l, const size_type r, const T &v) const
        noexcept(NO_EXCEPT) {
        return this->sum_in_range(l, r, v, std::numeric_limits<T>::max());
    }
    inline T sum(const size_type l, const size_type r) const
        noexcept(NO_EXCEPT) {
        return this->_sum[this->_bits][r] - this->_sum[this->_bits][l];
    }
    inline size_type count_under(size_type l, size_type r, const T &y) const
        noexcept(NO_EXCEPT) {
        if(y >= (T{1} << this->_bits))
            return r - l;
        size_type res = 0;
        REPD(h, this->_bits) {
            bool f = (y >> h) & 1;
            size_type l0 = this->_index[h].rank0(l),
                      r0 = this->_index[h].rank0(r);
            if(f) {
                res += r0 - l0;
                l += this->_index[h].zeros() - l0;
                r += this->_index[h].zeros() - r0;
            } else {
                l = l0;
                r = r0;
            }
        }
        return res;
    }
    inline size_type count_in_range(const size_type l, const size_type r,
                                    const T &x, const T &y) const
        noexcept(NO_EXCEPT) {
        return this->count_under(l, r, y) - this->count_under(l, r, x);
    }
    inline size_type count_or_under(size_type l, size_type r, const T &v) const
        noexcept(NO_EXCEPT) {
        return this->count_under(l, r, v + 1);
    }
    inline size_type count_over(size_type l, size_type r, const T &v) const
        noexcept(NO_EXCEPT) {
        return this->count_or_over(l, r, v + 1);
    }
    inline size_type count_or_over(size_type l, size_type r, const T &v) const
        noexcept(NO_EXCEPT) {
        return r - l - this->count_under(l, r, v);
    }
    inline size_type count_equal_to(const size_type l, const size_type r,
                                    const T &v) const noexcept(NO_EXCEPT) {
        return this->count_under(l, r, v + 1) - this->count_under(l, r, v);
    }
    inline std::optional<T> next(const size_type l, const size_type r,
                                 const T &v, const size_type k) const
        noexcept(NO_EXCEPT) {
        const size_type rank = this->count_under(l, r, v) + k;
        return (rank < 0 or rank >= r - l)
                   ? std::optional<T>{}
                   : std::optional<T>(this->kth_smallest(l, r, rank));
    }
    inline std::optional<T> prev(const size_type l, const size_type r,
                                 const T &v, const size_type k) const
        noexcept(NO_EXCEPT) {
        const size_type rank = this->count_over(l, r, v) + k;
        return (rank < 0 or rank >= r - l)
                   ? std::optional<T>{}
                   : std::optional<T>(this->kth_largest(l, r, rank));
    }
};
} // namespace wavelet_matrix_impl
} // namespace internal
template <class T, class dict_type = std::unordered_map<T, internal::size_t>>
struct compressed_wavelet_matrix;
template <class T, class dict_type = std::unordered_map<T, internal::size_t>>
struct wavelet_matrix : internal::wavelet_matrix_impl::base<T, dict_type> {
    using compressed = compressed_wavelet_matrix<T, dict_type>;

  private:
    using base = typename internal::wavelet_matrix_impl::base<T, dict_type>;

  public:
    using value_type = T;
    using size_type = typename base::size_type;

  protected:
    inline size_type _positivize_index(const size_type p) const
        noexcept(NO_EXCEPT) {
        return p < 0 ? this->size() + p : p;
    }

  public:
    using base::base;
    bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }
    inline T get(size_type p) const noexcept(NO_EXCEPT) {
        p = this->_positivize_index(p), assert(0 <= p && p < this->size());
        return this->base::get(p);
    }
    inline T operator[](const size_type p) const noexcept(NO_EXCEPT) {
        return this->get(p);
    }
    inline size_type select(const T &v, const size_type p) const
        noexcept(NO_EXCEPT) {
        return this->base::select(v, p);
    }
    struct iterator;
    struct range_reference;
    template <lib::interval rng = lib::interval::right_open>
    inline range_reference range(const size_type l, const size_type r) const
        noexcept(NO_EXCEPT) {
        if constexpr(rng == lib::interval::right_open)
            return range_reference(this, l, r);
        if constexpr(rng == lib::interval::left_open)
            return range_reference(this, l + 1, r + 1);
        if constexpr(rng == lib::interval::open)
            return range_reference(this, l + 1, r);
        if constexpr(rng == lib::interval::closed)
            return range_reference(this, l, r + 1);
    }
    inline range_reference range() const noexcept(NO_EXCEPT) {
        return range_reference(this, 0, this->size());
    }
    inline range_reference operator()(const size_type l,
                                      const size_type r) const
        noexcept(NO_EXCEPT) {
        return range_reference(this, l, r);
    }
    inline range_reference subseq(const size_type p, const size_type c) const
        noexcept(NO_EXCEPT) {
        return range_reference(this, p, p + c);
    }
    inline range_reference subseq(const size_type p) const noexcept(NO_EXCEPT) {
        return range_reference(this, p, this->size());
    }
    struct range_reference : internal::range_reference<const wavelet_matrix> {
        range_reference(const wavelet_matrix *const super, const size_type l,
                        const size_type r) noexcept(NO_EXCEPT)
            : internal::range_reference<const wavelet_matrix>(
                  super, super->_positivize_index(l),
                  super->_positivize_index(r)) {
            assert(0 <= this->_begin && this->_begin <= this->_end &&
                   this->_end <= this->_super->size());
        }
        inline T get(const size_type k) const noexcept(NO_EXCEPT) {
            k = this->_super->_positivize_index(k);
            assert(0 <= k and k < this->size());
            return this->_super->get(this->_begin + k);
        }
        inline T operator[](const size_type k) const noexcept(NO_EXCEPT) {
            return this->get(k);
        }
        inline T kth_smallest(const size_type k) const noexcept(NO_EXCEPT) {
            assert(0 <= k && k < this->size());
            return this->_super->base::kth_smallest(this->_begin, this->_end,
                                                    k);
        }
        inline auto kth_smallest_element(const size_type k) const
            noexcept(NO_EXCEPT) {
            if(k == this->size())
                return this->end();
            assert(0 <= k && k < this->size());
            return std::next(this->_super->begin(),
                             this->_super->base::kth_smallest_index(
                                 this->_begin, this->_end, k));
        }
        inline T kth_largest(const size_type k) const noexcept(NO_EXCEPT) {
            assert(0 <= k && k < this->size());
            return this->_super->base::kth_largest(this->_begin, this->_end, k);
        }
        inline auto kth_largest_element(const size_type k) const
            noexcept(NO_EXCEPT) {
            if(k == this->size())
                return this->end();
            assert(0 <= k && k < this->size());
            return std::next(this->_super->begin(),
                             this->_super->base::kth_largest_index(
                                 this->_begin, this->_end, k));
        }
        inline T min() const noexcept(NO_EXCEPT) {
            return this->kth_smallest(0);
        }
        inline T max() const noexcept(NO_EXCEPT) {
            return this->kth_largest(0);
        }
        inline T median() const noexcept(NO_EXCEPT) {
            return this->kth_smallest(this->size() / 2);
        }
        inline T sum_in_range(const T &x, const T &y) const
            noexcept(NO_EXCEPT) {
            return this->_super->base::sum_in_range(this->_begin, this->_end, x,
                                                    y);
        }
        inline T sum_under(const T &v) const noexcept(NO_EXCEPT) {
            return this->_super->base::sum_under(this->_begin, this->_end, v);
        }
        inline T sum_over(const T &v) const noexcept(NO_EXCEPT) {
            return this->_super->base::sum_over(this->_begin, this->_end, v);
        }
        inline T sum_or_under(const T &v) const noexcept(NO_EXCEPT) {
            return this->_super->base::sum_or_under(this->_begin, this->_end,
                                                    v);
        }
        inline T sum_or_over(const T &v) const noexcept(NO_EXCEPT) {
            return this->_super->base::sum_or_over(this->_begin, this->_end, v);
        }
        inline T sum(const T &x, const T &y) const noexcept(NO_EXCEPT) {
            return this->_super->base::sum_in_range(this->_begin, this->_end, x,
                                                    y);
        }
        inline T sum() const noexcept(NO_EXCEPT) {
            return this->_super->base::sum(this->_begin, this->_end);
        }
        template <comp com>
        inline size_type sum(const T &v) const noexcept(NO_EXCEPT) {
            if constexpr(com == comp::under)
                return this->sum_under(v);
            if constexpr(com == comp::over)
                return this->sum_over(v);
            if constexpr(com == comp::or_under)
                return this->sum_or_under(v);
            if constexpr(com == comp::or_over)
                return this->sum_or_over(v);
            assert(false);
        }
        inline size_type count_in_range(const T &x, const T &y) const
            noexcept(NO_EXCEPT) {
            return this->_super->base::count_in_range(this->_begin, this->_end,
                                                      x, y);
        }
        inline size_type count_under(const T &v) const noexcept(NO_EXCEPT) {
            return this->_super->base::count_under(this->_begin, this->_end, v);
        }
        inline size_type count_over(const T &v) const noexcept(NO_EXCEPT) {
            return this->_super->base::count_over(this->_begin, this->_end, v);
        }
        inline size_type count_or_under(const T &v) const noexcept(NO_EXCEPT) {
            return this->_super->base::count_or_under(this->_begin, this->_end,
                                                      v);
        }
        inline size_type count_or_over(const T &v) const noexcept(NO_EXCEPT) {
            return this->_super->base::count_or_over(this->_begin, this->_end,
                                                     v);
        }
        template <comp com = comp::equal_to>
        inline size_type count(const T &v) const noexcept(NO_EXCEPT) {
            if constexpr(com == comp::eq)
                return this->_super->count_equal_to(this->_begin, this->_end,
                                                    v);
            if constexpr(com == comp::under)
                return this->count_under(v);
            if constexpr(com == comp::over)
                return this->count_over(v);
            if constexpr(com == comp::or_under)
                return this->count_or_under(v);
            if constexpr(com == comp::or_over)
                return this->count_or_over(v);
            assert(false);
        }
        inline auto next_element(const T &v, const size_type k = 0) const
            noexcept(NO_EXCEPT) {
            return this->kth_smallest_element(
                std::clamp(this->count_under(v) + k, 0, this->size()));
        }
        inline auto prev_element(const T &v, const size_type k = 0) const
            noexcept(NO_EXCEPT) {
            return this->kth_smallest_element(
                std::clamp(this->count_over(v) - k, 0, this->size()));
        }
        inline std::optional<T> next(const T &v, const size_type k = 0) const
            noexcept(NO_EXCEPT) {
            return this->_super->base::next(this->_begin, this->_end, v, k);
        }
        inline std::optional<T> prev(const T &v, const size_type k = 0) const
            noexcept(NO_EXCEPT) {
            return this->_super->base::prev(this->_begin, this->_end, v, k);
        }
    };
    inline T kth_smallest(const size_type k) const noexcept(NO_EXCEPT) {
        return this->range().kth_smallest(k);
    }
    inline T kth_smallest_element(const size_type k) const noexcept(NO_EXCEPT) {
        return this->range().kth_smallest_element(k);
    }
    inline T kth_largest(const size_type k) const noexcept(NO_EXCEPT) {
        return this->range().kth_largest(k);
    }
    inline T kth_largest_element(const size_type k) const noexcept(NO_EXCEPT) {
        return this->range().kth_largest_element(k);
    }
    inline T min() const noexcept(NO_EXCEPT) {
        return this->range().kth_smallest(0);
    }
    inline T max() const noexcept(NO_EXCEPT) {
        return this->range().kth_largest(0);
    }
    inline T median() const noexcept(NO_EXCEPT) {
        return this->range().median();
    }
    inline T sum_in_range(const T &x, const T &y) const noexcept(NO_EXCEPT) {
        return this->range().sum_in_range(x, y);
    }
    inline T sum_under(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().sum_under(v);
    }
    inline T sum_over(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().sum_over(v);
    }
    inline T sum_or_under(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().sum_or_under(v);
    }
    inline T sum_or_over(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().sum_or_over(v);
    }
    inline T sum(const T &x, const T &y) const noexcept(NO_EXCEPT) {
        return this->range().sum_in_range(x, y);
    }
    inline T sum() const noexcept(NO_EXCEPT) { return this->range().sum(); }
    template <comp com>
    inline size_type sum(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().template sum<com>(v);
    }
    inline size_type count_in_range(const T &x, const T &y) const
        noexcept(NO_EXCEPT) {
        return this->range().count_in_range(x, y);
    }
    inline size_type count_under(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().count_under(v);
    }
    inline size_type count_over(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().count_over(v);
    }
    inline size_type count_or_under(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().count_or_under(v);
    }
    inline size_type count_or_over(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().count_or_over(v);
    }
    inline size_type count(const T &x, const T &y) const noexcept(NO_EXCEPT) {
        return this->range().count(x, y);
    }
    template <comp com = comp::equal_to>
    inline size_type count(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().template count<com>(v);
    }
    inline auto next_element(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().next_element(v);
    }
    inline auto prev_element(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().prev_element(v);
    }
    inline std::optional<T> next(const T &v, const size_type k = 0) const
        noexcept(NO_EXCEPT) {
        return this->range().next(v, k);
    }
    inline std::optional<T> prev(const T &v, const size_type k = 0) const
        noexcept(NO_EXCEPT) {
        return this->range().prev(v, k);
    }

  protected:
    using iterator_interface =
        internal::container_iterator_interface<T, wavelet_matrix>;

  public:
    struct iterator : virtual iterator_interface {
        iterator(const wavelet_matrix *const ref,
                 const size_type pos) noexcept(NO_EXCEPT)
            : iterator_interface(ref, pos) {}
        inline T operator*() const noexcept(NO_EXCEPT) {
            return this->ref()->get(this->pos());
        }
        inline T operator[](
            const typename iterator_interface::difference_type count) const
            noexcept(NO_EXCEPT) {
            return *(*this + count);
        }
    };
    inline iterator begin() const noexcept(NO_EXCEPT) {
        return iterator(this, 0);
    }
    inline iterator end() const noexcept(NO_EXCEPT) {
        return iterator(this, this->size());
    }
};
template <class T, class dict_type>
struct compressed_wavelet_matrix
    : protected wavelet_matrix<typename compression<T>::size_type, dict_type> {
  protected:
    using core = wavelet_matrix<typename compression<T>::size_type, dict_type>;
    compression<T> compressed;

  public:
    using value_type = typename core::value_type;
    using size_type = typename core::size_type;
    template <class I>
    compressed_wavelet_matrix(const I first, const I last) noexcept(NO_EXCEPT) {
        this->build(first, last);
    }
    template <class I>
    void build(const I first, const I last) noexcept(NO_EXCEPT) {
        this->compressed = compression<T>(first, last);
        this->core::build(ALL(this->compressed));
    }
    inline T get(const size_type k) const noexcept(NO_EXCEPT) {
        return this->compressed(this->core::get(k));
    }
    inline size_type operator[](const size_type k) const noexcept(NO_EXCEPT) {
        return this->core::get(k);
    }
    struct iterator;
    struct range_reference;
    template <lib::interval rng = lib::interval::right_open>
    inline range_reference range(const size_type l, const size_type r) const
        noexcept(NO_EXCEPT) {
        if constexpr(rng == lib::interval::right_open)
            return range_reference(this, l, r);
        if constexpr(rng == lib::interval::left_open)
            return range_reference(this, l + 1, r + 1);
        if constexpr(rng == lib::interval::open)
            return range_reference(this, l + 1, r);
        if constexpr(rng == lib::interval::closed)
            return range_reference(this, l, r + 1);
    }
    inline range_reference range() const noexcept(NO_EXCEPT) {
        return range_reference(this, 0, this->size());
    }
    inline range_reference operator()(const size_type l,
                                      const size_type r) const
        noexcept(NO_EXCEPT) {
        return range_reference(this, l, r);
    }
    inline range_reference subseq(const size_type p, const size_type c) const
        noexcept(NO_EXCEPT) {
        return range_reference(this, p, p + c);
    }
    inline range_reference subseq(const size_type p) const noexcept(NO_EXCEPT) {
        return range_reference(this, p, this->size());
    }
    struct range_reference
        : internal::range_reference<const compressed_wavelet_matrix> {
        range_reference(const compressed_wavelet_matrix *const super,
                        const size_type l,
                        const size_type r) noexcept(NO_EXCEPT)
            : internal::range_reference<const compressed_wavelet_matrix>(
                  super, super->_positivize_index(l),
                  super->_positivize_index(r)) {
            assert(0 <= this->_begin && this->_begin <= this->_end &&
                   this->_end <= this->_super->size());
        }

      private:
        inline auto _range() const noexcept(NO_EXCEPT) {
            return this->_super->core::range(this->_begin, this->_end);
        }

      public:
        inline T get(const size_type k) const noexcept(NO_EXCEPT) {
            return this->_super->compressed(this->_range().get(k));
        }
        inline T operator[](const size_type k) const noexcept(NO_EXCEPT) {
            return this->get(k);
        }
        inline T kth_smallest(const size_type k) const noexcept(NO_EXCEPT) {
            return this->_super->compressed(this->_range().kth_smallest(k));
        }
        inline auto kth_smallest_element(const size_type k) const
            noexcept(NO_EXCEPT) {
            return std::next(
                this->_super->begin(),
                std::distance(this->_super->core::begin(),
                              this->_range().kth_smallest_element(k)));
        }
        inline T kth_largest(const size_type k) const noexcept(NO_EXCEPT) {
            return this->_super->compressed(this->_range().kth_largest(k));
        }
        inline auto kth_largest_element(const size_type k) const
            noexcept(NO_EXCEPT) {
            return std::next(
                this->_super->begin(),
                std::distance(this->_super->core::begin(),
                              this->_range().kth_largest_element(k)));
        }
        inline T min() const noexcept(NO_EXCEPT) {
            return this->kth_smallest(0);
        }
        inline T max() const noexcept(NO_EXCEPT) {
            return this->kth_largest(0);
        }
        inline T median() const noexcept(NO_EXCEPT) {
            return this->kth_smallest(this->size() / 2);
        }
        inline size_type count_in_range(const T &x, const T &y) const
            noexcept(NO_EXCEPT) {
            return this->_range().count_in_range(
                this->_super->compressed.rank(x),
                this->_super->compressed.rank(y));
        }
        inline size_type count_under(const T &v) const noexcept(NO_EXCEPT) {
            return this->_range().count_under(this->_super->compressed.rank(v));
        }
        inline size_type count_over(const T &v) const noexcept(NO_EXCEPT) {
            return this->_range().count_over(this->_super->compressed.rank2(v));
        }
        inline size_type count_or_under(const T &v) const noexcept(NO_EXCEPT) {
            return this->_range().count_or_under(
                this->_super->compressed.rank2(v));
        }
        inline size_type count_or_over(const T &v) const noexcept(NO_EXCEPT) {
            return this->_range().count_or_over(
                this->_super->compressed.rank(v));
        }
        template <comp com = comp::equal_to>
        inline size_type count(const T &v) const noexcept(NO_EXCEPT) {
            return this->_range().template count<com>(
                this->_super->compressed.rank(v));
        }
        inline auto next_element(const T &v, const size_type k = 0) const
            noexcept(NO_EXCEPT) {
            return this->kth_smallest_element(
                std::clamp(this->_range().count_under(
                               this->_super->compressed.rank(v) + k),
                           0, this->size()));
        }
        inline auto prev_element(const T &v, const size_type k = 0) const
            noexcept(NO_EXCEPT) {
            return this->kth_largest_element(
                std::clamp(this->_range().count_over(
                               this->_super->compressed.rank2(v) + k),
                           0, this->size()));
        }
        inline std::optional<T> next(const T &v, const size_type k = 0) const
            noexcept(NO_EXCEPT) {
            const std::optional<size_type> res =
                this->_range().next(this->_super->compressed.rank(v), k);
            if(res.has_value())
                return this->_super->compressed(res.value());
            return {};
        }
        inline std::optional<T> prev(const T &v, const size_type k = 0) const
            noexcept(NO_EXCEPT) {
            const std::optional<size_type> res =
                this->_range().prev(this->_super->compressed.rank2(v), k);
            if(res.has_value())
                return this->_super->compressed(res.value());
            return {};
        }
    };
    inline T kth_smallest(const size_type k) const noexcept(NO_EXCEPT) {
        return this->range().kth_smallest(k);
    }
    inline auto kth_smallest_element(const size_type k) const
        noexcept(NO_EXCEPT) {
        return this->range().kth_smallest_element(k);
    }
    inline T kth_largest(const size_type k) const noexcept(NO_EXCEPT) {
        return this->range().kth_largest(k);
    }
    inline auto kth_largest_element(const size_type k) const
        noexcept(NO_EXCEPT) {
        return this->range().kth_largest_element(k);
    }
    inline T min() const noexcept(NO_EXCEPT) {
        return this->range().kth_smallest(0);
    }
    inline T max() const noexcept(NO_EXCEPT) {
        return this->range().kth_largest(0);
    }
    inline T median() const noexcept(NO_EXCEPT) {
        return this->range().median();
    }
    inline size_type count_in_range(const T &x, const T &y) const
        noexcept(NO_EXCEPT) {
        return this->range().count_in_range(x, y);
    }
    inline size_type count_under(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().count_under(v);
    }
    inline size_type count_over(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().count_over(v);
    }
    inline size_type count_or_under(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().count_or_under(v);
    }
    inline size_type count_or_over(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().count_or_over(v);
    }
    template <comp com = comp::equal_to>
    inline size_type count(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().template count<com>(v);
    }
    inline auto next_element(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().next_element(v);
    }
    inline auto prev_element(const T &v) const noexcept(NO_EXCEPT) {
        return this->range().prev_element(v);
    }
    inline std::optional<T> next(const T &v, const size_type k = 0) const
        noexcept(NO_EXCEPT) {
        return this->range().next(v, k);
    }
    inline std::optional<T> prev(const T &v, const size_type k = 0) const
        noexcept(NO_EXCEPT) {
        return this->range().prev(v, k);
    }

  protected:
    using iterator_interface =
        internal::container_iterator_interface<T, compressed_wavelet_matrix>;

  public:
    struct iterator : virtual iterator_interface {
        iterator(const compressed_wavelet_matrix *const ref,
                 const size_type pos) noexcept(NO_EXCEPT)
            : iterator_interface(ref, pos) {}
        inline T operator*() const noexcept(NO_EXCEPT) {
            return this->ref()->get(this->pos());
        }
        inline T operator[](
            const typename iterator_interface::difference_type count) const
            noexcept(NO_EXCEPT) {
            return *(*this + count);
        }
    };
    inline iterator begin() const noexcept(NO_EXCEPT) {
        return iterator(this, 0);
    }
    inline iterator end() const noexcept(NO_EXCEPT) {
        return iterator(this, this->size());
    }
};
} // namespace lib
/* [data_structure/wavelet_matrix.hpp] */

signed main() {
    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    input >> a;

    lib::wavelet_matrix<int> data(ALL(a));
    debug(data);

    REP(q) {
        int l, r, x;
        std::cin >> l >> r >> x;
        print(data.range(l, r).count(x));
    }
}
