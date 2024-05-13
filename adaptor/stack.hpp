#pragma once


#include <stack>
#include <vector>


namespace uni {


template<class T, class Allocator = std::allocator<T>>
using stack = std::stack<T, std::vector<T, Allocator>>;


} // namespace uni
