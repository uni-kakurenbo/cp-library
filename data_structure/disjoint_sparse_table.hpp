#pragma once

#include <cassert>
#include <vector>
#include <iterator>
#include <utility>
#include <type_traits>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/range_reference.hpp"

#include "numeric/bit.hpp"

#include "algebraic/internal/traits.hpp"
#include "data_structure/internal/is_action.hpp"
#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace internal {

namespace disjoint_sparse_table_impl {

// Thanks to: https://noshi91.hatenablog.com/entry/2018/05/08/183946
template<class S>
struct base {
    using size_type = internal::size_t;
    using iterator = std::vector<std::vector<S>>;

  private:
    size_type _n = 0, _depth = 0;
    bool _built = false;

  protected:
    std::vector<std::vector<S>> _table = {};

  public:
    explicit base(const size_type n = 0) noexcept(NO_EXCEPT) : _n(n) {
        this->_depth = bit_width<std::make_unsigned_t<size_type>>(n);
        this->_table.resize(this->_depth+1, std::vector<S>(n));
    }

    template<bool FORCE = false>
    inline auto& build() noexcept(NO_EXCEPT) {
        if(!FORCE and this->_built) return *this;
        for(size_type i=2; i<=this->_depth; ++i) {
            const size_type len = 1 << i;
            for(size_type l = 0, m = len/2; m < this->_n; l += len, m = l + len/2) {
                this->_table[i-1][m - 1] = this->_table.front()[m - 1];
                for(size_type j = m-2; j>=l; --j) {
                    this->_table[i-1][j] = this->_table[i-1][j + 1] + this->_table.front()[j];
                }
                this->_table[i-1][m] = this->_table.front()[m];
                for(size_type j = m+1; j<std::min(l + len, this->_n); ++j) {
                    this->_table[i-1][j] = this->_table[i-1][j - 1] +  this->_table.front()[j];
                }
            }
        }
        this->_built = true;
        return *this;
    }

    inline auto& raw() noexcept(NO_EXCEPT) {
        return this->_table.front();
        this->_built = false;
    }
    inline const auto& raw() const noexcept(NO_EXCEPT) { return this->_table.front(); }
    inline const auto& data() const noexcept(NO_EXCEPT) { return this->_table.front(); }

    size_type size() const noexcept(NO_EXCEPT) { return this->_n; }

    S fold(const size_type l, size_type r) {
        if(l == r) return S{};
        if(l == --r) return this->_table.front()[l];

        this->build();

        const size_type p = highest_bit_pos<std::make_unsigned_t<size_type>>(l ^ r);
        return this->_table[p][l] + this->_table[p][r];
    }
};


template<class, class = std::void_t<>> struct core {};

template<class semigroup>
struct core<semigroup, std::void_t<typename algebraic::internal::is_semigroup_t<semigroup>>> : base<semigroup> {
    using value_type = semigroup;
    using size_type = internal::size_t;

  private:
    using base = internal::disjoint_sparse_table_impl::base<semigroup>;

  protected:
    inline size_type _positivize_index(const size_type p) const noexcept(NO_EXCEPT) {
        return p < 0 ? this->size() + p : p;
    }

  public:
    using base::base;

    explicit core(const size_type n, const semigroup& val) noexcept(NO_EXCEPT) : core(n) {
        this->_table.begin()->assign(n, val);
    }

    template<class I>
    explicit core(const I first, const I last) noexcept(NO_EXCEPT) : core(std::distance(first, last)) {
        std::copy(first, last, this->_table.begin()->begin());
    }

    struct range_reference : internal::range_reference<core> {
        range_reference(core *const super, const size_type l, const size_type r) noexcept(NO_EXCEPT)
          : internal::range_reference<core>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

        inline value_type fold() noexcept(NO_EXCEPT) {
            return this->_super->fold(this->_begin, this->_end);
        }
        inline value_type operator*() noexcept(NO_EXCEPT) {
            return this->_super->fold(this->_begin, this->_end);
        }
    };

    inline value_type fold(size_type l, size_type r) noexcept(NO_EXCEPT) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->size());
        return this->base::fold(l, r);
    }
    inline value_type fold() noexcept(NO_EXCEPT) { return this->fold(0, this->size()); }

    inline auto operator[](const size_type index) const noexcept(NO_EXCEPT) { return this->_table.front()[index]; }
    inline range_reference operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) { return range_reference(this, l, r); }

    inline auto begin() const noexcept(NO_EXCEPT) { return this->_table.begin()->begin(); }
    inline auto end() const noexcept(NO_EXCEPT) { return this->_table.begin()->end(); }
};


template<class Action>
struct core<Action, std::void_t<typename internal::is_action_t<Action>>> : core<typename Action::operand> {
    using action = Action;
    using core<typename action::operand>::core;

    static_assert(action::tags.none() or action::tags.has(actions::flags::range_folding));
};


} // namespace disjoint_sparse_table_impl

} // namespace internal


template<class semigroup> struct disjoint_sparse_table : internal::disjoint_sparse_table_impl::core<semigroup> {
    using internal::disjoint_sparse_table_impl::core<semigroup>::core;
};


} // namespace lib
