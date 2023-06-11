#pragma once


#include <tuple>


namespace lib {


template<typename F, typename ...T>
void for_each(std::tuple<T...> const& t, F f){
    std::apply([f](auto&... args) constexpr { (f(args), ...); }, t);
}


}
