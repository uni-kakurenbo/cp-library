#pragma once

#include <utility>
#include <functional>

#include "internal/dev_env.hpp"


namespace lib {


template<class> struct virtual_combined_map {};

template<class Mapped, class... Keys>
struct virtual_combined_map<Mapped(Keys...)> {
    using key_type = std::tuple<Keys...>;
    using mapped_type = Mapped;
    using value_type = std::pair<key_type,mapped_type>;

  protected:
    std::function<Mapped(Keys...)> _f;

  public:
    virtual_combined_map() = default;

    template<class F>
    explicit virtual_combined_map(F&& f) noexcept(NO_EXCEPT) : _f(f) {};

    inline const auto* get_functor() const noexcept(NO_EXCEPT) { return this->_f; }
    inline auto* get_functor() noexcept(NO_EXCEPT) { return this->_f; }

    template<class F>
    inline auto& set_functor(F&& f) const noexcept(NO_EXCEPT) { return this->_f = f; }

    inline mapped_type operator()(const Keys&... key) const noexcept(NO_EXCEPT) {
        return this->_f(key...);
    }

    inline mapped_type operator[](const key_type& key) const noexcept(NO_EXCEPT) {
        return std::apply(this->_f, key);
    }
};

template<class Key, class Mapped>
struct virtual_map : virtual_combined_map<Mapped(Key)> {
    using key_type = Key;
    using mapped_type = Mapped;
    using value_type = std::pair<key_type,mapped_type>;

    using virtual_combined_map<mapped_type(key_type)>::virtual_combined_map;

    inline mapped_type operator[](const key_type& key) const noexcept(NO_EXCEPT) {
        return this->_f(key);
    }
};

}; // namesapce lib
