#pragma once


#include <iostream>
#include <limits>
#include <iterator>
#include <sstream>
#include <array>
#include <string>
#include <cstring>
#include <vector>
#include <bitset>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <utility>
#include <iomanip>
#include <ranges>
#include <concepts>
#include <type_traits>


#include "numeric/int128.hpp"

#include "internal/type_traits.hpp"
#include "internal/concepts.hpp"
#include "internal/resolving_rank.hpp"


namespace debugger {


template<class T>
auto _debug (T&& val) -> decltype(val._debug()) {
    return val._debug();
}


std::ostream *cdebug = &std::clog;


const std::string COLOR_INIT = "\033[m";
const std::string COLOR_STRING = "\033[33m";
const std::string COLOR_TYPE = "\033[34m";
const std::string COLOR_NUMERIC = "\033[36m";
const std::string COLOR_LITERAL_OPERATOR = "\033[31m";


using Brackets = std::pair<std::string, std::string>;


template<class T>
std::string dump(T&&);


struct debug_t : std::string {
    template<std::same_as<std::string> T>
    debug_t(const T& str) {
        this->assign(str);
    }
};


template<size_t N, class T>
void dump_tuple_impl([[maybe_unused]] T&& val, std::stringstream &res) {
    if constexpr(N < std::tuple_size_v<std::decay_t<T>>) {
        res << dump(std::get<N>(val));
        if constexpr(N < std::tuple_size_v<std::decay_t<T>> - 1) res << ", ";
        dump_tuple_impl<N + 1>(std::forward<T>(val), res);
    }
}


template<std::ranges::range R>
std::string dump_range_impl(R&& range, const Brackets& brcs = { "[", "]" }, const std::string& spl = ", ") {
    std::stringstream res;

    res << brcs.first << " ";

    auto itr = std::ranges::begin(range);
    auto end = std::ranges::end(std::forward<R>(range));

    while(itr != end) {
        if(std::ranges::next(itr) == end) res << dump(*itr) << " ";
        else res << dump(*itr) << spl;
        ++itr;
    }

    res << brcs.second ;

    return res.str();
}


std::string dump_debug_t(debug_t info) {
    return info;
}


struct dump_primitive_like {
    std::string operator()(std::nullptr_t) const {
        return COLOR_INIT;
    }

    template<lib::internal::pointer T>
    std::string operator()(const T ptr) const {
        return dump(*ptr);
    }

    template<lib::internal::derived_from_template<std::basic_string> T>
    std::string operator()(const T& val) const {
        std::stringstream res;
        res << COLOR_STRING << "`" << val << "`" << COLOR_INIT;
        return res.str();
    }

    std::string operator()(const char val) const {
        std::stringstream res;
        res << COLOR_STRING << "\'" << val << "\'" << COLOR_INIT;
        return res.str();
    }

    std::string operator()(const char val[]) const {
        std::stringstream res;
        res << COLOR_STRING << "\"" << val << "\"" <<  COLOR_INIT;
        return res.str();
    }

    std::string operator()(const unsigned char val) const {
        std::stringstream res;
        res << COLOR_NUMERIC << static_cast<int>(val) << COLOR_INIT;
        return res.str();
    }


    std::string operator()(const bool val) const {
        std::stringstream res;
        res << COLOR_NUMERIC << (val ? "true" : "false" ) << COLOR_INIT;
        return res.str();
    }


    template<lib::internal::arithmetic T>
    std::string operator()(const T val) const {
        std::stringstream res;
        res << COLOR_NUMERIC << std::setprecision(std::numeric_limits<T>::digits10);
        res << val << COLOR_LITERAL_OPERATOR << lib::internal::literal_operator_v<T>;
        res << COLOR_INIT;
        return res.str();
    };

    template<lib::internal::derived_from_template<std::optional> T>
    std::string operator()(const T& val) const {
        if(val.has_value()) return dump(*val);
        return COLOR_TYPE + "<optional> invalid" + COLOR_INIT;
    }
};


struct dump_bitset {
    template<std::size_t N>
    std::string operator()(std::bitset<N>&& val) const {
        std::stringstream res;
        res << COLOR_NUMERIC << val.to_string() << COLOR_INIT;
        return res.str();
    }
};


struct dump_has_val {
    template<class T>
        requires requires (T val) { val.val(); }
    std::string operator()(const T& val) const {
        return COLOR_TYPE + "<...> " + dump(val.val());
    }
};


struct dump_iterator {
    template<std::input_or_output_iterator I>
    std::string operator()(I&& itr) const {
        return COLOR_TYPE + "<iterator> " + COLOR_INIT+ dump(*itr);
    }
};


struct dump_wrapper {
    template<lib::internal::derived_from_template<std::map> T>
    std::string operator()(const T& val) const {
        return dump_range_impl(val, Brackets(COLOR_TYPE + "<map>" + COLOR_INIT + " {", "}"));
    }

    template<lib::internal::derived_from_template<std::multimap> T>
    std::string operator()(const T& val) const {
        return dump_range_impl(val, Brackets(COLOR_TYPE + "<multimap>" + COLOR_INIT + " {", "}"));
    }

    template<lib::internal::derived_from_template<std::unordered_map> T>
    std::string operator()(const T& val) const {
        return dump_range_impl(val, Brackets(COLOR_TYPE + "<unordered_map>" + COLOR_INIT + " {", "}"));
    }

    template<lib::internal::derived_from_template<std::unordered_multimap> T>
    std::string operator()(const T& val) const {
        return dump_range_impl(val, Brackets(COLOR_TYPE + "<unordered_multimap>" + COLOR_INIT + " {", "}"));
    }

    template<lib::internal::derived_from_template<std::set> T>
    std::string operator()(const T& val) const {
        return dump_range_impl(val, Brackets(COLOR_TYPE + "<set>" + COLOR_INIT + " {", "}"));
    }

    template<lib::internal::derived_from_template<std::multiset> T>
    std::string operator()(const T& val) const {
        return dump_range_impl(val, Brackets(COLOR_TYPE + "<multiset>" + COLOR_INIT + " {", "}"));
    }

    template<lib::internal::derived_from_template<std::unordered_set> T>
    std::string operator()(const T& val) const {
        return dump_range_impl(val, Brackets(COLOR_TYPE + "<unordered_set>" + COLOR_INIT + " {", "}"));
    }

    template<lib::internal::derived_from_template<std::unordered_multiset> T>
    std::string operator()(const T& val) const {
        return dump_range_impl(val, Brackets(COLOR_TYPE + "<unordered_multiset>" + COLOR_INIT + " {", "}"));
    }

    template<lib::internal::derived_from_template<std::vector> T>
    std::string operator()(const T& val) const {
        return dump_range_impl(val, Brackets(COLOR_TYPE + "<vector>" + COLOR_INIT + " [", "]"));
    }

    template<lib::internal::derived_from_template<std::deque> T>
    std::string operator()(const T& val) const {
        return dump_range_impl(val, Brackets(COLOR_TYPE + "<deque>" + COLOR_INIT + " [", "]"));
    }


    template<lib::internal::derived_from_template<std::stack> T>
    std::string operator()(T val) const {
        std::vector<typename T::value_type> vec;
        vec.resize(val.size());

        while(!val.empty()) vec.emplace_back(val.top()), val.pop();
        std::ranges::reverse(vec);

        return dump_range_impl(vec, Brackets(COLOR_TYPE + "<stack>" + COLOR_INIT + " [", "]"));
    }

    template<lib::internal::derived_from_template<std::priority_queue> T>
    std::string operator()(T val) const {
        std::vector<typename T::value_type> vec;
        vec.resize(val.size());

        while(!val.empty()) vec.emplace_back(val.top()), val.pop();

        return dump_range_impl(vec, Brackets(COLOR_TYPE + "<priority_queue>" + COLOR_INIT + " [", "]"));
    }


    template<lib::internal::derived_from_template<std::pair> T>
    std::string operator()(const T& val) const {
        std::stringstream res;
        res << COLOR_TYPE << "<pair>" << COLOR_INIT << " ( ";
        res << dump(val.first);
        res << ", ";
        res << dump(val.second);
        res << " )";
        return res.str();
    }

    template<lib::internal::derived_from_template<std::tuple> T>
    std::string operator()(const T& val) const {
        std::stringstream res;
        res << COLOR_TYPE << "<tuple>" << COLOR_INIT << " ( ";
        dump_tuple_impl<0>(val, res);
        res << " )";
        return res.str();
    }
};


struct dump_range {
    template<std::ranges::range T>
    std::string operator()(T&& val) const {
        return dump_range_impl(val);
    }
};


struct dump_loggable {
    template<lib::internal::loggable T>
    std::string operator()(const T& val) const {
        auto res = _debug(val);

        if constexpr(std::same_as<decltype(res), debug_t>) {
            return res;
        }
        else {
            return dump(res);
        }
    }
};


template<class T>
std::string dump(T&& val) {
    if constexpr(std::same_as<T, debug_t>) {
        // return "debug_t";
        return dump_debug_t(std::forward<T>(val));
    }

    if constexpr(std::invocable<dump_primitive_like, T>) {
        // return "primitive";
        return dump_primitive_like{}(std::forward<T>(val));
    }
    if constexpr(std::invocable<dump_loggable, T>) {
        // return "loggable";
        return dump_loggable{}(std::forward<T>(val));
    }
    if constexpr(std::invocable<dump_has_val, T>) {
        // return "has val";
        return dump_has_val{}(std::forward<T>(val));
    }

    if constexpr(std::invocable<dump_bitset, T>) {
        // return "bitset";
        return dump_bitset{}(std::forward<T>(val));
    }
    if constexpr(std::invocable<dump_iterator, T>) {
        // return "iterator";
        return dump_iterator{}(std::forward<T>(val));
    }

    if constexpr(std::invocable<dump_wrapper, T>) {
        // return "wrapper";
        return dump_wrapper{}(std::forward<T>(val));
    }


    if constexpr(std::invocable<dump_range, T>) {;
        // return "range
        return dump_range{}(std::forward<T>(val));
    }

    assert(false);
}


template<class T = std::nullptr_t> void debug(T&& = nullptr, const std::string& = "\n");

template<class T> void debug(T&& val, const std::string& endl) {
    *cdebug << dump(val) << endl << std::flush;
}


std::vector<std::string> split(const std::string& str) {
    constexpr char SEPARATOR = ',';
    constexpr char ESCAPE = '\\';
    constexpr char QUOTATIONS[] = "\"\'";
    constexpr char PARENTHESES[] = "()[]{}<>";
    constexpr auto PARENTHESES_KINDS = std::char_traits<char>::length(PARENTHESES);
    static_assert(PARENTHESES_KINDS % 2 == 0);

    std::vector<std::string> res = { "" };

    bool quoted = false;
    std::array<int,(PARENTHESES_KINDS / 2)> enclosed = { 0 };

    for(auto itr = std::ranges::begin(str); itr != std::ranges::end(str); ++itr) {
        if(std::ranges::find(QUOTATIONS, *itr) != std::ranges::end(QUOTATIONS)) {
            if(itr == std::ranges::begin(str) or *std::ranges::prev(itr) != ESCAPE) {
                quoted ^= true;
            }
        }

        if(const auto found = std::ranges::find(PARENTHESES, *itr); found != std::ranges::end(PARENTHESES)) {
            if(not quoted) {
                auto& target = enclosed[std::ranges::distance(std::begin(PARENTHESES), found) / 2];
                target = std::max(0, target - static_cast<int>((std::ranges::distance(std::begin(PARENTHESES), found) % 2) * 2) + 1);

            }
        }

        if(
            not quoted
            and static_cast<std::size_t>(std::ranges::count(enclosed, 0)) == std::ranges::size(enclosed)
            and *itr == SEPARATOR
        ) {
            res.push_back("");
        }
        else {
            res.back() += *itr;
        }
    }

    return res;
}

template<class Arg> void raw(std::nullptr_t, Arg&& arg) { *cdebug << std::forward<Arg>(arg) << std::flush; }
template<class Arg> void raw(Arg&& arg) { *cdebug << dump(std::forward<Arg>(arg)) << std::flush; }

void debug(
    std::vector<std::string> __attribute__ ((unused)) args,
    __attribute__ ((unused)) size_t idx,
    __attribute__ ((unused)) int LINE_NUM
) { debug(nullptr, COLOR_INIT + "\n"); }

template<typename Head, typename... Tail> void debug(std::vector<std::string> args, size_t idx, int LINE_NUM, Head&& H, Tail&&... T) {
    if(idx > 0) debug(nullptr, ","); else debug(nullptr, "\033[3;35m#" + std::to_string(LINE_NUM) + "  " + COLOR_INIT);
    debug(nullptr, "\033[32m" + args[idx]  + COLOR_INIT + ": ");
    debug(std::forward<Head>(H), std::string());
    debug(args, idx + 1, 0, std::forward<Tail>(T)...);
}

} // namespace debugger
