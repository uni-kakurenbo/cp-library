#include <atcoder/modint>
#include <bits/stdc++.h>
/* #expanded [template.hpp] */
/* #expanded [aliases.hpp] */
/* #expanded [types.hpp] */
using ll = long long;
using ull = unsigned long long;
using ld = long double;
/* [types.hpp] */
#define until(...) while(!(__VA_ARGS__))
#define ALL(x) std::begin((x)), std::end((x))
#define RALL(x) std::rbegin((x)), std::rend((x))
#define $F first
#define $S second
constexpr char ln = '\n';
constexpr char spc = ' ';
#include <utility>
constexpr std::pair<int, int> DIRS4[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
constexpr std::pair<int, int> DIRS8[] = {{-1, 0}, {-1, 1}, {0, 1},  {1, 1},
                                         {1, 0},  {1, -1}, {0, -1}, {-1, -1}};
template <class T1, class T2> inline auto mod(T1 x, T2 r) {
    return (x % r + r) % r;
}
template <class T1, class T2> inline bool chmax(T1 &a, T2 b) {
    return (a < b ? a = b, true : false);
}
template <class T1, class T2> inline bool chmin(T1 &a, T2 b) {
    return (a > b ? a = b, true : false);
}
/* [aliases.hpp] */
/* #expanded [iterations.hpp] */
/* #expanded [overload.hpp] */
#define $OVERLOAD2(arg0, arg1, cmd, ...) cmd
#define $OVERLOAD3(arg0, arg1, arg2, cmd, ...) cmd
#define $OVERLOAD4(arg0, arg1, arg2, arg3, cmd, ...) cmd
#define $OVERLOAD5(arg0, arg1, arg2, arg3, arg4, cmd, ...) cmd
/* [overload.hpp] */
#define LOOP(n) REPI($_, (n))
#define REPI(i, n) for(int i = 0, i##_length = int(n); i < i##_length; ++i)
#define REPF(i, l, r) for(auto i = (l), i##_last = (r); i != i##_last; ++i)
#define REPIS(i, l, r, s)                                                      \
    for(auto i = (l), i##_last = (r); i != i##_last; i += (s))
#define REPR(i, n) for(auto i = (n); --i >= 0;)
#define REPB(i, l, r) for(auto i = (r), i##_last = (l); --i >= i##_last;)
#define REPRS(i, l, r, s)                                                      \
    for(auto i = (r), i##_last = (l); (i -= (s)) >= i##_last;)
#define REP(...) $OVERLOAD4(__VA_ARGS__, REPIS, REPF, REPI, LOOP)(__VA_ARGS__)
#define REPD(...) $OVERLOAD4(__VA_ARGS__, REPRS, REPB, REPR)(__VA_ARGS__)
#define FORI(i, l, r) for(auto i = (l), i##_last = (r); i <= i##_last; ++i)
#define FORIS(i, l, r, s)                                                      \
    for(auto i = (l), i##_last = (r); i <= i##_last; i += (s))
#define FORR(i, l, r) for(auto i = (r), i##_last = (l); i >= i##_last; --i)
#define FORRS(i, l, r, s)                                                      \
    for(auto i = (r), i##_last = (l); i >= i##_last; i -= (s))
#define FOR(...) $OVERLOAD4(__VA_ARGS__, FORIS, FORI)(__VA_ARGS__)
#define FORD(...) $OVERLOAD4(__VA_ARGS__, FORRS, FORR)(__VA_ARGS__)
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
/* [iterations.hpp] */
/* #expanded [fast_io.hpp] */
#include <iostream>
#ifdef __GNUC__
__attribute__((constructor)) inline void fast_io() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr);
}
#else
inline void fast_io() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr);
}
#endif
/* [fast_io.hpp] */
#ifdef LOCAL_JUDGE
#include <debug>
#define debug(...)                                                             \
    debugger::debug(debugger::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
#define DEBUG(...)                                                             \
    do {                                                                       \
        debugger::DEBUG(nullptr,                                               \
                        "\033[3;35m#" + to_string(__LINE__) + "\033[m ");      \
        debugger::DEBUG(__VA_ARGS__);                                          \
        debugger::DEBUG(nullptr, "\033[m\n");                                  \
    } while(0);
#else
#define debug(...) ({ ; })
#define DEBUG(...) ({ ; })
#endif
/* #expanded [input.hpp] */
#include <atcoder/modint>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
/* #expanded [resolving_rank.hpp] */
namespace lib {
namespace internal {
template <int P> struct resolving_rank : resolving_rank<P - 1> {};
template <> struct resolving_rank<0> {};
} // namespace internal
} // namespace lib
/* [resolving_rank.hpp] */
template <class source = std::istream> struct input_adapter {
  private:
    template <class T>
    auto _set(lib::internal::resolving_rank<3>, T *const val)
        -> decltype(std::declval<source>() >> *val, 0) {
        *this->in >> *val;
        return 0;
    }
    template <class T>
    auto _set(lib::internal::resolving_rank<2>, T *const val)
        -> decltype(std::begin(*val), std::end(*val), 0) {
        (*this)(std::begin(*val), std::end(*val));
        return 0;
    }
    template <class T>
    auto _set(lib::internal::resolving_rank<1>, T *const val)
        -> decltype(val->first, val->second, 0) {
        (*this)(&*val);
        return 0;
    }
    template <class T>
    auto _set(lib::internal::resolving_rank<0>, T *const val)
        -> std::enable_if_t<atcoder::internal::is_modint<T>::value, int> {
        long long v;
        std::cin >> v;
        *val = {v};
        return 0;
    }

  protected:
    template <class T> source *set(T *const val) {
        this->_set(lib::internal::resolving_rank<3>{}, val);
        return this->in;
    }

  public:
    using char_type = typename source::char_type;
    source *in;
    input_adapter(source *in = &std::cin) : in(in) {}
    template <class T> inline input_adapter &operator>>(T &s) {
        this->set(&s);
        return *this;
    }
    template <class T> inline T one() {
        T val;
        *this >> val;
        return val;
    }
    template <class T> inline void operator()(T *const val) { *this >> *val; }
    template <class T, class... Args>
    inline void operator()(T *const head, Args *const... tail) {
        *this >> *head;
        (*this)(tail...);
    }
    template <class I, class = typename I::value_type>
    inline void operator()(I first, I last) {
        for(I itr = first; itr != last; ++itr)
            *this >> *itr;
    }
    template <class F, class S>
    inline void operator()(std::pair<F, S> *const p) {
        (*this)(&p->first, &p->second);
    }
};
/* [input.hpp] */
/* #expanded [output.hpp] */
#include <iostream>
#include <string>
#include <vector>

template <class destination = std::ostream, class Terminator = std::string,
          class Separator = std::string>
struct output_adapter {
  private:
    template <class T>
    auto _put(lib::internal::resolving_rank<2>, const T &val)
        -> decltype(std::declval<destination>() << val, 0) {
        *this->out << val;
        return 0;
    }
    template <class T>
    auto _put(lib::internal::resolving_rank<1>, const T &val)
        -> decltype(val.val(), 0) {
        *this->out << val.val();
        return 0;
    }
    template <class T>
    auto _put(lib::internal::resolving_rank<0>, const T &val)
        -> decltype(std::begin(val), std::end(val), 0) {
        (*this)(std::begin(val), std::end(val), false);
        return 0;
    }

  protected:
    template <class T> destination *put(const T &val) {
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
                   Separator separator = " ")
        : out(out), endline(endline), separator(separator) {}
    template <class T> inline output_adapter &operator<<(const T &s) {
        this->put(s);
        return *this;
    }
    template <class T = std::string> inline void operator()(const T &val = "") {
        *this << val << this->endline;
    }
    template <class T, class... Args>
    inline void operator()(const T &head, const Args &...tail) {
        *this << head << this->separator;
        (*this)(tail...);
    }
    template <class I, class = typename I::iterator_category>
    inline void operator()(const I first, const I last,
                           const bool terminate = true) {
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
    inline void operator()(const std::initializer_list<T> vals) {
        std::vector wrapped(vals.begin(), vals.end());
        (*this)(wrapped.begin(), wrapped.end());
    }
    template <class F, class S>
    inline void operator()(const std::pair<F, S> &p) {
        (*this)(p.first, p.second);
    }
};
/* [output.hpp] */
input_adapter _input;
output_adapter _print;
#define input _input
#define print _print
/* [template.hpp] */
/* #expanded [matrix.hpp] */
#include <vector>
/* #expanded [dev_assert.hpp] */
#ifdef LOCAL_JUDGE
#include <cassert>
#define dev_assert(...) assert(__VA_ARGS__)
#else
#define dev_assert(...) ({ ; })
#endif
/* [dev_assert.hpp] */
/* #expanded [grid.hpp] */
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

/* #expanded [dev_env.hpp] */
#ifdef LOCAL_JUDGE
constexpr bool DEV_ENV = true;
#else
constexpr bool DEV_ENV = false;
#endif
/* [dev_env.hpp] */
/* #expanded [types.hpp] */
#include <cstdint>
namespace lib {
namespace internal {
using size_t = std::int32_t;
}
} // namespace lib
/* [types.hpp] */
namespace lib {
namespace internal {
namespace grid_lib {
template <class T> struct interface {
    virtual void assign(const size_t, const size_t, const T &&) = 0;
    virtual void resize(const size_t, const size_t) = 0;
    virtual size_t height() const = 0;
    virtual size_t width() const = 0;
    virtual size_t id(const size_t, const size_t) const = 0;
    virtual T &operator()(const size_t, const size_t) = 0;
    virtual const T &operator()(const size_t, const size_t) const = 0;
};
template <class T> struct container_base : virtual interface<T> {
  private:
    size_t _h, _w;

  protected:
    inline void _validate_index(__attribute__((unused)) const size_t i,
                                __attribute__((unused)) const size_t j) const {
        dev_assert(0 <= i and i < this->height());
        dev_assert(0 <= j and j < this->width());
    }
    inline size_t _positivize_row_index(const size_t x) const {
        return x < 0 ? this->height() + x : x;
    }
    inline size_t _positivize_col_index(const size_t x) const {
        return x < 0 ? this->width() + x : x;
    }

  public:
    container_base() = default;
    container_base(const size_t _h, const size_t _w) : _h(_h), _w(_w) {}
    virtual void resize(const size_t h, const size_t w) override {
        this->_h = h, this->_w = w;
    }
    inline size_t height() const override { return this->_h; }
    inline size_t width() const override { return this->_w; }
    inline size_t id(const size_t i, const size_t j) const override {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return _i * this->width() + _j;
    }
};
template <class T, class Row = std::vector<T>, class base = std::vector<Row>>
struct container : base, container_base<T>, virtual interface<T> {
    container(const size_t n = 0) : container(n, n) {}
    container(const size_t h, const size_t w, const T &&val = T{})
        : base(h, Row(w, std::forward<const T>(val))), container_base<T>(h, w) {
    }
    container(const std::initializer_list<Row> init_list) : base(init_list) {
        const size_t rows = std::distance(ALL(init_list));
        const size_t first_cols = init_list.begin()->size();
        if constexpr(DEV_ENV) {
            ITR(init_row, init_list)
            dev_assert((size_t)init_row.size() == first_cols);
        }
        this->container_base<T>::resize(rows, first_cols);
    }
    inline void assign(const container &source) {
        this->resize(source.height(), source.width());
        this->base::assign(ALL(source));
    }
    inline void assign(const size_t h, const size_t w,
                       const T &&val = T{}) override {
        this->container_base<T>::resize(h, w);
        this->base::resize(h);
        ITRR(row, *this) row.assign(w, std::forward<const T>(val));
    }
    inline void resize(const size_t h, const size_t w) override {
        this->container_base<T>::resize(h, w);
        this->base::resize(h);
        ITRR(row, *this) row.resize(w);
    }
    inline T &operator()(const size_t i, const size_t j) override {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }
    inline const T &operator()(const size_t i, const size_t j) const override {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }
};
template <class T, class base = std::vector<T>>
struct unfolded_container : base, container_base<T>, virtual interface<T> {
    unfolded_container(size_t n = 0) : unfolded_container(n, n) {}
    unfolded_container(const size_t h, const size_t w, const T &&val = T{})
        : base(h * w, std::forward<const T>(val)), container_base<T>(h, w) {}
    unfolded_container(
        std::initializer_list<std::initializer_list<T>> init_list) {
        const size_t rows = std::distance(init_list.begin(), init_list.end());
        const size_t first_cols = init_list.begin()->size();
        this->resize(rows, first_cols);
        for(auto index = 0, itr = init_list.begin(), itr_end = init_list.end();
            itr != itr_end; ++itr) {
            dev_assert((size_t)itr->size() == first_cols);
            for(auto v = itr->begin(), v_end = itr->end(); v != v_end; ++v)
                (*this)[index++] = *v;
        }
    }
    inline void assign(const unfolded_container &source) {
        this->resize(source.height(), source.width());
        this->base::assign(ALL(source));
    }
    inline void assign(const size_t h, const size_t w,
                       const T &&val = T{}) override {
        this->container_base<T>::resize(h, w);
        this->base::assign(h * w, std::forward<const T>(val));
    }
    inline void resize(const size_t h, const size_t w) override {
        this->container_base<T>::resize(h, w);
        this->base::resize(h * w);
    }
    inline T &operator()(const size_t i, const size_t j) override {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        return (*this)[this->id(_i, _j)];
    }
    inline const T &operator()(const size_t i, const size_t j) const override {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        return (*this)[this->id(_i, _j)];
    }
};
} // namespace grid_lib
template <class T, class container>
struct grid_core : container, virtual grid_lib::interface<T> {
    using container::container;
    enum class InvertDirection { Vertical, Horizontal };
    enum class RotateDirection { CounterClockwise, Clockwise };
    template <class U = T, class Stream = std::istream>
    void inline read(Stream *const ist = &std::cin) {
        REP(i, this->height()) REP(j, this->width()) {
            U val;
            *ist >> val;
            (*this)(i, j) = val;
        }
    }
    template <InvertDirection DIRECT = InvertDirection::Vertical>
    inline grid_core &invert() {
        grid_core res(this->height(), this->width());
        REP(i, this->height()) REP(j, this->width()) {
            if constexpr(DIRECT == InvertDirection::Vertical) {
                res(i, j) = (*this)(this->height() - i - 1, j);
            } else {
                res(i, j) = (*this)(i, this->width() - j - 1);
            }
        }
        this->assign(res);
        return *this;
    }
    template <RotateDirection DIRECT = RotateDirection::Clockwise>
    inline grid_core &rotate(const size_t k) {
        grid_core res = *this;
        REP(i, k) { res = res.rotate<DIRECT>(); }
        this->assign(res);
        return *this;
    }
    template <RotateDirection DIRECT = RotateDirection::Clockwise>
    inline grid_core &rotate() {
        grid_core res(this->width(), this->height());
        REP(i, this->width()) REP(j, this->height()) {
            if constexpr(DIRECT == RotateDirection::Clockwise) {
                res(i, j) = (*this)(this->height() - j - 1, i);
            } else {
                res(i, j) = (*this)(j, this->width() - i - 1);
            }
        }
        this->assign(res);
        return *this;
    }
    inline grid_core &transpose() {
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
    internal::grid_core<T, internal::grid_lib::container<T, Row, base>>;
template <class T, class base = std::vector<T>>
using unfolded_grid =
    internal::grid_core<T, internal::grid_lib::unfolded_container<T, base>>;
} // namespace lib
/* [grid.hpp] */
/* #expanded [valarray.hpp] */
#include <algorithm>
#include <type_traits>
#include <valarray>

namespace lib {
template <class T> struct valarray : std::valarray<T> {
    using std::valarray<T>::valarray;
    valarray(const internal::size_t length, const T &&val = T{})
        : std::valarray<T>(std::forward<const T>(val), length) {}
    inline void reserve(const internal::size_t) {}
    template <class I,
              std::enable_if_t<std::is_same_v<
                  T, typename std::iterator_traits<I>::value_type>> * = nullptr>
    inline void assign(const I first, const I last) {
        this->resize(std::distance(first, last));
        std::copy(first, last, begin(*this));
    }
    inline void assign(const internal::size_t length, const T &&val = T{}) {
        this->std::valarray<T>::resize(length, std::forward<const T>(val));
    }
    inline void resize(const internal::size_t length, const T &&val = T{}) {
        std::valarray<T> temp = *this;
        this->assign(length, std::forward<const T>(val));
        std::move(begin(temp),
                  std::min(std::end(temp), std::next(std::begin(temp), length)),
                  begin(*this));
    }
};
} // namespace lib
/* [valarray.hpp] */
namespace lib {
namespace internal {
namespace matrix_lib {
template <class T> struct interface : virtual grid_lib::interface<T> {
    virtual size_t rows() const = 0;
    virtual size_t cols() const = 0;
    virtual size_t square() const = 0;
};
} // namespace matrix_lib
template <class T, class base>
struct matrix_core : base, virtual matrix_lib::interface<T> {
    using base::base;
    static inline matrix_core Identity(const size_t n, const T &&val = {1}) {
        matrix_core res(n);
        REP(i, n) res(i, i) = val;
        return res;
    }
    inline size_t rows() const override { return this->height(); }
    inline size_t cols() const override { return this->width(); }
    inline size_t square() const override {
        return this->rows() == this->cols();
    }
    template <class U> inline matrix_core &operator+=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs;
        return *this;
    }
    template <class... U>
    inline matrix_core &operator+=(const matrix_core<U...> rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs(i, j);
        return *this;
    }
    template <class U> inline matrix_core operator+(const U rhs) const {
        return matrix_core(*this) += rhs;
    }
    template <class U> inline matrix_core &operator-=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs;
        return *this;
    }
    template <class... U>
    inline matrix_core &operator-=(const matrix_core<U...> rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs(i, j);
        return *this;
    }
    template <class U> inline matrix_core operator-(const U rhs) const {
        return matrix_core(*this) -= rhs;
    }
    template <class... U>
    inline matrix_core operator*(const matrix_core<U...> rhs) {
        dev_assert(this->cols() == rhs.rows());
        matrix_core res(this->rows(), rhs.cols());
        REP(i, this->rows()) REP(j, rhs.cols()) REP(k, this->cols()) {
            res(i, j) += (*this)(i, k) * rhs(k, j);
        }
        return res;
    }
    template <class U> inline matrix_core operator*(const U rhs) {
        matrix_core res(*this);
        REP(i, res.rows()) REP(j, res.cols()) res(i, j) *= rhs;
        return res;
    }
    template <class U> inline matrix_core &operator*=(const U rhs) {
        matrix_core res = *this * rhs;
        this->assign(res);
        return *this;
    }
    template <class U> inline matrix_core &operator/=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) /= rhs;
        return *this;
    }
    template <class U> inline matrix_core operator/(const U rhs) const {
        return matrix_core(*this) /= rhs;
    }
    template <class U> inline matrix_core &operator%=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) %= rhs;
        return *this;
    }
    template <class U> inline matrix_core operator%(const U rhs) const {
        return matrix_core(*this) %= rhs;
    }
    inline matrix_core pow(ll p) {
        dev_assert(this->square());
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
/* [matrix.hpp] */

signed main() {
    int n, m, k;
    std::cin >> n >> m >> k;
    lib::matrix<atcoder::modint998244353> a(n, m), b(m, k);
    input >> a >> b;
    print(a * b);
}
