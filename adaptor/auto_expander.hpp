#pragma once


#include <functional>
#include <optional>
#include <cassert>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace uni {


template<class Container>
struct auto_expander : Container {
    using size_type = Container::size_type;

  public:
    template<class... Args>
    explicit auto_expander(Args&&... args) noexcept(NO_EXCEPT) : Container(std::forward<Args>(args)...) {}

    inline auto& operator[](const size_type pos) noexcept(NO_EXCEPT) {
        if(this->size() <= pos) this->Container::resize(pos + 1);
        return this->Container::operator[](pos);
    }
};


}
