/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_B"

#include "template/debug.hpp"

#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "adapter/string.hpp"
#include "data_structure/disjoint_sparse_table.hpp"
#include "action/range_sequence_hash.hpp"

signed main() {
    lib::string s, p; input >> s >> p;

    lib::disjoint_sparse_table<lib::actions::range_sequence_hash<>> hash(s + p);
    debug(hash);

    auto p_hash = hash(s.size(), hash.size()).fold();
    debug(p_hash);

    FOR(i, s.size() - p.size()) {
        debug(hash(i, i + p.size()).fold());
        if(p_hash == hash(i, i + p.size()).fold()) print(i);
    }
}
