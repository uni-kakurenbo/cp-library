#pragma once


#include <cassert>
#include <iterator>
#include <vector>
#include <functional>
#include <numeric>
#include <ranges>
#include <concepts>


#include "snippet/iterations.hpp"

#include "internal/types.hpp"
#include "internal/type_traits.hpp"

#include "adaptor/vector.hpp"


namespace uni {


template<class T, class Container = vector<T>>
struct accumulation : Container {
    using size_type = internal::size_t;

  protected:
    inline size_type _positivize_index(const size_type x) const noexcept(NO_EXCEPT) {
        return x < 0 ? std::size(*this) + x : x;
    }

  public:
    accumulation() noexcept(NO_EXCEPT) {}

    template<std::ranges::input_range R, class Operator = std::plus<T>>
        requires std::regular_invocable<Operator, T, T>
    explicit accumulation(R&& range, const T& head = T(), Operator&& op = std::plus<T>{})
      : accumulation(ALL(range), head, op)
    {}

    template<
        std::input_iterator I, std::sentinel_for<I> S,
        class Operator = std::plus<T>
    >
    accumulation(I first, S last, const T& head = T(), Operator&& op = std::plus<T>{}) noexcept(NO_EXCEPT) {
        this->resize(std::ranges::distance(first, last) + 1);
        *this->begin() = head;
        for(auto itr = std::ranges::begin(*this); first != last; ++first) {
            const auto prev = itr++;
            *itr = op(*prev, *first);
        }
    }

    template<class Operator = std::minus<T>>
        requires std::regular_invocable<Operator, T, T>
    inline T operator()(size_type left, size_type right, Operator&& op = std::minus<T>{}) const noexcept(NO_EXCEPT) {
        left = _positivize_index(left), right = _positivize_index(right);
        assert(0 <= left and left <= right and right < (size_type)std::size(*this));
        return op((*this)[right], (*this)[left]);
    }
};

template<std::input_iterator I, std::sentinel_for<I> S>
explicit accumulation(I, S) -> accumulation<typename std::iterator_traits<I>::value_type>;

template<std::ranges::input_range R>
explicit accumulation(R&&) -> accumulation<typename std::ranges::range_value_t<R>>;


template<class T, class Container = vector<vector<T>>, class Operator = std::plus<T>>
struct accumulation_2d : Container {
    using size_type = internal::size_t;

  protected:
    inline size_type _positivize_index(const size_type x) const noexcept(NO_EXCEPT) {
        return x < 0 ? std::size(*this) + x : x;
    }

    Operator _op;

  public:
    accumulation_2d() noexcept(NO_EXCEPT) {}

    template<std::ranges::input_range R>
        requires
            std::ranges::input_range<typename std::ranges::range_value_t<R>> &&
            std::regular_invocable<Operator, T, T>
    explicit accumulation_2d(R&& range, const T& head = T(), Operator&& op = std::plus<T>{})
      : accumulation_2d(ALL(range), head, op)
    {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    accumulation_2d(I first, S last, const T head = T{}, const Operator op = std::plus<T>{}) noexcept(NO_EXCEPT) : _op(op) {
        const auto h = static_cast<size_type>(std::ranges::distance(first, last));
        const auto w = static_cast<size_type>(std::ranges::distance(ALL(*first)));
        {
            auto row = first;
            this->assign(h + 1, {});
            (*this)[0].assign(w + 1, head);
            REP(i, h) {
                assert(w == std::ranges::ssize(*row));
                (*this)[i + 1].assign(w + 1, head);
                REP(j, w) (*this)[i + 1][j + 1] = first[i][j];
                ++row;
            }
        }
        FOR(i, 1, h) FOR(j, w) (*this)[i][j] = op((*this)[i][j], (*this)[i - 1][j]);
        FOR(i, h) FOR(j, 1, w) (*this)[i][j] = op((*this)[i][j], (*this)[i][j - 1]);
    }

    template<class Rev = std::minus<T>>
    inline T operator()(size_type a, size_type b, size_type c, size_type d, const Rev rev = std::minus<T>{}) const noexcept(NO_EXCEPT) {
        a = _positivize_index(a), b = _positivize_index(b);
        c = _positivize_index(c), d = _positivize_index(d);
        assert(0 <= a and a <= b and b < (size_type)std::size(*this));
        assert(0 <= c and c <= d and d < (size_type)std::size((*this)[0]));
        return this->_op(rev((*this)[b][d], this->_op((*this)[a][d], (*this)[b][c])), (*this)[a][c]);
    }

    template<class Rev = std::minus<T>>
    inline T operator()(const std::pair<size_type,size_type> p, const std::pair<size_type,size_type> q, const Rev rev = std::minus<T>{}) const noexcept(NO_EXCEPT) {
        return this->operator()(p.first, p.second, q.first, q.second, rev);
    }
};

template<std::input_iterator I, std::sentinel_for<I> S>
explicit accumulation_2d(I, S) ->
    accumulation_2d<
        typename std::iterator_traits<typename std::ranges::iterator_t<typename std::iterator_traits<I>::value_type>>::value_type
    >;

template<std::input_iterator I, std::sentinel_for<I> S>
explicit accumulation_2d(I, S) ->
    accumulation_2d<
        typename std::iterator_traits<typename uni::internal::iterator_t<typename std::iterator_traits<I>::value_type>>::value_type
    >;

template<std::ranges::input_range R>
    requires std::ranges::input_range<typename std::ranges::range_value_t<R>>
explicit accumulation_2d(R&&) ->
    accumulation_2d<
        typename std::ranges::range_value_t<typename std::ranges::range_value_t<R>>
    >;


} // namespace uni
