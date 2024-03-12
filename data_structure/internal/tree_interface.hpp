#pragma once


#include <concepts>
#include <type_traits>

#include "internal/concepts.hpp"


namespace lib {

namespace internal {


template<class T>
concept tree_interface =
    std::default_initializable<T> &&
    std::integral<typename T::size_type> &&
    requires (T interface, typename T::size_type key, typename T::node_pointer tree, const typename T::node_pointer const_tree) {
        interface.split(const_tree, key, tree, tree);
        interface.merge(tree, const_tree, const_tree);
    };


} // namespace internal


} // namespace name
