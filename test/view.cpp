#include "template.hpp"
#include "views.hpp"

signed main() {

    vector<int> a = { 1, 2, 3, 4, 5 };
    vector<int> b = { 2, 4, 6, 8, 10 };
    vector<int> c = { 3, 6, 9, 12, 15 };

    debug(lib::views::zip(lib::views::range(a)));

    using itr = decltype(lib::views::concat(a, b, c))::iterator;
    static_assert(std::is_same_v<
        std::iter_rvalue_reference_t<itr>,
        std::iter_rvalue_reference_t<const itr>
    >);

    {
        debug(lib::views::zip(a));
        debug(lib::views::zip(a, b));
        debug(lib::views::zip(a, b, c));
        // lib::zip(a): [ <tuple> ( 1 ), <tuple> ( 2 ), <tuple> ( 3 ), <tuple> ( 4 ), <tuple> ( 5 ) ]
        // lib::zip(a, b): [ <tuple> ( 1, 2 ), <tuple> ( 2, 4 ), <tuple> ( 3, 6 ), <tuple> ( 4, 8 ), <tuple> ( 5, 10 ) ]
        // lib::zip(a, b, c): [ <tuple> ( 1, 2, 3 ), <tuple> ( 2, 4, 6 ), <tuple> ( 3, 6, 9 ), <tuple> ( 4, 8, 12 ), <tuple> ( 5, 10, 15 ) ]
    }
    {
        debug(lib::views::concat(a));
        debug(lib::views::concat(a, b));
        debug(lib::views::concat(a, b, c));
        // lib::concat(a): [ 1, 2, 3, 4, 5 ]
        // lib::concat(a, b): [ 1, 2, 3, 4, 5, 2, 4, 6, 8, 10 ]
        // lib::concat(a, b, c): [ 1, 2, 3, 4, 5, 2, 4, 6, 8, 10, 3, 6, 9, 12, 15 ]
    }
    {
        for(auto& x : lib::views::concat(a, b, c)) x = 1;
        debug(a, b, c);
        // a: [ 1, 1, 1, 1, 1 ], b: [ 1, 1, 1, 1, 1 ], c: [ 1, 1, 1, 1, 1 ]

        for(auto [ x, y, z ] : lib::views::zip(a, b, c)) x = 1, y = 2, z = 3;
        debug(a, b, c);
    }

    // {
    //     lib::cyclic_view cyc_view_vec1d(vec1d);
    //     debug(cyc_view_vec1d);

    //     cyc_view_vec1d.drop(50).take(vec1d.size());
    //     cyc_view_vec1d[1] = 5;
    //     debug(cyc_view_vec1d);

    //     cyc_view_vec1d.drop(2).take(4);
    //     cyc_view_vec1d[120] = 9;
    //     debug(cyc_view_vec1d);
    // }


    std::vector<std::vector<int>> vec2d = {
        { 1, 42, 2, 8, 2, 23, 112, 0, 4 },
        { 5, 2, 3, 1, 2, 0, 63, 192, 92 },
        { 9, 2, 5, 1, 53, 12, 12, 3, 23 },
        { 9, 4, 5, 12, 1, 9, 43, 21, 10 }
    };
    debug(vec2d);

    // {
    //     lib::multi_view<decltype(vec2d),2> view_vec2d(&vec2d);

    //     debug(view_vec2d[{0, 0}]);
    //     view_vec2d[{0, 0}]++;

    //     view_vec2d.drop({ 2, 2 });
    //     view_vec2d(0, 0) *= 5;
    //     debug(vec2d);

    //     view_vec2d.drop();

    //     view_vec2d.transpose(
    //         [&](std::initializer_list<int> pos) -> std::vector<int> {
    //             return {  4 - pos.begin()[0] - 1, 9 - pos.begin()[1] - 1 };
    //         }
    //     );

    //     REP(i, vec2d.size()) REP(j, vec2d[0].size()) {
    //         debug(view_vec2d(i, j));
    //     }

    // }
}
