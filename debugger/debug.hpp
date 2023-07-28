#pragma once


#include <iostream>
#include <limits>
#include <sstream>
#include <array>
#include <string>
#include <cstring>
#include <vector>
#include <bitset>
#include <deque>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <utility>
#include <iomanip>

#include <atcoder/modint>

#include "internal/type_traits.hpp"


namespace debugger {


template<class T> auto _debug (T &val) -> decltype(val._debug()) {
    return val._debug();
}


std::ostream *cdebug = &std::clog;

const std::string COLOR_INIT = "\033[m";
const std::string COLOR_STRING = "\033[33m";
const std::string COLOR_TYPE = "\033[34m";
const std::string COLOR_NUMERIC = "\033[36m";
const std::string COLOR_LITERAL_OPERATOR = "\033[31m";

template<class T> int depth(const T&) { return 0; }
template<class T> int depth(const std::vector<T>&) { return depth(T ()) + 1; }

using Brackets = std::pair<std::string, std::string>;

// template<class T, std::enable_if_t<lib::internal::is_iterable_v<T> && !lib::internal::is_template<std::map,T>::value>* = nullptr>
// std::string lit(T, Brackets = { "[", "]" }, std::string = ", ");
template<class T, std::enable_if_t<lib::internal::is_iterable_v<T> && !lib::internal::is_template<std::map,T>::value && !std::is_base_of_v<std::string,T>>* = nullptr>
std::string lit(const T&, Brackets = { "[", "]" }, std::string = ", ");

template<class T, std::enable_if_t<lib::internal::is_template<std::map,T>::value>* = nullptr>
std::string lit(const T&, Brackets = { "{", "}" }, std::string = ", ");

template<class I> std::string lit(const I, const I, const Brackets = { "[", "]" }, const std::string = ", ");

template<class F, class S> std::string lit(const std::pair<F, S>&);
template<class... T> std::string lit(const std::tuple<T...>&);

template<size_t N = 0, class T> void iterate_tuple(const T&, std::stringstream&);

template<class T, std::enable_if_t<lib::internal::is_loggable_v<T>>* = nullptr> std::string lit(const T&);

template<class T> std::string lit(const T*);


struct debug_t : std::string {
    using std::string::string;
    debug_t(const std::string &str) {
        this->assign(str);
    }
};
std::string lit(debug_t info) {
    return info;
}

std::string lit(std::nullptr_t) {
    return COLOR_INIT;
}

template<class S, std::enable_if_t<std::is_base_of_v<std::string,S> && !std::is_same_v<S,debug_t>>* = nullptr>
std::string lit(const S &val) {
    std::stringstream res;
    res << COLOR_STRING << "`" << val << "`" << COLOR_INIT;
    return res.str();
}
std::string lit(const char &val) {
    std::stringstream res;
    res << COLOR_STRING << "\'" << val << "\'" << COLOR_INIT;
    return res.str();
}
std::string lit(const char val[]) {
    std::stringstream res;
    res << COLOR_STRING << "\"" << val << "\"" <<  COLOR_INIT;
    return res.str();
}

std::string lit(const unsigned char &val) {
    std::stringstream res;
    res << COLOR_NUMERIC << static_cast<int>(val) << COLOR_INIT;
    return res.str();
}
std::string lit(const bool &val) {
    std::stringstream res;
    res << COLOR_NUMERIC << (val ? "true" : "false" ) << COLOR_INIT;
    return res.str();
}

template<std::size_t N>
std::string lit(const std::bitset<N>& val) {
    std::stringstream res;
    res << COLOR_NUMERIC << val.to_string() << COLOR_INIT;
    return res.str();
}

template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr> std::string lit(T val) {
    std::stringstream res;
    res << COLOR_NUMERIC << std::fixed << std::setprecision(std::numeric_limits<T>::digits10);// << scientific;
    res << val << COLOR_LITERAL_OPERATOR << lib::internal::literal_operator_v<T>;
    res << COLOR_INIT;
    return res.str();
};

template<class T> auto lit(const T& val) -> decltype(val.val(), std::string());

template<class T, atcoder::internal::is_modint_t<T>*>
std::string _lit(int, const T& val) {
    return COLOR_TYPE + "<modint> " + lit(val.val());
}

template<class T> std::string _lit(bool, const T& val) {
    return COLOR_TYPE + "<...> " + lit(val.val());
}


template<class T> auto lit(const T& val) -> decltype(val.val(), std::string()) {
    return _lit(0, val);
}

template<class K, class V> std::string lit(const std::map<K, V>& mp) {
    return lit(mp, Brackets(COLOR_TYPE + "<map>" + COLOR_INIT + " {", "}"));
}
template<class K, class V> std::string lit(const std::unordered_map<K, V>& mp) {
    return lit(mp, Brackets(COLOR_TYPE + "<unordered_map>" + COLOR_INIT + " {", "}"));
}
template<class T> std::string lit(const std::set<T>& st) {
    return lit(st, Brackets(COLOR_TYPE + "<set>" + COLOR_INIT + " {", "}"));
}
template<class T> std::string lit(const std::unordered_set<T>& st) {
    return lit(st, Brackets(COLOR_TYPE + "<unordered_set>" + COLOR_INIT + " {", "}"));
}
template<class T> std::string lit(const std::vector<T>& vec) {
    return lit(vec, Brackets(COLOR_TYPE + "<vector>" + COLOR_INIT + " [", "]"));
}
template<class T> std::string lit(const std::deque<T>& deq) {
    return lit(deq, Brackets(COLOR_TYPE + "<deque>" + COLOR_INIT + " [", "]"));
}

template<class F, class S> std::string lit(const std::pair<F,S>& pr) {
    std::stringstream res;
    res << COLOR_TYPE << "<pair>" << COLOR_INIT << " ( ";
    res << lit(pr.first);
    res << ", ";
    res << lit(pr.second);
    res << " )";
    return res.str();
}
template<class... T> std::string lit(const std::tuple<T...>& tpl) {
    std::stringstream res;
    res << COLOR_TYPE << "<tuple>" << COLOR_INIT << " ( ";
    iterate_tuple(tpl, res);
    res << " )";
    return res.str();
}

template<size_t N, class T> void iterate_tuple(__attribute__ ((unused)) const T& val, std::stringstream &res) {
    if constexpr(N < std::tuple_size_v<T>) {
        res << lit(std::get<N>(val));
        if constexpr(N < std::tuple_size_v<T> - 1) res << ", ";
        iterate_tuple<N + 1>(val, res);
    }
}

// template<class T, std::enable_if_t<lib::internal::is_iterable_v<T> && !lib::internal::is_template<std::map,T>::value>*>
// std::string lit(T val, Brackets brcs, std::string sep) {
//     return lit(lib::internal::iterator_resolver<T>::begin(val), lib::internal::iterator_resolver<T>::end(val), brcs, sep);
// }
template<class T, std::enable_if_t<lib::internal::is_iterable_v<T> && !lib::internal::is_template<std::map,T>::value && !std::is_base_of_v<std::string,T>>*>
std::string lit(const T& val, Brackets brcs, std::string sep) {
    return lit(lib::internal::iterator_resolver<T>::begin(val), lib::internal::iterator_resolver<T>::end(val), brcs, sep);
}

template<class T, std::enable_if_t<lib::internal::is_template<std::map,T>::value>*>
std::string lit(const T& val, Brackets brcs, std::string sep) {
    return lit(val.begin(), val.end(), brcs, sep);
}

template<class T, std::enable_if_t<lib::internal::is_loggable_v<T>>*>
std::string lit(const T &val) {
    const auto& res = _debug(val);
    if constexpr(std::is_same_v<decltype(res),debug_t>) {
        return res;
    } else {
        return lit(res);
    }
}

template<class T> std::string lit(const T *val) {
    return lit(*val);
}

template<class I, class = typename std::iterator_traits<I>::iterator_category>
std::string lit(const I& itr) {
    return COLOR_TYPE + "<iterator> " + COLOR_INIT+ lit(*itr);
}

template<class I> std::string lit(const I first, const I last, const Brackets brcs, const std::string spl) {
    std::stringstream res;
    res << brcs.first << " ";
    auto itr = first;
    while(itr != last) {
        if(std::next(itr) == last) res << lit(*itr) << " ";
        else res << lit(*itr) << spl;
        ++itr;
    }
    res << brcs.second ;
    return res.str();
}

template<class T = std::nullptr_t> void debug(T&& = nullptr, std::string = "\n");

template<class T> void debug(T&& val, std::string endl) {
    *cdebug << lit(std::forward<T>(val)) << endl << std::flush;
}


std::vector<std::string> split(const std::string& str) {
    constexpr char SEPARATOR = ',';
    constexpr char ESCAPE = '\\';
    constexpr char QUOTATIONS[] = "\"\'";
    constexpr char PARENTHESES[] = "()[]{}<>";
    static_assert(std::strlen(PARENTHESES) % 2 == 0);

    std::vector<std::string> res = { "" };

    bool quoted = false;
    std::array<int,(std::strlen(PARENTHESES) / 2)> enclosed = { 0 };

    for(auto itr = std::begin(str); itr != std::end(str); ++itr) {
        if(std::find(std::begin(QUOTATIONS), std::end(QUOTATIONS), *itr) != std::end(QUOTATIONS)) {
            if(itr == std::begin(str) or *std::prev(itr) != ESCAPE) {
                quoted ^= true;
            }
        }

        if(const auto found = std::find(std::begin(PARENTHESES), std::end(PARENTHESES), *itr); found != std::end(PARENTHESES)) {
            if(not quoted) {
                auto& target = enclosed[std::distance(std::begin(PARENTHESES), found) / 2];
                target = std::max(0, target - static_cast<int>((std::distance(std::begin(PARENTHESES), found) % 2) * 2) + 1);

            }
        }

        if(
            not quoted
            and static_cast<std::size_t>(std::count(std::begin(enclosed), std::end(enclosed), 0)) == std::size(enclosed)
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

template<class Arg> void DEBUG(std::nullptr_t, Arg&& arg) { *cdebug << std::forward<Arg>(arg) << std::flush; }
template<class... Args> void DEBUG(Args&&... args) { *cdebug << lit(std::forward<Args>(args)...) << std::flush; }

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

} // namespace debug
