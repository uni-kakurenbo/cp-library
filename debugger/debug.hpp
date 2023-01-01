#pragma once

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <utility>
#include <iomanip>

#include <atcoder/modint>

#include "./internal/type_traits.hpp"

namespace debugger {

std::ostream *cdebug = &std::clog;

const std::string COLOR_INIT = "\033[m";
const std::string COLOR_STRING = "\033[33m";
const std::string COLOR_TYPE = "\033[34m";
const std::string COLOR_NUMERIC = "\033[36m";
const std::string COLOR_LITERAL_OPERATOR = "\033[31m";

template<class T> int depth(const T&) { return 0; }
template<class T> int depth(const std::vector<T>&) { return depth(T ()) + 1; }

using Brackets = std::pair<std::string, std::string>;

// template<class T, std::enable_if_t<internal::is_iterable_v<T> && !internal::is_template<std::map,T>::value>* = nullptr>
// std::string lit(T, Brackets = { "[", "]" }, std::string = ", ");
template<class T, std::enable_if_t<internal::is_iterable_v<T> && !internal::is_template<std::map,T>::value>* = nullptr>
std::string lit(const T&, Brackets = { "[", "]" }, std::string = ", ");

template<class T, std::enable_if_t<internal::is_template<std::map,T>::value>* = nullptr>
std::string lit(const T&, Brackets = { "{", "}" }, std::string = ", ");

template<class I> std::string lit(I, I, Brackets = { "[", "]" }, std::string = ", ");

template<class F, class S> std::string lit(const std::pair<F, S>&);
template<class... T> std::string lit(const std::tuple<T...>&);

template<size_t N = 0, class T> void iterate_tuple(const T&, std::stringstream&);

template<class T, std::enable_if_t<internal::is_loggable_v<T>>* = nullptr> std::string lit(const T&);

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
std::string lit(const std::string &val) {
    std::stringstream res;
    res << COLOR_STRING << "`" << val << "`" << COLOR_INIT;
    return res.str();
}
std::string lit(const char &val) {
    std::stringstream res;
    res << COLOR_STRING << "\'", res << val, res << "\'" << COLOR_INIT;
    return res.str();
}
std::string lit(const char val[]) {
    std::stringstream res;
    res << COLOR_STRING << "\"", res << val, res << "\"" <<  COLOR_INIT;
    return res.str();
}

std::string lit(const bool &val) {
    std::stringstream res;
    res << COLOR_NUMERIC << (val ? "true" : "false" ) << COLOR_INIT;
    return res.str();
}

template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr> std::string lit(T val) {
    std::stringstream res;
    res << COLOR_NUMERIC << std::fixed << std::setprecision(std::numeric_limits<T>::digits10);// << scientific;
    res << val << COLOR_LITERAL_OPERATOR << internal::literal_operator_v<T>;
    res << COLOR_INIT;
    return res.str();
};


// template<class T> auto lit(const T&) -> decltype(val.val(), std::string());

template<class T> std::string _lit(bool, const T& val) {
    return COLOR_TYPE + "<...> " + lit(val.val());
}

template<class T, atcoder::internal::is_modint_t<T>*>
std::string _lit(int, const T& val) {
    return COLOR_TYPE + "<modint> " + lit(val.val());
}

template<class T> auto lit(const T& val) -> decltype(val.val(), std::string()){
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

// template<class T, std::enable_if_t<internal::is_iterable_v<T> && !internal::is_template<std::map,T>::value>*>
// std::string lit(T val, Brackets brcs, std::string sep) {
//     return lit(internal::iterator_resolver<T>::begin(val), internal::iterator_resolver<T>::end(val), brcs, sep);
// }
template<class T, std::enable_if_t<internal::is_iterable_v<T> && !internal::is_template<std::map,T>::value>*>
std::string lit(const T& val, Brackets brcs, std::string sep) {
    return lit(internal::iterator_resolver<T>::begin(val), internal::iterator_resolver<T>::end(val), brcs, sep);
}

template<class T, std::enable_if_t<internal::is_template<std::map,T>::value>*>
std::string lit(const T& val, Brackets brcs, std::string sep) {
    return lit(val.begin(), val.end(), brcs, sep);
}

template<class T, std::enable_if_t<internal::is_loggable_v<T>>*>
std::string lit(const T &val) {
    auto res = _debug(val);
    if constexpr(std::is_same_v<decltype(res),debug_t>) {
        return res;
    } else {
        return lit(res);
    }
}

template<class T> std::string lit(const T *val) {
    return lit(*val);
}

template<class I> std::string lit(I first, I last, Brackets brcs, std::string spl) {
    std::stringstream res;
    res << brcs.first << " ";
    while(first != last) {
        if(std::distance(first, last)<=1) res << lit(*first) << " ";
        else res << lit(*first) << spl;
        ++first;
    }
    res << brcs.second ;
    return res.str();
}

template<class T = std::nullptr_t> void debug(T&& = nullptr, std::string = "\n");

template<class T> void debug(T&& val, std::string endl) {
    *cdebug << lit(std::forward<T>(val)) << endl << std::flush;
}


std::vector<std::string> split(std::string str) {
    str += ',';
    std::vector<std::string> res;
    while(!str.empty()) {
        res.push_back(str.substr(0, str.find(',')));
        str = str.substr(str.find(',') + 1);
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
