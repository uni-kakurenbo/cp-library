#pragma once


#include <concepts>
#include <ranges>


#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "adaptor/vector.hpp"
#include "view/cyclic.hpp"

#include "geometry/point.hpp"
#include "geometry/arrow.hpp"


namespace uni {


template<class Point, class Container = vector<Point>>
struct polygon : Container {
    using point_type = Point;
    using container_type = Container;

    using value_type = point_type::value_type;
    using size_type = Container::size_type;

    static_assert(std::same_as<typename container_type::value_type, point_type>);


    using container_type::container_type;


    auto area() noexcept(NO_EXCEPT) {
        const auto n = std::ranges::ssize(*this);
        const auto view = uni::views::cyclic(*this);

        value_type res = 0;
        REP(i, n) res += cross(view[i], view[i + 1]);

        return res / 2;
    }


    template<bool = true>
    bool is_convex() noexcept(NO_EXCEPT);


    // implemented in geometry/convex_hull.hpp
    template<bool = true, bool = false>
    auto convex_hull() noexcept(NO_EXCEPT);

};


template<class Point, class Container>
struct convex_polygon : polygon<Point, Container> {
    using polygon<Point, Container>::polygon;
};


namespace internal {


template<bool ALLOW_LINE, std::ranges::sized_range R>
    requires std::ranges::forward_range<R>
bool is_convex(R&& range) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::size(range);
    const auto view = uni::views::cyclic(range);

    auto itr0 = std::ranges::begin(view);
    auto itr1 = std::ranges::next(itr0);
    auto itr2 = std::ranges::next(itr1);

    REP(n) {
        const auto r = arrow(*(itr0++), *(itr1++)).relation(*(itr2++));

        if constexpr(ALLOW_LINE) {
            if(r == positional_relation::clockwise) return false;
        }
        else {
            if(r != positional_relation::anti_clockwise) return false;
        }
    }

    return true;
}


} // namespace internal


template<class Point, class Container>
template<bool ALLOW_LINE>
inline bool polygon<Point, Container>::is_convex() noexcept(NO_EXCEPT) {
    return
        (
            internal::is_convex<ALLOW_LINE>(*this) ||
            internal::is_convex<ALLOW_LINE>(std::views::reverse(*this))
        );
}


} // namespace uni
