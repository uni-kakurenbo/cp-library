/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#include "snippet/aliases.hpp"
#include "utility/timer.hpp"
#include "random/engine.hpp"
#include "random/adaptor.hpp"

constexpr std::size_t RANGE = 300;

template<class T, class Tree>
void test(const uni::timer::time_point modify_limit, const uni::timer::time_point check_limit) {
    using value_type = T;

    std::vector<T> corr(RANGE);
    Tree comp(RANGE);

    uni::timer timer(modify_limit);
    uni::random_adaptor<uni::random_engine_64bit> rng;

    uni::i32 modify_count = 0;
    while(!timer.expired()) {
        ++modify_count;
        const uni::i64 t = rng(4);

        uni::i64 l = rng(RANGE);
        uni::i64 r = rng(RANGE);
        if(l > r) std::swap(l, r);

        if(t == 0) {
            const uni::i64 u = rng(3);

            if(u == 0) {
                std::ranges::reverse(corr | std::views::take(r) | std::views::drop(l));
                comp.reverse(l, r);
            }
            else if(u == 1) {
                if(l == r) ++r;

                const uni::i64 m = rng(l, r);
                const uni::i64 v = rng(2);

                std::ranges::rotate(corr | std::views::take(r) | std::views::drop(l), corr.begin() + m);

                if(v == 0) comp.rotate(l, m, r);
                else comp.reverse(l, m), comp.reverse(m, r), comp.reverse(l, r);
            }
            else {
                if(l == r) ++r;

                const uni::i64 v = rng(2);
                const uni::i64 p = rng(r - l);

                if(v == 0) {
                    auto view = corr | std::views::take(r) | std::views::drop(l);
                    std::shift_left(ALL(view), p);
                    std::ranges::fill(view | std::views::drop(r - l - p), T{});
                    comp.shift_left(l, r, p);
                }
                else {
                    auto view = corr | std::views::take(r) | std::views::drop(l);
                    std::shift_right(ALL(view), p);
                    std::ranges::fill(view | std::views::take(p), T{});
                    comp.shift_right(l, r, p);
                }
            }
        }
        else if(t == 1) {
            const uni::i64 u = rng(3);

            std::ranges::fill(corr | std::views::take(r) | std::views::drop(l), T{});

            if(u == 0) comp.fill(l, r, {});
            else if(u == 1) comp.shift_left(l, r, r - l);
            else comp.shift_right(l, r, r - l);
        }
        else if(t == 2) {
            const uni::i64 u = rng(3);

            if(u == 0) {
                const value_type a = rng(uni::shiftl(1UL, 30));
                const value_type b = rng(uni::shiftl(1UL, 30));

                {
                    std::vector<T> res;

                    std::ranges::transform(
                        corr | std::views::take(r) | std::views::drop(l),
                        std::back_inserter(res),
                        [&](auto x) { return x * a + b; }
                    );

                    std::ranges::copy(res, corr.begin() + l);
                }

                comp(l, r) *= { a, b };
            }
            else if(u == 1) {
                const value_type a = rng(uni::shiftl(1UL, 30));

                std::ranges::fill(corr | std::views::take(r) | std::views::drop(l), a);
                comp(l, r) = a;
            }
            else {
                const value_type a = rng(uni::shiftl(1UL, 30));
                corr[l] += a;
                comp[l] += a;
            }
        }
        else {
            assert(std::accumulate(corr.begin() + l, corr.begin() + r, T{}) == comp(l, r).fold());
        }

        assert(std::ranges::equal(corr, comp));
    }

    debug(modify_count);


    timer.reset(check_limit);

    uni::i32 check_count = 0;
    while(!timer.expired()) {
        ++check_count;

        uni::i64 l = rng(RANGE + 1);
        uni::i64 r = rng(RANGE + 1);
        if(l > r) std::swap(l, r);

        const auto view = corr | std::views::take(r) | std::views::drop(l);
        assert(std::accumulate(ALL(view), T{}).val() == comp(l, r).fold());
        assert(corr[l].val() == comp[l].val());
        assert(corr[r].val() == comp[r].val());
    }

    debug(check_count);
}
