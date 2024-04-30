#pragma once


#include <cassert>
#include <vector>
#include <iterator>
#include <utility>
#include <type_traits>
#include <ranges>
#include <concepts>
#include <bit>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/concepts.hpp"
#include "internal/range_reference.hpp"
#include "internal/unconstructible.hpp"

#include "numeric/bit.hpp"

#include "algebraic/internal/concepts.hpp"
#include "action/base.hpp"


namespace uni {

namespace internal {

namespace disjoint_sparse_table_impl {


// Thanks to: https://noshi91.hatenablog.com/entry/2018/05/08/183946
template<algebraic::internal::semigroup Operand>
struct core {
    using size_type = internal::size_t;
    using operand = Operand;

    using iterator = std::vector<std::vector<Operand>>;

    size_type _n = 0, _depth = 0;
    bool _built = false;

  protected:
    std::vector<std::vector<operand>> _table = {};

  public:
    explicit core(const size_type n = 0) noexcept(NO_EXCEPT) : _n(n) {
        this->_depth = std::bit_width<std::make_unsigned_t<size_type>>(n);
        this->_table.resize(this->_depth+1, std::vector<operand>(n));
    }


    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    core(I first, S last) noexcept(NO_EXCEPT)
      : core(static_cast<size_type>(std::ranges::distance(first, last)))
    {
        std::ranges::copy(first, last, this->_table.begin()->begin());
    }


    template<bool FORCE = false>
    inline auto& build() noexcept(NO_EXCEPT) {
        if(!FORCE and this->_built) return *this;

        FOR(i, 2, this->_depth) {
            const size_type len = 1 << i;

            for(size_type l = 0, m = (len >> 1); m < this->_n; l += len, m = l + (len >> 1)) {
                this->_table[i - 1][m - 1] = this->_table.front()[m - 1];
                REPD(j, l, m-1) {
                    this->_table[i - 1][j] = this->_table.front()[j] + this->_table[i - 1][j + 1];
                }

                this->_table[i - 1][m] = this->_table.front()[m];
                REP(j, m + 1, std::min(l + len, this->_n)) {
                    this->_table[i - 1][j] = this->_table[i - 1][j - 1] + this->_table.front()[j];
                }
            }
        }

        this->_built = true;

        return *this;
    }

    inline auto& raw() noexcept(NO_EXCEPT) {
        this->_built = false;
        return this->_table.front();
    }

    inline const auto& raw() const noexcept(NO_EXCEPT) { return this->_table.front(); }

    inline auto& data() noexcept(NO_EXCEPT) { return this->_table; }
    inline const auto& data() const noexcept(NO_EXCEPT) { return this->_table; }

    size_type size() const noexcept(NO_EXCEPT) { return this->_n; }

    operand fold(const size_type l, size_type r) {
        if(l == r) return operand{};
        if(l == --r) return this->_table.front()[l];

        this->build();

        const size_type p = highest_bit_pos<std::make_unsigned_t<size_type>>(l ^ r);
        return this->_table[p][l] + this->_table[p][r];
    }
};


} // namespace disjoint_sparse_table_impl

} // namespace internal




template<class> struct disjoint_sparse_table : internal::unconstructible {};


template<algebraic::internal::semigroup Semigroup>
struct disjoint_sparse_table<Semigroup> {
  private:
    using core = internal::disjoint_sparse_table_impl::core<Semigroup>;
    using iterator = core::iterator;

    core _impl;

  public:
    using value_type = Semigroup;
    using size_type = core::size_type;

  protected:
    inline auto _positivize_index(const size_type p) const noexcept(NO_EXCEPT) {
        return p < 0 ? this->_impl.size() + p : p;
    }

  public:
    explicit disjoint_sparse_table(const size_type n, const value_type& val = value_type()) noexcept(NO_EXCEPT) : _impl(n) {
        this->_impl.data().begin()->assign(n, val);
    }

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    disjoint_sparse_table(I first, S last) noexcept(NO_EXCEPT) : _impl(first, last) {}

    template<std::ranges::input_range R>
    explicit disjoint_sparse_table(R&& range) noexcept(NO_EXCEPT)
      : _impl(std::ranges::begin(range), std::ranges::end(range))
    {}


    inline auto& raw() noexcept(NO_EXCEPT) { return this->_impl.raw(); }
    inline const auto& raw() const noexcept(NO_EXCEPT) { return this->_impl.raw(); }
    inline const auto& data() const noexcept(NO_EXCEPT) { return this->impl.data(); }

    inline auto size() const noexcept(NO_EXCEPT) { return this->_impl.size(); }


    friend internal::range_reference<disjoint_sparse_table>;

    struct range_reference : internal::range_reference<disjoint_sparse_table> {
        range_reference(disjoint_sparse_table *const super, const size_type l, const size_type r) noexcept(NO_EXCEPT)
          : internal::range_reference<disjoint_sparse_table>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

        inline auto fold() noexcept(NO_EXCEPT) {
            return this->_super->fold(this->_begin, this->_end);
        }
    };


    inline auto fold(size_type l, size_type r) noexcept(NO_EXCEPT) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->size());
        return this->_impl.fold(l, r);
    }
    inline auto fold() noexcept(NO_EXCEPT) { return this->fold(0, this->size()); }

    inline auto operator[](const size_type index) const noexcept(NO_EXCEPT) { return this->_impl.data().front()[index]; }
    inline auto operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) { return range_reference(this, l, r); }

    inline auto begin() const noexcept(NO_EXCEPT) { return this->_impl.data().begin()->begin(); }
    inline auto end() const noexcept(NO_EXCEPT) { return this->_impl.data().begin()->end(); }
};


template<actions::internal::operatable_action Action>
struct disjoint_sparse_table<Action> : disjoint_sparse_table<typename Action::operand> {
    using disjoint_sparse_table<typename Action::operand>::disjoint_sparse_table;
};


} // namespace uni
