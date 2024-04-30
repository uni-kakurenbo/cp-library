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
#include "utility/functional.hpp"

template<class T, class Tree0, class Tree1 = Tree0>
void test(const uni::timer::time_point modify_limit, const uni::timer::time_point check_limit) {
    using value_type = T;

    Tree0 t0;
    Tree1 t1;

    uni::timer timer(modify_limit);
    uni::random_adaptor<uni::random_engine_64bit> rng;


    uni::i32 modify_count = 0;
    while(!timer.expired()) {
        ++modify_count;
        const uni::i64 t = rng(4);

        const uni::i64 l = rng(-uni::to_signed(uni::shiftl(1UL, rng(60))), uni::shiftl(1UL, rng(60)));
        uni::i64 r = l + rng(uni::shiftl(1UL, rng(60)));

        t0.expand(l, r + 1);
        t1.expand(l, r + 1);

        if(t == 0) {
            const uni::i64 u = rng(5);

            if(u == 0) {
                t0.reverse(l, r), t1.reverse(l, r);
            }
            else if(u == 1) {
                if(l == r) ++r;

                const uni::i64 m = rng(l, r);
                const uni::i64 v = rng(2);

                t0.rotate(l, m, r);

                if(v == 0) t1.rotate(l, m, r);
                else {
                    t1.reverse(l, m), t1.reverse(m, r), t1.reverse(l, r);
                }
            }
            else if(u == 2) {
                if(l == r) ++r;

                const uni::i64 v = rng(4);
                const uni::i64 p = rng(uni::shiftl(1UL, rng(60)));

                if(v == 0) t0.shift_left(l, r, p), t1.shift_left(l, r, p);
                else if(v == 1) t0.shift_right(l, r, p), t1.shift_right(l, r, p);
                else if(v == 2) t0.shift_left(l, r, p), t1.shift_right(l, r, -p);
                else t0.shift_right(l, r, p), t1.shift_left(l, r, -p);
            }
            else if(u == 3) {
                if(l == r) ++r;

                const uni::i64 v = rng(4);
                const uni::i64 p = rng(uni::shiftl(1UL, rng(60)));

                if(v == 0) t0.rotate_left(l, r, p), t1.rotate_left(l, r, p);
                else if(v == 1) t0.rotate_right(l, r, p), t1.rotate_right(l, r, p);
                else if(v == 2) t0.rotate_left(l, r, p), t1.rotate_right(l, r, -p);
                else t0.rotate_right(l, r, p), t1.rotate_left(l, r, -p);
            }
            else {
                if(l == r) ++r;

                const uni::i64 v = rng(2);
                uni::i64 p = rng(uni::shiftl(1UL, rng(60)));

                if(v == 0) {
                    t0.shift_left(l, r, p);

                    uni::chmin(p, r - l);

                    t1.insert(r, {}, p);

                    auto val = t1.fold(l, l + p);
                    assert(val == t1.pop(l, p));
                }
                else {
                    t0.shift_right(l, r, p);

                    uni::chmin(p, r - l);

                    auto val = t1.fold(r - p, r);
                    assert(val == t1.pop(r - p, p));

                    t1.insert(l, {}, p);
                }
            }
        }
        else if(t == 1) {
            const uni::i64 u = rng(2);

            if(u == 0) {
                const uni::i64 v = rng(3);
                const value_type a = rng(uni::shiftl(1UL, 30));

                if(v == 0) {
                    t0.insert(l, a);
                    const uni::i64 w = rng(2);

                    if(w == 0) t1.insert(l, a);
                    else {
                        t1.reverse(l, r), t1.insert(r, a), t1.reverse(l, r + 1);
                    }
                }
                else if(v == 1) {
                    t0.push_back(a);
                    const uni::i64 w = rng(2);

                    if(w == 0) t1.push_back(a);
                    else t1.reverse(), t1.push_front(a), t1.reverse();
                }
                else {
                    t0.push_front(a);
                    const uni::i64 w = rng(2);

                    if(w == 0) t1.push_front(a);
                    else t1.reverse(), t1.push_back(a), t1.reverse();
                }
            }
            else {
                const uni::i64 v = rng(3);

                if(v == 0) {
                    t0.fill(l, r, {});

                    const uni::i64 w = rng(3);

                    if(w == 0) t1.fill(l, r, {});
                    else if(w == 1) t1.shift_left(l, r, r - l);
                    else t1.shift_right(l, r, r - l);
                }
                else if(v == 1) {
                    t0.pop_back();
                    const uni::i64 w = rng(2);

                    if(w == 0) t1.pop_back();
                    else t1.reverse(), t1.pop_front(), t1.reverse();
                }
                else {
                    t0.pop_front();
                    const uni::i64 w = rng(2);

                    if(w == 0) t1.pop_front();
                    else t1.reverse(), t1.pop_back(), t1.reverse();
                }
            }
        }
        else if(t == 2) {
            const uni::i64 u = rng(2);

            if(u == 0) {
                const uni::i64 v = rng(3);

                if(v == 0) {
                    const value_type a = rng(uni::shiftl(1UL, 30));
                    const value_type b = rng(uni::shiftl(1UL, 30));

                    t0(l, r) *= { a, b }, t1(l, r) *= { a, b };
                }
                else if(v == 1) {
                    const value_type a = rng(uni::shiftl(1UL, 30));

                    t0(l, r) *= { 0, a };
                    t1(l, r) = a;
                }
                else {
                    const value_type a = rng(uni::shiftl(1UL, 30));

                    t0[l] *= { 1, a };
                    t1[l] += a;
                }
            }
            else {
                const uni::i64 v = rng(2);
                const value_type a = rng(uni::shiftl(1UL, 30));

                if(v == 0) {
                    t0(l, r) = a;
                    t1(l, r) = a;
                }
                else {
                    t0(l, r) = a;
                    t1(l, r) *= { 0, a };
                }
            }
        }
        else {
            assert(t0(l, r).fold() == t1(l, r).fold());
        }

        assert(t0.size() == t1.size());
    }
    debug(modify_count);

    timer.reset(check_limit);

    uni::i32 check_count = 0;
    while(!timer.expired()) {
        ++check_count;

        const uni::i64 l = rng(-uni::to_signed(uni::shiftl(1UL, rng(60))), uni::shiftl(1UL, rng(60)));
        const uni::i64 r = l + rng(uni::shiftl(1UL, rng(60)));

        assert(t0(l, r).fold() == t1(l, r).fold());
        assert(t0[l].val() == t1[l].val());
        assert(t0[r].val() == t1[r].val());
    }

    debug(check_count);
}
