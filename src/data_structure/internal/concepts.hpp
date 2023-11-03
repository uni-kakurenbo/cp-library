#pragma once

#include <concepts>


namespace lib {

namespace internal {


template<
    template<class...> class Structure,
    class Action
>
concept available_for = std::default_initializable<Structure<Action>>;


} // namespace internal

} // namespace lib
