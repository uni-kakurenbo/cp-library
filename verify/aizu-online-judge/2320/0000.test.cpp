/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2320"

#include <iostream>
#include <string_view>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "adaptor/string.hpp"
#include "structure/grid.hpp"
#include "numeric/repeater.hpp"

constexpr std::string_view DIRS = "NESW";

signed main() {
    while(true) {
        uni::i32 h, w; uni::i64 l;
        input >> h >> w >> l;
        if(h == 0) break;

        uni::grid<char, uni::string> grid(h, w); input >> grid;
        debug(grid);

        uni::i32 sid = 0;
        REP(i, h) REP(j, w) {
            if(const auto d = DIRS.find(grid(i, j)); d != std::string_view::npos) {
                sid = grid.id(i, j) * 4 + d;
                debug(i, j, d);
            }
        }

        auto f = [&](uni::i64 id) -> uni::i64 {
            if(!grid.is_valid(grid.pos(id / 4))) return 0;
            if(grid(grid.pos(id / 4)) == '#') return 0;

            uni::i32 i, j, d = id % 4 - 1;

            uni::i32 cnt = 0;
            do {
                if(++cnt > 4) return 0;

                std::tie(i, j) = grid.pos(id / 4);
                d = uni::mod(d + 1, 4);
                debug(i, j, d);

                i += uni::DIRS4[d].first, j += uni::DIRS4[d].second;
            } while(!grid.is_valid(i, j) || grid(i, j) == '#');

            return grid.id(i, j) * 4 + d;
        };

        uni::repeater<uni::i64, 40000> repeater(f);
        {
            const uni::i64 tid = repeater[l](sid);
            const auto [ i, j ] = grid.pos(tid / 4);
            print(i + 1, j + 1, DIRS[tid % 4]);
        }
    }
}
