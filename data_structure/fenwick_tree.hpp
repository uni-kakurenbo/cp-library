#include <atcoder/internal_type_traits>

#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "internal/dev_assert.hpp"
#include "internal/dev_env.hpp"

#include "utility/functional.hpp"

#ifdef LOCAL_JUDGE

#include "random/xorshift.hpp"

#endif

namespace lib {

namespace fenwick_tree_lib {


// Thanks to: atcoder::fenwick_tree
template<class Action>
struct base {
    using size_type = internal::size_t;
  private:

    size_t _n;
    std::vector<T> data;

  protected:
    T prod(size_type r) const {
        T s = 0;
        while (r > 0) {
            s = op(s, data[r - 1]);
            r -= r & -r;
        }
        return s;
    }

  public:
    explicit base(const size_type n = 0) : _n(n), data(n) {}

    void add(size_type p, const T& x) {
        dev_assert(0 <= p && p < _n);
        p++;
        while (p <= _n) {
            data[p-1] = op(data[p-1], T(x));
            p += p & -p;
        }
    }

    T prod(const size_type l, const size_type r) const {
        dev_assert(0 <= l && l <= r && r <= _n);
        return r_op(this->prod(r), this->prod(l));
    }
};


template<class T,T (*op)(T,T) = internal::plus<T>,T (*r_op)(T,T) = internal::minus<T>>
struct core : fenwick_tree_lib::base<T,op,r_op> {
   using size_type = typename fenwick_tree_lib::base<T,op,r_op>::size_type;

  protected:
    size_type _n;

  public:
    core(const size_type n = 0) : fenwick_tree_lib::base<T,op,r_op>(n+1) { this->_n = n; }
    core(std::initializer_list<T> init_list) : core(ALL(init_list)) {}

    template<class I>
    core(const I first, const I last) : core(std::distance(first, last)) {
        for(auto itr=first; itr!=last; ++itr) this->set(itr-first, *itr);
    }

    inline size_type size() const { return this->_n; }

    inline T get(const size_type p) const {
        dev_assert(0 <= p and p < this->size());
        return this->prod(p, p+1);
    }
    inline T operator[](size_type pos) const { return this->get(pos); }

    inline void set(const size_type p, const T& x) {
        dev_assert(0 <= p and p < this->size());
        this->add(p, r_op(x, this->get(p)));
    }

    struct iterator : virtual internal::container_iterator_interface<T,core> {
        iterator(const core *const ref, const size_type pos) : internal::container_iterator_interface<T,fenwick_tree>(ref, pos) {}

        inline T operator*() const override { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


} // namespace fenwick_tree_lib


template<class action> struct fenwick_tree : fenwick_tree_lib::core<action> {};


} // namespace lib
