/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/seqetitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */
// #define DEBUGGER_ENABLED

#include "template/standard.hpp"

// void solve();

// signed main() {
//     debug(__cplusplus);
//     int $ = 1;
//     // std::cin >> $;
//     for(int _ = 0; _ < $; ++_) {
//         debug_("Case: #" + std::to_string(_));
//         solve();
//     }
//     return 0;
// }

#include "benchmark/benchmark.h"
/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/seqetitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#include "snippet/aliases.hpp"
#include "utility/timer.hpp"
#include "random/engine.hpp"
#include "random/adaptor.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "data_structure/red_black_tree.hpp"
#include "action/range_affine_range_sum.hpp"
#include "utility/timer.hpp"

template<class T, class Tree>
void test(const i32 times, const i64 range) {
    Tree seq(range);
    std::println("{}", seq.size());

    uni::random_adaptor<uni::random_engine_64bit> rng;
    uni::timer timer(30'0000);

    REP(times) {
        if(times % 100 == 0 && timer.expired()) break;

        std::array<uni::u64, 4> is = { rng(range), rng(range), rng(range), rng(range) };
        std::ranges::sort(is);

        seq.swap_ranges(is[0], is[1], is[2], is[3]);
    }

    assert(seq.size() == range);
}


template<bool COMP>
static void treap(benchmark::State& state) {
    while(state.KeepRunning()) {
        using Target = uni::dynamic_sequence<uni::actions::range_affine_range_sum<uni::i32>, uni::treap_context<uni::i64, COMP>>;
        test<i64, Target>(1LL << 16, 1LL << state.range());
    }
}

template<bool COMP>
static void rbt(benchmark::State& state) {
    while(state.KeepRunning()) {
        using Target = uni::dynamic_sequence<uni::actions::range_affine_range_sum<uni::i32>, uni::red_black_tree_context<uni::i64, COMP>>;
        test<i64, Target>(1LL << 16, 1LL << state.range());
    }
}

template<bool COMP>
static void per_rbt(benchmark::State& state) {
    while(state.KeepRunning()) {
        using Target = uni::dynamic_sequence<uni::actions::range_affine_range_sum<uni::i32>, uni::persistent_red_black_tree_context<uni::i64, COMP>>;
        test<i64, Target>(1LL << 16, 1LL << state.range());
    }
}


BENCHMARK(treap<false>)->Name("treap-A")->DenseRange(4, 60, 4)->Repetitions(100);
BENCHMARK(treap<true>)->Name("treap-B")->DenseRange(4, 60, 4)->Repetitions(100);


BENCHMARK(rbt<false>)->Name("rbt-A")->DenseRange(4, 60, 4)->Repetitions(100);
BENCHMARK(rbt<true>)->Name("rbt-B")->DenseRange(4, 60, 4)->Repetitions(100);


BENCHMARK(per_rbt<false>)->Name("per-rbt-A")->DenseRange(4, 60, 4)->Repetitions(100);
BENCHMARK(per_rbt<true>)->Name("per-rbt-B")->DenseRange(4, 60, 4)->Repetitions(100);


BENCHMARK_MAIN();
