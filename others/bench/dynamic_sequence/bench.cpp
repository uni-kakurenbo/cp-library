/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 28 GCC */
// #define DEBUGGER_ENABLED

#include "benchmark/benchmark.h"

#include "template/standard.hpp"

#include "data_structure/dynamic_sequence.hpp"
#include "data_structure/red_black_tree.hpp"

#include "utility/timer.hpp"

template<class T, class Tree>
static void test(const i32 times, const i64 range) {
    uni::random_adaptor<uni::random_engine_64bit> rng;

    Tree seq(range);

    REP(times) {
        std::array<uni::u64, 4> is = { rng(range), rng(range), rng(range), rng(range) };
        std::ranges::sort(is);

        seq.swap_ranges(is[0], is[1], is[2], is[3]);
    }

    assert(seq.size() == range);
}


template<class Context>
static void bench(benchmark::State& state) {
    const auto n = 1LL << state.range();

    while(state.KeepRunning()) {
        using Target = uni::dynamic_sequence<char, Context>;
        test<i64, Target>(1LL << 16, n);
    }
}


BENCHMARK(bench<uni::treap_context<uni::i64, false>>)
    ->Name("treap-A")->DenseRange(4, 28, 4);

BENCHMARK(bench<uni::treap_context<uni::i64, true>>)
    ->Name("treap-B")->DenseRange(4, 60, 4);


BENCHMARK(bench<uni::red_black_tree_context<uni::i64, false>>)
    ->Name("rbt-A")->DenseRange(4, 24, 4);
BENCHMARK(bench<uni::red_black_tree_context<uni::i64, true>>)
    ->Name("rbt-B")->DenseRange(4, 60, 4);


BENCHMARK(bench<uni::persistent_red_black_tree_context<uni::i64, false>>)
    ->Name("per-rbt-A")->DenseRange(4, 24, 4);

BENCHMARK(bench<uni::persistent_red_black_tree_context<uni::i64, true>>)
    ->Name("per-rbt-B")->DenseRange(4, 60, 4);


BENCHMARK_MAIN();
