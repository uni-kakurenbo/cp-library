#include "template.hpp"
#include "view.hpp"

signed main() {
    std::vector<int> vec1d = { 1, 42, 2, 8, 2, 23, 112, 0, 4 };
    debug(vec1d);

    {
        lib::view view_vec1d(&vec1d);
        debug(view_vec1d);

        view_vec1d.drop(0).take(3);
        debug(view_vec1d);

        view_vec1d[1] = 12;
        debug(view_vec1d);
        debug(vec1d);

        view_vec1d.drop(2);
        debug(view_vec1d);

        view_vec1d.take(5);
        debug(view_vec1d);

        view_vec1d.drop(3).take(5);
        debug(view_vec1d);

        view_vec1d.transpose([&](const int i) { return view_vec1d.size() - i - 1; });
        debug(view_vec1d);
    }

    {
        lib::cyclic_view cyc_view_vec1d(&vec1d);
        debug(cyc_view_vec1d.take());

        cyc_view_vec1d.drop(50).take();
        cyc_view_vec1d[1] = 5;
        debug(cyc_view_vec1d);

        cyc_view_vec1d.drop(2).take(4);
        cyc_view_vec1d[120] = 9;
        debug(cyc_view_vec1d);

        cyc_view_vec1d.transpose([&](const int i) { return cyc_view_vec1d.size() - i - 1; });
        debug(cyc_view_vec1d);
    }


    std::vector<std::vector<int>> vec2d = {
        { 1, 42, 2, 8, 2, 23, 112, 0, 4 },
        { 5, 2, 3, 1, 2, 0, 63, 192, 92 },
        { 9, 2, 5, 1, 53, 12, 12, 3, 23 },
        { 9, 4, 5, 12, 1, 9, 43, 21, 10 }
    };
    debug(vec2d);

    {
        lib::multi_view<decltype(vec2d),2> view_vec2d(&vec2d);

        debug(view_vec2d[{0, 0}]);
        view_vec2d[{0, 0}]++;

        view_vec2d.drop({ 2, 2 });
        view_vec2d(0, 0) *= 5;
        debug(vec2d);

        view_vec2d.drop();

        view_vec2d.transpose(
            [&](std::initializer_list<int> pos) -> std::vector<int> {
                return {  4 - pos.begin()[0] - 1, 9 - pos.begin()[1] - 1 };
            }
        );

        REP(i, vec2d.size()) REP(j, vec2d[0].size()) {
            debug(view_vec2d(i, j));
        }

    }
}
