#pragma once


#include <ranges>

#include "snippet/aliases.hpp"

#include "internal/concepts.hpp"

#include "adapter/vector.hpp"


namespace lib {


// Thanks to: https://maspypy.github.io/library/nt/stern_brocot_tree.hpp
template<
    class Value = i32,
    class Middle = i64,
    class Large = i128,
    class Fraction = spair<Value>, std::ranges::range Path = lib::vector<Value>
>
struct stern_brocot_tree {
    using value_type = Value;
    using middle_type = Middle;
    using large_type = middle_type;
    using path_type = Path;
    using fraction_type = Fraction;

    static constexpr std::tuple<path_type, fraction_type, fraction_type> path_and_rang(const fraction_type x) {
        path_type path;
        fraction_type l = { 0, 1 }, r = { 1, 0 };
        fraction_type m = { 1, 1 };
        middle_type det_l = l.first * x.second - l.second * x.first;
        middle_type det_r = r.first * x.second - r.second * x.first;
        middle_type det_m = det_l + det_r;
        while(true) {
            if(det_m == 0) break;
            middle_type k = lib::div_ceil(-det_m, det_r);
            path.emplace_back(k);
            l = { l.first + k * r.first, l.second + k * r.second };
            m = { l.first + r.first, l.second + r.second };
            det_l += k * det_r;
            det_m += k * det_r;
            if(det_m == 0) break;
            k = lib::div_ceil(det_m, -det_l);
            path.emplace_back(k);
            r = { r.first + k * l.first, r.second + k * l.second };
            m = { l.first + r.first, l.second + r.second };
            det_r += k * det_l;
            det_m += k * det_l;
        }
        return { path, l, r };
    }

    static constexpr path_type path(const fraction_type x) {
        const auto [ path, l, r ] = path_and_rang(x);
        return path;
    }

    static constexpr pair<fraction_type, fraction_type> range(const fraction_type x) {
        const auto [ path, l, r ] = path_and_rang(x);
        return { l, r };
    }

    // x in range(y)
    static constexpr bool in_subtree(const fraction_type x, const fraction_type y) {
        const auto [ l, r ] = range(y);
        const bool ok_l = static_cast<middle_type>(x.first) * l.second - static_cast<middle_type>(x.second) * l.first > 0;
        const bool ok_r = static_cast<middle_type>(r.first) * x.second - static_cast<middle_type>(r.second) * x.first > 0;
        return ok_l && ok_r;
    }

    template<class T = middle_type, class Frac = spair<middle_type>>
    static constexpr Frac from_path(const path_type &p) {
        Frac l = { 0, 1 }, r = { 1, 0 };
        FOR(i, std::ranges::size(p)) {
            middle_type k = p[i];
            if((i & 1) == 0) {
                l.first += static_cast<T>(k) * r.first;
                l.second += static_cast<T>(k) * r.second;
            }
            if((i & 1) == 1) {
                r.first += static_cast<T>(k) * l.first;
                r.second += static_cast<T>(k) * l.second;
            }
        }
        return { l.first + r.first, l.second + r.second };
    }

    inline static constexpr pair<fraction_type, fraction_type> children(const fraction_type x) {
        const auto [ l, r ] = range(x);
        const fraction_type lc = { l.first + x.first, l.second + x.second };
        const fraction_type rc = { x.first + r.first, x.second + r.second };
        return { lc, rc };
    }

    inline static constexpr fraction_type lowest_common_ancestor(const fraction_type x, const fraction_type y) {
        const auto px = path(x);
        const auto py = path(y);
        path_type path;
        FOR(i, lib::min(std::ranges::ssize(px), std::ranges::ssize(py))) {
            middle_type k = lib::min(px[i], py[i]);
            path.emplace_back(k);
            if(k < px[i] || k < py[i]) break;
        }
        return from_path(path);
    }

    inline static constexpr fraction_type ancestor(const fraction_type x, middle_type dep) {
        fraction_type l = { 0, 1 }, r = { 1, 0 };
        fraction_type m = { 1, 1 };
        middle_type det_l = l.first * x.second - l.second * x.first;
        middle_type det_r = r.first * x.second - r.second * x.first;
        middle_type det_m = det_l + det_r;
        while(true) {
            if(det_m == 0 || dep == 0) break;
            middle_type k = lib::min(dep, lib::div_ceil(-det_m, det_r));
            l = { l.first + k * r.first, l.second + k * r.second };
            m = { l.first + r.first, l.second + r.second };
            det_l += k * det_r;
            det_m += k * det_r;
            dep -= k;
            if(det_m == 0 || dep == 0) break;
            k = lib::min(dep, lib::div_ceil(det_m, -det_l));
            r = {r.first + k * l.first, r.second + k * l.second };
            m = { l.first + r.first, l.second + r.second };
            det_r += k * det_l;
            det_m += k * det_l;
            dep -= k;
        }
        if(dep == 0) return m;
        return { -1, -1 };
    }

    inline static constexpr std::string to_string(const path_type &p) {
        std::string res;
        char c = 'L';
        ITR(x, p) {
            c = 'L' + 'R' - c;
            if(x == 0) continue;
            res += c;
            res += " " + std::to_string(x);
        }
        return res;
    }
};


} // namespace lib
