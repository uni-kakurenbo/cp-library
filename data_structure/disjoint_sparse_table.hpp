#pragma once

#include <cassert>
#include <vector>
#include <iterator>
#include <utility>

#include "internal/types.hpp"
#include "internal/range_reference.hpp"

#include "numeric/bit.hpp"

#include "algebraic/internal/is_algebraic.hpp"
#include "data_structure/internal/is_action.hpp"
#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace internal {

namespace disjoint_sparse_table_impl {


template<class S>
struct base {
    using size_type = internal::size_t;
    using iterator = std::vector<std::vector<S>>;

  private:
    size_type _n = 0;

  protected:
    std::vector<std::vector<S>> _table = {};

  public:
    base() {}

    template<class I>
    explicit base(const I l, const I r) : _n(std::distance(l, r)) {
        this->_table.emplace_back(l, r);

        for(size_type i=2; i < this->_n; i <<= 1) {
            std::vector<S> vec; vec.reserve(this->_n);

            for(size_type j=i; j < this->_n; j += i << 1) {
                vec.emplace_back(this->_table.front()[j - 1]);
                for(size_type k=2; k<=i; ++k) {
                    vec.emplace_back(this->_table.front()[j - k] + vec.back());
                }
                vec.emplace_back(this->_table.front()[j]);
                for(size_type k=1; k<i and j+k < this->_n; ++k) {
                    vec.emplace_back(vec.back() + this->_table.front()[j + k]);
                }
            }

            this->_table.emplace_back(std::move(vec));
        }
    }

    size_type size() const { return this->_n; }

    S fold(const size_type l, size_type r) const {
        if(l == r) return S{};
        if(l == --r) return this->_table.front()[l];

        const size_type p = highest_bit_pos<unsigned>(l ^ r);
        return this->_table[p][l ^ ((size_type{1} << p) - 1)] + this->_table[p][r];
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
    inline size_type _positivize_index(const size_type p) const {
        return p < 0 ? this->size() + p : p;
    }

  public:
    template<class I>
    explicit core(const I first, const I last) : base(first, last) {}

    struct range_reference : internal::range_reference<const core> {
        range_reference(const core *const super, const size_type l, const size_type r)
          : internal::range_reference<const core>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

        inline value_type fold() {
            return this->_super->fold(this->_begin, this->_end);
        }
        inline value_type operator*() {
            return this->_super->fold(this->_begin, this->_end);
        }
    };

    inline value_type fold(size_type l, size_type r) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
            assert(0 <= l && l <= r && r <= this->_super->size());
        return this->base::fold(l, r);
    }
    inline value_type fold() const { return this->fold(0, this->size()); }

    inline auto operator[](const size_type index) const { return this->_table.front()[index]; }
    inline range_reference operator()(const size_type l, const size_type r) { return range_reference(this, l, r); }

    inline auto begin() const { return this->_table.begin()->begin(); }
    inline auto end() const { return this->_table.begin()->end(); }
};


template<class Action>
struct core<Action, std::void_t<typename internal::is_action_t<Action>>> : core<typename Action::operand> {
    using action = Action;
    using core<typename action::operand>::core;

    static_assert(action::tags.none() or action::tags.has(actions::flags::disjoint_sparse_table));
};


} // namespace disjoint_sparse_table_impl

} // namespace internal


template<class semigroup> struct disjoint_sparse_table : internal::disjoint_sparse_table_impl::core<semigroup> {
    using internal::disjoint_sparse_table_impl::core<semigroup>::core;
};


} // namespace lib
