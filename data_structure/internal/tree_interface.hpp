#pragma once


#include <concepts>
#include <type_traits>

#include "internal/concepts.hpp"


namespace lib {

namespace internal {


template<class T>
concept tree_interface =
    std::default_initializable<T> &&
    std::integral<typename T::key_type> &&
    requires (T interface, const T const_interface, typename T::key_type key, typename T::node_type<>* tree, const typename T::node_type<>* const_tree) {
        const_interface.split(const_tree, key, tree, tree);
        interface.merge(tree, const_tree, const_tree);
    };


} // namespace internal


} // namespace name
