#pragma once


#include <concepts>
#include <type_traits>

#include "internal/concepts.hpp"


namespace uni {

namespace internal {


template<class T>
concept basic_tree =
    std::default_initializable<T> &&
    std::integral<typename T::size_type> &&
    requires (T base, typename T::size_type key, typename T::node_pointer tree, const typename T::node_pointer const_tree) {
        base.split(const_tree, key, tree, tree);
        base.merge(tree, const_tree, const_tree);
    };


} // namespace internal


} // namespace name
