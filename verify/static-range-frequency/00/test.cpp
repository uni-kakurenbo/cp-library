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
    inline void seekp(const typename destination::off_type off,
                      const std::ios_base::seekdir dir = std::ios_base::cur) {
        this->out->seekp(off, dir);
    };
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
/* #expanded [wavelet_matrix.hpp] */
#include <algorithm>
#include <array>
#include <atcoder/internal_bit>
#include <cstdint>
#include <iterator>
#include <limits>
#include <optional>
#include <utility>
#include <vector>

/* #expanded [dev_assert.hpp] */
#ifdef LOCAL_JUDGE
#include <cassert>
#define dev_assert(...) assert(__VA_ARGS__)
#else
#define dev_assert(...) ({ ; })
#endif
/* [dev_assert.hpp] */
/* #expanded [types.hpp] */
#include <cstdint>
namespace lib {
namespace internal {
using size_t = std::int32_t;
using int128_t = __int128_t;
using uint128_t = __uint128_t;
} // namespace internal
} // namespace lib
/* [types.hpp] */
/* #expanded [iterator.hpp] */
#include <iterator>

namespace lib {
namespace internal {
template <class T> struct iterator_interface {
    using iterator_category = std::output_iterator_tag;
    using difference_type = size_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    virtual T operator*() const = 0;
};
template <class T>
struct bidirectiona_iterator_interface : iterator_interface<T> {
    using iterator_category = std::bidirectional_iterator_tag;
    virtual bidirectiona_iterator_interface &operator++() = 0;
    virtual bidirectiona_iterator_interface &operator--() = 0;
};
template <class T>
struct random_access_iterator_base : bidirectiona_iterator_interface<T> {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type =
        typename bidirectiona_iterator_interface<T>::difference_type;

  public:
    virtual random_access_iterator_base &
    operator+=(const difference_type count) = 0;
    virtual random_access_iterator_base &
    operator-=(const difference_type count) = 0;
};
template <class T, class container>
struct container_iterator_interface : public random_access_iterator_base<T> {
    using difference_type =
        typename bidirectiona_iterator_interface<T>::difference_type;

  protected:
    const container *const _ref;
    difference_type _pos;
    container_iterator_interface(const container *const ref,
                                 const difference_type &pos)
        : _ref(ref), _pos(pos) {}

  public:
    inline const container *ref() const { return this->_ref; }
    inline difference_type pos() const { return this->_pos; }
    inline difference_type &pos() { return this->_pos; }
    inline container_iterator_interface &operator++() override {
        return ++this->pos(), *this;
    }
    inline container_iterator_interface &operator--() override {
        return --this->pos(), *this;
    }
    inline container_iterator_interface &
    operator+=(const difference_type count) override {
        return this->pos() += count, *this;
    }
    inline container_iterator_interface &
    operator-=(const difference_type count) override {
        return this->pos() -= count, *this;
    }
    inline difference_type
    operator-(const container_iterator_interface &other) const {
        return this->pos() - other.pos();
    }
    inline bool operator<(const container_iterator_interface &other) const {
        return *this - other < 0;
    }
    inline bool operator>(const container_iterator_interface &other) const {
        return *this - other > 0;
    }
    inline bool operator<=(const container_iterator_interface &other) const {
        return not(*this > other);
    }
    inline bool operator>=(const container_iterator_interface &other) const {
        return not(*this < other);
    }
    inline bool operator!=(const container_iterator_interface &other) const {
        return this->ref() != other.ref() or *this < other or *this > other;
    }
    inline bool operator==(const container_iterator_interface &other) const {
        return not(*this != other);
    }
};
} // namespace internal
} // namespace lib
/* [iterator.hpp] */
/* #expanded [range_reference.hpp] */
#include <iterator>
/* #expanded [constants.hpp] */
#include <cstdint>
namespace lib {
enum class comp : std::int8_t {
    equal_to,
    not_equal_to,
    equals = equal_to,
    eq = equal_to,
    under,
    over,
    under_or,
    over_or,
    less = under,
    more = over,
    less_than = under,
    more_than = over,
    not_less_than = over_or,
    not_more_than = under_or,
    leq = under_or,
    geq = over_or
};
enum class range : std::int8_t {
    right_open,
    left_open,
    open,
    closed,
};
} // namespace lib
/* [constants.hpp] */
namespace lib {
namespace internal {
template <class Super> struct range_reference {
    using size_type = typename Super::size_type;
    using iterator = typename Super::iterator;

  protected:
    const Super *const super;
    const size_type _begin, _end;
    range_reference(const Super *const super, const size_type begin,
                    const size_type end)
        : super(super), _begin(begin), _end(end) {}

  public:
    inline iterator begin() const {
        return std::next(super->begin(), this->_begin);
    }
    inline iterator end() const {
        return std::next(super->begin(), this->_end);
    }
    inline size_type size() const { return this->_end - this->_begin; }

  protected:
    inline range_reference sub_range(size_type l, size_type r) const {
        l = this->super->_positivize_index(l),
        r = this->super->_positivize_index(r);
        dev_assert(0 <= l and l <= r and r <= this->size());
        return range_reference(this->super, this->_begin + l, this->_begin + r);
    }

  public:
    template <lib::range rng = lib::range::right_open>
    inline range_reference range(const size_type l, const size_type r) const {
        if constexpr(rng == lib::range::right_open)
            return this->sub_range(l, r);
        if constexpr(rng == lib::range::left_open)
            return this->sub_range(l + 1, r + 1);
        if constexpr(rng == lib::range::open)
            return this->sub_range(l + 1, r);
        if constexpr(rng == lib::range::closed)
            return this->sub_range(l, r + 1);
    }
    inline range_reference range() const {
        return range_reference(this->_begin, this->_end);
    }
    inline range_reference operator()(const size_type l,
                                      const size_type r) const {
        return this->sub_range(l, r);
    }
    inline range_reference subseq(const size_type p, const size_type c) const {
        return this->sub_range(p, p + c);
    }
    inline range_reference subseq(const size_type p) const {
        return this->sub_range(p, this->size());
    }
};
} // namespace internal
} // namespace lib
/* [range_reference.hpp] */
/* #expanded [succinct_bit_vector.hpp] */

#include <cstdint>
#include <immintrin.h>
#include <vector>
namespace lib {
struct succinct_bit_vector {
    using size_type = internal::size_t;

  private:
    static constexpr std::uint32_t w = 64;
    std::vector<std::uint64_t> _block;
    std::vector<std::uint32_t> _count;
    size_type _n, _zeros;

  public:
    succinct_bit_vector() {}
    succinct_bit_vector(const size_type _n) { this->init(_n); }
    inline size_type size() const { return this->_n; }
    inline size_type zeros() const { return this->_zeros; }
    inline void set(size_type i) { this->_block[i / w] |= 1LL << (i % w); }
    inline bool get(size_type i) const {
        return std::uint32_t(this->_block[i / w] >> (i % w)) & 1U;
    }
    __attribute__((optimize("O3", "unroll-loops"))) void init(const int _n) {
        this->_n = this->_zeros = _n;
        this->_block.resize(_n / w + 1, 0);
        this->_count.resize(this->_block.size(), 0);
    }
    __attribute__((target("popcnt"))) void build() {
        for(auto i = 1UL; i < this->_block.size(); ++i)
            this->_count[i] =
                this->_count[i - 1] + _mm_popcnt_u64(this->_block[i - 1]);
        this->_zeros = this->rank0(this->_n);
    }
    __attribute__((target("bmi2,popcnt"))) inline size_type
    rank1(size_type i) const {
        return this->_count[i / w] +
               _mm_popcnt_u64(_bzhi_u64(this->_block[i / w], i % w));
    }
    inline size_type rank0(size_type i) const { return i - this->rank1(i); }
};
} // namespace lib
/* [succinct_bit_vector.hpp] */

/* #expanded [compression.hpp] */
#include <algorithm>
#include <iterator>
#include <map>
#include <vector>

namespace lib {
template <class T, class container = std::vector<internal::size_t>>
struct compression : container {
    using size_type = internal::size_t;

  protected:
    std::vector<T> values;

  public:
    explicit compression() {}
    template <class I> compression(const I first, const I last) {
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
    inline size_type rank(const T &val) const {
        return std::distance(
            this->values.begin(),
            std::lower_bound(this->values.begin(), this->values.end(), val));
    }
    inline T val() const { return this->values[val]; }
    inline T operator()(const internal::size_t val) const {
        return this->values[val];
    }
};
} // namespace lib
/* [compression.hpp] */
namespace lib {
namespace internal {
namespace wavelet_matrix_lib {
template <class T> struct base {
    using size_type = internal::size_t;

  private:
    using int64_t = std::int64_t;
    using uint64_t = std::uint64_t;
    size_type _n, _bits;
    std::vector<succinct_bit_vector> _index;
    std::vector<size_type> _first_ones;
    std::vector<std::vector<T>> _sum;
    T _max = 0;

  public:
    base() {}
    template <class I> base(const I first, const I last) {
        this->build(first, last);
    }
    inline size_type size() const { return this->_n; }
    inline size_type bits() const { return this->_bits; }
    template <class I>
    __attribute__((optimize("O3"))) void build(const I first, const I last) {
        this->_n = std::distance(first, last);
        this->_max = first == last ? 0 : *std::max_element(first, last) + 1;
        this->_bits = atcoder::internal::ceil_pow2(this->_max);
        this->_index.assign(this->_bits, this->_n);
        this->_first_ones.assign(this->_bits, this->_n);
        std::vector<T> depth(first, last), nxt(this->_n);
        this->_sum.assign(this->_bits + 1, std::vector<T>(this->_n + 1));
        {
            size_type i = 0;
            for(auto itr = first; itr != last; ++i, ++itr) {
                this->_sum[this->_bits][i + 1] =
                    this->_sum[this->_bits][i] + *itr;
            }
        }
        for(size_type h = this->_bits - 1; h >= 0; --h) {
            std::vector<size_type> vals;
            for(size_type i = 0; i < this->_n; ++i) {
                if((depth[i] >> h) & 1)
                    this->_index[h].set(i);
            }
            this->_index[h].build();
            this->_first_ones[h] = this->_index[h].zeros();
            std::array<typename std::vector<T>::iterator, 2> itrs{
                std::begin(nxt),
                std::next(std::begin(nxt), this->_index[h].zeros())};
            for(size_type i = 0; i < this->_n; ++i)
                *itrs[this->_index[h].get(i)]++ = depth[i];
            REP(i, nxt.size()) this->_sum[h][i + 1] = this->_sum[h][i] + nxt[i];
            std::swap(depth, nxt);
        }
    }

  protected:
    inline T get(size_type k) const {
        T res = 0;
        for(size_type h = this->_bits - 1; h >= 0; --h) {
            bool f = this->_index[h].get(k);
            res |= f ? T{1} << h : 0;
            k = f ? this->_index[h].rank1(k) + this->_index[h].zeros()
                  : this->_index[h].rank0(k);
        }
        return res;
    }
    inline T kth_smallest(size_type l, size_type r, size_type k) const {
        T res = 0;
        for(size_type h = this->_bits - 1; h >= 0; --h) {
            size_type l0 = this->_index[h].rank0(l),
                      r0 = this->_index[h].rank0(r);
            if(k < r0 - l0)
                l = l0, r = r0;
            else {
                k -= r0 - l0;
                res |= T{1} << h;
                l += this->_index[h].zeros() - l0;
                r += this->_index[h].zeros() - r0;
            }
        }
        return res;
    }
    inline T kth_largest(const size_type l, const size_type r,
                         const size_type k) const {
        return this->kth_smallest(l, r, r - l - k - 1);
    }
    inline std::pair<size_type, size_type>
    succ0(const size_type l, const size_type r, const size_type h) const {
        return std::make_pair(this->_index[h].rank0(l),
                              this->_index[h].rank0(r));
    }
    inline std::pair<size_type, size_type>
    succ1(const size_type l, const size_type r, const size_type h) const {
        size_type l0 = this->_index[h].rank0(l);
        size_type r0 = this->_index[h].rank0(r);
        size_type vals = this->_index[h].zeros();
        return std::make_pair(l + vals - l0, r + vals - r0);
    }
    T sum_in_range(const size_type l, const size_type r, const T &x, const T &y,
                   const T &cur, const size_type bit) const {
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
               this->sum_in_range(this->_first_ones[bit] + l1,
                                  this->_first_ones[bit] + r1, x, y, nxt,
                                  bit - 1);
    }
    inline T sum_in_range(const size_type l, const size_type r, const T &x,
                          const T &y) const {
        return this->sum_in_range(l, r, x, y, 0, this->_bits - 1);
    }
    inline T sum_under(const size_type l, const size_type r, const T &v) const {
        return this->sum_in_range(l, r, 0, v);
    }
    inline T sum_over(const size_type l, const size_type r, const T &v) const {
        return this->sum_in_range(l, r, v + 1, std::numeric_limits<T>::max());
    }
    inline T sum_under_or(const size_type l, const size_type r,
                          const T &v) const {
        return this->sum_in_range(l, r, 0, v + 1);
    }
    inline T sum_over_or(const size_type l, const size_type r,
                         const T &v) const {
        return this->sum_in_range(l, r, v, std::numeric_limits<T>::max());
    }
    inline size_type count_under(size_type l, size_type r, const T &y) const {
        if(y >= (T{1} << this->_bits))
            return r - l;
        size_type res = 0;
        for(size_type h = this->_bits - 1; h >= 0; --h) {
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
                                    const T &x, const T &y) const {
        return this->count_under(l, r, y) - this->count_under(l, r, x);
    }
    inline size_type count_under_or(size_type l, size_type r,
                                    const T &y) const {
        return this->count_under(l, r, y + 1);
    }
    inline size_type count_over(size_type l, size_type r, const T &x) const {
        return this->count_over_or(l, r, x + 1);
    }
    inline size_type count_over_or(size_type l, size_type r, const T &x) const {
        return r - l - this->count_under(l, r, x);
    }
    inline size_type count_equal_to(const size_type l, const size_type r,
                                    const T &v) const {
        return this->count_under(l, r, v + 1) - this->count_under(l, r, v);
    }
    inline std::optional<T> prev_value(const size_type l, const size_type r,
                                       const T &y) const {
        size_type cnt = this->count_under(l, r, y);
        return cnt == 0 ? std::optional<T>{}
                        : std::optional<T>(this->kth_smallest(l, r, cnt - 1));
    }
    inline std::optional<T> next_value(const size_type l, const size_type r,
                                       const T &x) const {
        size_type cnt = this->count_under(l, r, x);
        return cnt == r - l ? std::optional<T>{}
                            : std::optional<T>(this->kth_smallest(l, r, cnt));
    }
};
} // namespace wavelet_matrix_lib
} // namespace internal
template <class T> struct compressed_wavelet_matrix;
template <class T>
struct wavelet_matrix : internal::wavelet_matrix_lib::base<T> {
    using compressed = compressed_wavelet_matrix<T>;

  private:
    using base = typename internal::wavelet_matrix_lib::base<T>;

  public:
    using value_type = T;
    using size_type = typename base::size_type;

  protected:
    inline void
    _validate_index_in_right_open([[maybe_unused]] const size_type p) const {
        dev_assert(0 <= p and p < this->size());
    }
    inline void
    _validate_index_in_closed([[maybe_unused]] const size_type p) const {
        dev_assert(0 <= p and p <= this->size());
    }
    inline void
    _validate_rigth_open_interval([[maybe_unused]] const size_type l,
                                  [[maybe_unused]] const size_type r) const {
        dev_assert(0 <= l and l <= r and r <= this->size());
    }
    inline size_type _positivize_index(const size_type p) const {
        return p < 0 ? this->size() + p : p;
    }

  public:
    using base::base;
    inline T get(size_type k) const {
        k = this->_positivize_index(k);
        this->_validate_index_in_right_open(k);
        return this->base::get(k);
    }
    inline T operator[](const size_type k) const { return this->get(k); }
    struct iterator;
    struct range_reference;
    template <lib::range rng = lib::range::right_open>
    inline range_reference range(const size_type l, const size_type r) const {
        if constexpr(rng == lib::range::right_open)
            return range_reference(this, l, r);
        if constexpr(rng == lib::range::left_open)
            return range_reference(this, l + 1, r + 1);
        if constexpr(rng == lib::range::open)
            return range_reference(this, l + 1, r);
        if constexpr(rng == lib::range::closed)
            return range_reference(this, l, r + 1);
    }
    inline range_reference range() const {
        return range_reference(this, 0, this->size());
    }
    inline range_reference operator()(const size_type l,
                                      const size_type r) const {
        return range_reference(this, l, r);
    }
    inline range_reference subseq(const size_type p, const size_type c) const {
        return range_reference(this, p, p + c);
    }
    inline range_reference subseq(const size_type p) const {
        return range_reference(this, p, this->size());
    }
    struct range_reference : internal::range_reference<wavelet_matrix> {
        range_reference(const wavelet_matrix *const super, const size_type l,
                        const size_type r)
            : internal::range_reference<wavelet_matrix>(
                  super, super->_positivize_index(l),
                  super->_positivize_index(r)) {
            this->super->_validate_rigth_open_interval(this->_begin,
                                                       this->_end);
        }
        inline T get(const size_type k) const {
            k = this->super->_positivize_index(k);
            dev_assert(0 <= k and k < this->size());
            return this->super->get(this->_begin + k);
        }
        inline T operator[](const size_type k) const { return this->get(k); }
        inline T kth_smallest(const size_type k) const {
            dev_assert(0 <= k and k < this->size());
            return this->super->base::kth_smallest(this->_begin, this->_end, k);
        }
        inline T kth_largest(const size_type k) const {
            dev_assert(0 <= k and k < this->size());
            return this->super->base::kth_largest(this->_begin, this->_end, k);
        }
        inline T min() const { return this->kth_smallest(0); }
        inline T max() const { return this->kth_largest(0); }
        inline T median() const { return this->kth_smallest(this->size() / 2); }
        inline T sum_in_range(const T &x, const T &y) const {
            return this->super->base::sum_in_range(this->_begin, this->_end, x,
                                                   y);
        }
        inline T sum_under(const T &v) const {
            return this->super->base::sum_under(this->_begin, this->_end, v);
        }
        inline T sum_over(const T &v) const {
            return this->super->base::sum_over(this->_begin, this->_end, v);
        }
        inline T sum_under_or(const T &v) const {
            return this->super->base::sum_under_or(this->_begin, this->_end, v);
        }
        inline T sum_over_or(const T &v) const {
            return this->super->base::sum_over_or(this->_begin, this->_end, v);
        }
        inline T sum(const T &x, const T &y) const {
            return this->super->base::sum_in_range(this->_begin, this->_end, x,
                                                   y);
        }
        template <comp com> inline size_type sum(const T &v) const {
            if constexpr(com == comp::under)
                return this->sum_under(v);
            if constexpr(com == comp::over)
                return this->sum_over(v);
            if constexpr(com == comp::under_or)
                return this->sum_under_or(v);
            if constexpr(com == comp::over_or)
                return this->sum_over_or(v);
            assert(false);
        }
        inline size_type count_in_range(const T &x, const T &y) const {
            return this->super->base::count_in_range(this->_begin, this->_end,
                                                     x, y);
        }
        inline size_type count_under(const T &v) const {
            return this->super->base::count_under(this->_begin, this->_end, v);
        }
        inline size_type count_over(const T &v) const {
            return this->super->base::count_over(this->_begin, this->_end, v);
        }
        inline size_type count_under_or(const T &v) const {
            return this->super->base::count_under_or(this->_begin, this->_end,
                                                     v);
        }
        inline size_type count_over_or(const T &v) const {
            return this->super->base::count_over_or(this->_begin, this->_end,
                                                    v);
        }
        template <comp com = comp::equal_to>
        inline size_type count(const T &v) const {
            if constexpr(com == comp::eq)
                return this->super->count_equal_to(this->_begin, this->_end, v);
            if constexpr(com == comp::under)
                return this->count_under(v);
            if constexpr(com == comp::over)
                return this->count_over(v);
            if constexpr(com == comp::under_or)
                return this->count_under_or(v);
            if constexpr(com == comp::over_or)
                return this->count_over_or(v);
            assert(false);
        }
        inline std::optional<T> prev_value(const T &v) const {
            return this->super->base::prev_value(this->_begin, this->_end, v);
        }
        inline std::optional<T> next_value(const T &v) const {
            return this->super->base::next_value(this->_begin, this->_end, v);
        }
    };
    inline T kth_smallest(const size_type k) const {
        return this->range().kth_smallest(k);
    }
    inline T kth_largest(const size_type k) const {
        return this->range().kth_largest(k);
    }
    inline T min() const { return this->range().kth_smallest(0); }
    inline T max() const { return this->range().kth_largest(0); }
    inline T median() const { return this->range().median(); }
    inline T sum_in_range(const T &x, const T &y) const {
        return this->range().sum_in_range(x, y);
    }
    inline T sum_under(const T &v) const { return this->range().sum_under(v); }
    inline T sum_over(const T &v) const { return this->range().sum_over(v); }
    inline T sum_under_or(const T &v) const {
        return this->range().sum_under_or(v);
    }
    inline T sum_over_or(const T &v) const {
        return this->range().sum_over_or(v);
    }
    inline T sum(const T &x, const T &y) const {
        return this->range().sum_in_range(x, y);
    }
    template <comp com> inline size_type sum(const T &v) const {
        if constexpr(com == comp::under)
            return this->range().sum_under(v);
        if constexpr(com == comp::over)
            return this->range().sum_over(v);
        if constexpr(com == comp::under_or)
            return this->range().sum_under_or(v);
        if constexpr(com == comp::over_or)
            return this->range().sum_over_or(v);
        assert(false);
    }
    inline size_type count_in_range(const T &x, const T &y) const {
        return this->range().count_in_range(x, y);
    }
    inline size_type count_under(const T &v) const {
        return this->range().count_under(v);
    }
    inline size_type count_over(const T &v) const {
        return this->range().count_over(v);
    }
    inline size_type count_under_or(const T &v) const {
        return this->range().count_under_or(v);
    }
    inline size_type count_over_or(const T &v) const {
        return this->range().count_over_or(v);
    }
    template <comp com = comp::equal_to>
    inline size_type count(const T &v) const {
        if constexpr(com == comp::eq)
            return this->range().count_equal_to(v);
        if constexpr(com == comp::under)
            return this->range().count_under(v);
        if constexpr(com == comp::over)
            return this->range().count_over(v);
        if constexpr(com == comp::under_or)
            return this->range().count_under_or(v);
        if constexpr(com == comp::over_or)
            return this->range().count_over_or(v);
        assert(false);
    }
    inline std::optional<T> prev_value(const T &v) const {
        return this->range().prev_value(v);
    }
    inline std::optional<T> next_value(const T &v) const {
        return this->range().next_value(v);
    }
    struct iterator
        : virtual internal::container_iterator_interface<T, wavelet_matrix> {
        iterator(const wavelet_matrix *const ref, const size_type pos)
            : internal::container_iterator_interface<T, wavelet_matrix>(ref,
                                                                        pos) {}
        inline T operator*() const override {
            return this->ref()->get(this->pos());
        }
    };
    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};
template <class T>
struct compressed_wavelet_matrix
    : protected wavelet_matrix<typename compression<T>::size_type> {
  protected:
    using core = wavelet_matrix<typename compression<T>::size_type>;
    compression<T> compressed;

  public:
    using size_type = typename core::size_type;
    template <class I> compressed_wavelet_matrix(const I first, const I last) {
        this->build(first, last);
    }
    template <class I> void build(const I first, const I last) {
        this->compressed = compression<T>(first, last);
        this->core::build(ALL(this->compressed));
    }
    inline T get(const size_type k) const {
        return this->compressed(this->core::get(k));
    }
    inline size_type operator[](const size_type k) const {
        return this->core::get(k);
    }
    struct iterator;
    struct range_reference;
    template <lib::range rng = lib::range::right_open>
    inline range_reference range(const size_type l, const size_type r) const {
        if constexpr(rng == lib::range::right_open)
            return range_reference(this, l, r);
        if constexpr(rng == lib::range::left_open)
            return range_reference(this, l + 1, r + 1);
        if constexpr(rng == lib::range::open)
            return range_reference(this, l + 1, r);
        if constexpr(rng == lib::range::closed)
            return range_reference(this, l, r + 1);
    }
    inline range_reference range() const {
        return range_reference(this, 0, this->size());
    }
    inline range_reference operator()(const size_type l,
                                      const size_type r) const {
        return range_reference(this, l, r);
    }
    inline range_reference subseq(const size_type p, const size_type c) const {
        return range_reference(this, p, p + c);
    }
    inline range_reference subseq(const size_type p) const {
        return range_reference(this, p, this->size());
    }
    struct range_reference
        : internal::range_reference<compressed_wavelet_matrix> {
        range_reference(const compressed_wavelet_matrix *const super,
                        const size_type l, const size_type r)
            : internal::range_reference<compressed_wavelet_matrix>(
                  super, super->_positivize_index(l),
                  super->_positivize_index(r)) {
            this->super->_validate_rigth_open_interval(this->_begin,
                                                       this->_end);
        }

      private:
        inline auto _range() const {
            return this->super->core::range(this->_begin, this->_end);
        }

      public:
        inline T get(const size_type k) const {
            return this->super->compressed(this->_range().get(k));
        }
        inline T operator[](const size_type k) const { return this->get(k); }
        inline T kth_smallest(const size_type k) const {
            return this->super->compressed(this->_range().kth_smallest(k));
        }
        inline T kth_largest(const size_type k) const {
            return this->super->compressed(this->_range().kth_largest(k));
        }
        inline T min() const { return this->kth_smallest(0); }
        inline T max() const { return this->kth_largest(0); }
        inline T median() const { return this->kth_smallest(this->size() / 2); }
        inline size_type count_in_range(const T &x, const T &y) const {
            return this->_range().count_in_range(
                this->super->compressed.rank(x),
                this->super->compressed.rank(y));
        }
        inline size_type count_under(const T &v) const {
            return this->_range().count_under(this->super->compressed.rank(v));
        }
        inline size_type count_over(const T &v) const {
            return this->_range().count_over(this->super->compressed.rank(v));
        }
        inline size_type count_under_or(const T &v) const {
            return this->_range().count_under_or(
                this->super->compressed.rank(v));
        }
        inline size_type count_over_or(const T &v) const {
            return this->_range().count_over_or(
                this->super->compressed.rank(v));
        }
        template <comp com = comp::equal_to>
        inline size_type count(const T &v) const {
            return this->_range().template count<com>(
                this->super->compressed.rank(v));
        }
        inline std::optional<T> prev_value(const T &v) const {
            std::optional<size_type> res =
                this->_range().prev_value(this->super->compressed.rank(v));
            if(res.has_value())
                return this->super->compressed(res.value());
            return {};
        }
        inline std::optional<T> next_value(const T &v) const {
            std::optional<size_type> res =
                this->_range().next_value(this->super->compressed.rank(v));
            if(res.has_value())
                return this->super->compressed(res.value());
            return {};
        }
    };
    inline T kth_smallest(const size_type k) const {
        return this->range().kth_smallest(k);
    }
    inline T kth_largest(const size_type k) const {
        return this->range().kth_largest(k);
    }
    inline T min() const { return this->range().kth_smallest(0); }
    inline T max() const { return this->range().kth_largest(0); }
    inline T median() const { return this->range().median(); }
    inline size_type count_in_range(const T &x, const T &y) const {
        return this->range().count_in_range(x, y);
    }
    inline size_type count_under(const T &v) const {
        return this->range().count_under(v);
    }
    inline size_type count_over(const T &v) const {
        return this->range().count_over(v);
    }
    inline size_type count_under_or(const T &v) const {
        return this->range().count_under_or(v);
    }
    inline size_type count_over_or(const T &v) const {
        return this->range().count_over_or(v);
    }
    template <comp com = comp::equal_to>
    inline size_type count(const T &v) const {
        return this->range().template count<com>(v);
    }
    inline std::optional<T> prev_value(const T &v) const {
        return this->range().prev_value(v);
    }
    inline std::optional<T> next_value(const T &v) const {
        return this->range().next_value(v);
    }
    struct iterator : virtual internal::container_iterator_interface<
                          T, compressed_wavelet_matrix> {
        iterator(const compressed_wavelet_matrix *const ref,
                 const size_type pos)
            : internal::container_iterator_interface<T,
                                                     compressed_wavelet_matrix>(
                  ref, pos) {}
        inline T operator*() const override {
            return this->ref()->get(this->pos());
        }
    };
    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};
} // namespace lib
/* [wavelet_matrix.hpp] */

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
