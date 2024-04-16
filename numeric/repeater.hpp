#pragma once


#include <concepts>
#include <functional>
#include <cstddef>


#include "internal/exception.hpp"
#include "internal/types.hpp"

#include "numeric/bit.hpp"


namespace uni {


template<class T, internal::size_t SUP, internal::size_t DEFAULT_DEPTH = 64>
struct repeater {
    using value_type = T;
    using size_type = internal::size_t;

  private:
    std::vector<std::array<T, SUP>> _app;

    inline void _extend(const std::size_t k) noexcept(NO_EXCEPT) {
        const auto size = this->_app.size();
        const auto w = to_unsigned(std::bit_width(k));

        if(w <= size) return;
        this->_app.resize(w);

        REP(d, size, w) {
            REP(n, SUP) {
                this->_app[d][n] = this->_app[d - 1][this->_app[d - 1][n]];
            }
        }
    }

    template<std::unsigned_integral K>
    struct applyer {
      private:
        K _k;
        repeater* _super;

      public:
        applyer(const K k, repeater *const super) noexcept(NO_EXCEPT) : _k(k), _super(super) {}

        inline K times() const noexcept(NO_EXCEPT) { return this->_k; }

        inline value_type operator()(value_type val) const noexcept(NO_EXCEPT) {
            REP(d, this->_super->_app.size()) {
                if(uni::bit(this->_k, d)) val = this->_super->_app[d][val];
            }
            return val;
        }
    };


  public:
    template<class F>
        requires std::convertible_to<std::invoke_result_t<F, T>, T>
    explicit repeater(F&& f) noexcept(NO_EXCEPT) : _app(1) {
        this->_app.reserve(DEFAULT_DEPTH);
        REP(n, SUP) this->_app[0][n] = static_cast<value_type>(f(n));
    };

    template<std::ranges::sized_range R>
    explicit repeater(R&& f) noexcept(NO_EXCEPT) : _app(1) {
        assert(std::ranges::size(f) <= SUP);

        this->_app.reserve(DEFAULT_DEPTH);
        std::ranges::copy(f, std::ranges::begin(this->_app[0]));
    };


    template<std::integral K>
    inline auto operator[](K k) noexcept(NO_EXCEPT) {
        if(std::signed_integral<K>) assert(0 <= k);
        this->_extend(static_cast<std::size_t>(k));
        return applyer{ to_unsigned(k), this };
    }
};


} // namespace uni
