#pragma once


#include <cassert>
#include <type_traits>

#include "internal/dev_env.hpp"


namespace uni {


template<class Engine>
struct random_adaptor {
    using result_type = typename Engine::result_type;
    using signed_result_type = typename std::make_signed_t<result_type>;

  private:
    Engine engine;

  public:
    static constexpr result_type MIN = Engine::min();
    static constexpr result_type MAX = Engine::max();

    static constexpr result_type min() noexcept(NO_EXCEPT) { return MIN; }
    static constexpr result_type max() noexcept(NO_EXCEPT) { return MAX; }

    constexpr random_adaptor(unsigned long seed = 3141592653UL) noexcept(NO_EXCEPT) { this->engine.seed(seed); };

    inline constexpr result_type operator()() noexcept(NO_EXCEPT) {
        return this->engine();
    }

    inline constexpr result_type operator()(const result_type sup) noexcept(NO_EXCEPT) {
        assert(0 < sup);
        return this->engine() % sup;
    }
    inline constexpr signed_result_type operator()(const signed_result_type min, const signed_result_type sup) noexcept(NO_EXCEPT) {
        assert(min < sup);
        return min + (*this)(sup - min);
    };

    template<class T = double>
    inline constexpr auto real() noexcept(NO_EXCEPT) {
        const auto v = static_cast<T>((this->engine() + 0.5) / (1.0 + this->max()));
        return static_cast<T>((this->operator()() + v) / (1.0 + this->max()));
    }
};


} // namespace uni
