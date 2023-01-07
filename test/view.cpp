#include "view.hpp"
#include "template.hpp"

signed main() {
    std::vector<int> vec1d = { 1, 42, 2, 8, 2, 23, 112, 0, 4 };
    lib::view view_vec1d(ALL(vec1d));
    debug(view_vec1d);

    view_vec1d.focus(0).resize(3);
    debug(view_vec1d);

    view_vec1d[1] = 12;
    debug(view_vec1d);
    debug(vec1d);

    view_vec1d.focus(2);
    debug(view_vec1d);

    view_vec1d.resize(5);
    debug(view_vec1d);

    view_vec1d.focus(3).resize(5);
    debug(view_vec1d);
}
