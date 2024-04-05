#pragma once


#include <initializer_list>
#include <valarray>
#include <numeric>


#include "snippet/iterations.hpp"

#include "internal/types.hpp"
#include "internal/dev_env.hpp"

#include "adaptor/valarray.hpp"

#include <concepts>
#include <ranges>


namespace lib {


template<class T = internal::size_t>
struct leveler {
    using value_type = T;
    using size_type = internal::size_t;

  private:
    lib::valarray<value_type> _bases;
    size_type _dim;
    value_type _max;

    inline value_type _compute_max() const noexcept(NO_EXCEPT) {
        return std::reduce(std::begin(this->_bases), std::end(this->_bases), 1, std::multiplies<value_type>{});
    }

  public:
    leveler(const std::initializer_list<value_type> bases) noexcept(NO_EXCEPT) : _bases(bases), _dim(std::ranges::size(bases)) {
        this->_max = this->_compute_max();
    }

    template<class I>
    leveler(I first, I last) noexcept(NO_EXCEPT) : _bases(first, last), _dim(std::distance(first, last)) {
        this->_max = this->_compute_max();
    }

    template<std::ranges::forward_range R>
    leveler(R&& range) noexcept(NO_EXCEPT) : leveler(std::ranges::begin(range), std::ranges::end(range)) {}


    template<std::integral... Values>
    leveler(const Values... bases) noexcept(NO_EXCEPT) : leveler(std::initializer_list<value_type>{ bases... }) {}

    inline size_type dimension() const noexcept(NO_EXCEPT) { return this->_dim; }

    inline value_type sup() const noexcept(NO_EXCEPT) { return this->_max; }

    inline value_type convert(const std::initializer_list<value_type> inds) const noexcept(NO_EXCEPT) {
        return this->convert(inds | std::views::all);
    }

    template<std::ranges::forward_range R>
    inline value_type convert(R&& range) const noexcept(NO_EXCEPT) {
        assert(std::ranges::distance(range) == std::ranges::ssize(this->_bases));

        value_type res = 0;
        {
            auto size = std::ranges::begin(this->_bases);
            ITR(v, range) {
                assert(0 <= v and v < *size);
                res *= *(size++);
                res += v;
            }
        }

        return res;
    }

    template<std::integral... Values>
    inline value_type convert(const Values... inds) const noexcept(NO_EXCEPT) {
        return this->convert({ inds... });
    }

    inline lib::valarray<value_type> revert(value_type id) const noexcept(NO_EXCEPT) {
        assert(0 <= id and id < this->sup());

        lib::valarray<value_type> res(std::size(this->_bases));

        REPD(i, std::size(this->_bases)) {
            res[i] = id % this->_bases[i];
            id /= this->_bases[i];
        }

        return res;
    }

    auto _debug() const { return this->_bases; }
};


}
