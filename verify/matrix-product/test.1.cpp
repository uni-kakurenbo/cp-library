#include <atcoder/modint>
/* #expanded [snippet/fast_io.hpp] */
#include <iostream>
/* #expanded [internal/dev_env.hpp] */
#ifdef LOCAL_JUDGE
constexpr bool DEV_ENV = true;
constexpr bool NO_EXCEPT = false;
#else
constexpr bool DEV_ENV = false;
constexpr bool NO_EXCEPT = true;
#endif
/* [internal/dev_env.hpp] */
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
        : out(out), endline(endline), separator(separator) {}
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
/* #expanded [numeric/matrix.hpp] */
#include <cassert>
#include <vector>
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
/* #expanded [grid.hpp] */
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
/* #expanded [snippet/aliases.hpp] */
#include <cstdint>
#include <utility>
#include <vector>
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
namespace lib {
namespace internal {
namespace grid_impl {
template <class T> struct interface {
    virtual void assign(const size_t, const size_t,
                        const T &) noexcept(NO_EXCEPT) = 0;
    virtual void resize(const size_t, const size_t) noexcept(NO_EXCEPT) = 0;
    virtual size_t height() const noexcept(NO_EXCEPT) = 0;
    virtual size_t width() const noexcept(NO_EXCEPT) = 0;
    virtual size_t id(const size_t, const size_t) const noexcept(NO_EXCEPT) = 0;
    virtual T &operator()(const size_t, const size_t) noexcept(NO_EXCEPT) = 0;
    virtual const T &operator()(const size_t, const size_t) const
        noexcept(NO_EXCEPT) = 0;
};
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
                        const size_t w) noexcept(NO_EXCEPT) override {
        this->_h = h, this->_w = w;
    }
    inline size_t height() const noexcept(NO_EXCEPT) override { return this->_h; }
    inline size_t width() const noexcept(NO_EXCEPT) override { return this->_w; }
    inline size_t id(const size_t i, const size_t j) const
        noexcept(NO_EXCEPT) override {
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
                       const T &val = T{}) noexcept(NO_EXCEPT) override {
        this->container_base<T>::resize(h, w);
        this->base::resize(h);
        ITRR(row, *this) row.assign(w, val);
    }
    inline void resize(const size_t h,
                       const size_t w) noexcept(NO_EXCEPT) override {
        this->container_base<T>::resize(h, w);
        this->base::resize(h);
        ITRR(row, *this) row.resize(w);
    }
    inline T &operator()(const size_t i,
                         const size_t j) noexcept(NO_EXCEPT) override {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        this->_validate_index(_i, _j);
        return (*this)[_i][_j];
    }
    inline const T &operator()(const size_t i, const size_t j) const
        noexcept(NO_EXCEPT) override {
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
                       const T &val = T{}) noexcept(NO_EXCEPT) override {
        this->container_base<T>::resize(h, w);
        this->base::assign(h * w, val);
    }
    inline void resize(const size_t h,
                       const size_t w) noexcept(NO_EXCEPT) override {
        this->container_base<T>::resize(h, w);
        this->base::resize(h * w);
    }
    inline T &operator()(const size_t i,
                         const size_t j) noexcept(NO_EXCEPT) override {
        const size_t _i = this->_positivize_row_index(i);
        const size_t _j = this->_positivize_col_index(j);
        return (*this)[this->id(_i, _j)];
    }
    inline const T &operator()(const size_t i, const size_t j) const
        noexcept(NO_EXCEPT) override {
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
/* #expanded [valarray.hpp] */
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
    inline void reserve(const size_type) noexcept(NO_EXCEPT) {}
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
/* [valarray.hpp] */
namespace lib {
namespace internal {
namespace matrix_impl {
template <class T> struct interface : virtual grid_impl::interface<T> {
    virtual size_t rows() const noexcept(NO_EXCEPT) = 0;
    virtual size_t cols() const noexcept(NO_EXCEPT) = 0;
    virtual size_t square() const noexcept(NO_EXCEPT) = 0;
};
} // namespace matrix_impl
template <class T, class base>
struct matrix_core : base, virtual matrix_impl::interface<T> {
    using base::base;
    static inline matrix_core identity(const size_t n,
                                       const T &&val = {1}) noexcept(NO_EXCEPT) {
        matrix_core res(n);
        REP(i, n) res(i, i) = val;
        return res;
    }
    inline size_t rows() const noexcept(NO_EXCEPT) override {
        return this->height();
    }
    inline size_t cols() const noexcept(NO_EXCEPT) override {
        return this->width();
    }
    inline size_t square() const noexcept(NO_EXCEPT) override {
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

input_adapter input;
output_adapter print;

signed main() {
    int n, m, k;
    std::cin >> n >> m >> k;
    lib::matrix<atcoder::modint998244353> a(n, m), b(m, k);
    input >> a >> b;
    print(a * b);
}
