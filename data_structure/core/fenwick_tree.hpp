#include <atcoder/internal_type_traits>

#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "internal/dev_assert.hpp"
#include "internal/dev_env.hpp"

#include "utility/functional.hpp"

#ifdef LOCAL_JUDGE

#include "random/xorshift.hpp"

#endif

namespace Lib {

namespace FenwickTreeLib {


// Thanks to: atcoder::fenwick_tree
template<class T,T (*op)(T,T),T (*r_op)(T,T)>
struct Base {
   using Size = Internal::Size;

  private:
    Size _n;
    std::vector<T> data;

  protected:
    T prod(Size r) const {
        T s = 0;
        while (r > 0) {
            s = op(s, data[r - 1]);
            r -= r & -r;
        }
        return s;
    }

  public:
    explicit Base(const Size n = 0) : _n(n), data(n) {}

    void add(Size p, const T& x) {
        dev_assert(0 <= p && p < _n);
        p++;
        while (p <= _n) {
            data[p-1] = op(data[p-1], T(x));
            p += p & -p;
        }
    }

    T prod(const Size l, const Size r) const {
        dev_assert(0 <= l && l <= r && r <= _n);
        return r_op(this->prod(r), this->prod(l));
    }
};


} // namespace FenwickTreeLib


template<class T,T (*op)(T,T) = Internal::plus<T>,T (*r_op)(T,T) = Internal::minus<T>>
struct FenwickTree : FenwickTreeLib::Base<T,op,r_op> {
   using Size = typename FenwickTreeLib::Base<T,op,r_op>::Size;

  protected:
    Size _n;

  public:
    FenwickTree(const Size n = 0) : FenwickTreeLib::Base<T,op,r_op>(n+1) { this->_n = n; }
    FenwickTree(std::initializer_list<T> init_list) : FenwickTree(ALL(init_list)) {}

    template<class I>
    FenwickTree(const I first, const I last) : FenwickTree(std::distance(first, last)) {
        for(auto itr=first; itr!=last; ++itr) this->set(itr-first, *itr);
    }

    inline Size size() const { return this->_n; }

    inline T get(const Size p) const {
        dev_assert(0 <= p and p < this->size());
        return this->prod(p, p+1);
    }
    inline T operator[](Size pos) const { return this->get(pos); }

    inline void set(const Size p, const T& x) {
        dev_assert(0 <= p and p < this->size());
        this->add(p, r_op(x, this->get(p)));
    }

    struct Iterator : virtual Internal::IContainerIterator<T,FenwickTree> {
        Iterator(FenwickTree *const ref, const Size pos) : Internal::IContainerIterator<T,FenwickTree>(ref, pos) {}

        inline T operator*() const override { return this->ref()->get(this->pos()); }
    };
    using iterator = Iterator;

    inline Iterator begin() { return Iterator(this, 0); }
    inline Iterator end() { return Iterator(this, this->size()); }
};


} // namespace Lib
