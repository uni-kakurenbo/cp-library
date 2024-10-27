#pragma once


#include <iostream>
#include <limits>
#include <iterator>
#include <string_view>
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
#include "internal/exception.hpp"

#include <typeinfo>
#include <cxxabi.h>


namespace debugger {


template<class T>
auto _debug (T&& val) -> decltype(val._debug()) {
    return val._debug();
}


std::ostream *cdebug = &std::clog;


#ifdef DEBUGGER_COLORED_OUTPUT

constexpr std::string COLOR_LINE = "\033[3;35m";
constexpr std::string COLOR_IDENTIFIER = "\033[32m";
constexpr std::string COLOR_INIT = "\033[m";
constexpr std::string COLOR_STRING = "\033[33m";
constexpr std::string COLOR_TYPE = "\033[34m";
constexpr std::string COLOR_NUMERIC = "\033[36m";
constexpr std::string COLOR_LITERAL_OPERATOR = "\033[31m";

#else

constexpr std::string COLOR_LINE = "";
constexpr std::string COLOR_IDENTIFIER = "";
constexpr std::string COLOR_INIT = "";
constexpr std::string COLOR_STRING = "";
constexpr std::string COLOR_TYPE = "";
constexpr std::string COLOR_NUMERIC = "";
constexpr std::string COLOR_LITERAL_OPERATOR = "";

#endif

using Brackets = std::pair<std::string, std::string>;


template<class T>
std::string dump(T&&);


template<class T>
const std::string get_type_name(T&& val) {
    const char* const name = typeid(std::forward<T>(val)).name();
    int status = -4;
    char* const demangled_name = abi::__cxa_demangle(name, NULL, NULL, &status);
    std::string res{name};
    if (status == 0) {
        res = std::string(demangled_name);
        free(demangled_name);
    }

    return COLOR_TYPE + res + COLOR_INIT;
}

struct debug_t : std::string {
    using std::string::string;
    debug_t(const std::string& str) {
        this->assign(str);
    }
};


template<size_t N, class T>
void dump_tuple_impl([[maybe_unused]] T&& val, std::stringstream &res) {
    if constexpr(N < std::tuple_size_v<std::remove_cvref_t<T>>) {
        res << dump(std::get<N>(val));
        if constexpr(N < std::tuple_size_v<std::remove_cvref_t<T>> - 1) res << ", ";
        dump_tuple_impl<N + 1>(std::forward<T>(val), res);
    }
}


template<std::ranges::input_range R>
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

    template<uni::internal::pointer T>
    std::string operator()(const T ptr) const {
        return dump(*ptr);
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::basic_string>
    std::string operator()(T&& val) const {
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


    template<uni::internal::arithmetic T>
    std::string operator()(const T val) const {
        std::stringstream res;
        res << std::setprecision(std::numeric_limits<T>::digits10) << val;

        auto str = res.str();

        std::string dst = "";
        while(str.length() > 3) {
            dst = ',' + str.substr(str.length() - 3, 3) + dst;
            str = str.substr(0, str.length() - 3);
        }

        return COLOR_NUMERIC + str + dst + COLOR_LITERAL_OPERATOR + uni::internal::literal_operator_v<T> + COLOR_INIT;
    };

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::optional>
    std::string operator()(T&& val) const {
        if(val.has_value()) return dump(*val);
        return COLOR_TYPE + "invalid" + COLOR_INIT;
    }
};


struct dump_bitset {
    template<std::size_t N>
    std::string operator()(const std::bitset<N>& val) const {
        std::stringstream res;
        res << COLOR_NUMERIC << val.to_string() << COLOR_INIT;
        return res.str();
    }
};


struct dump_has_val {
    template<class T>
        requires requires (T val) { val.val(); }
    std::string operator()(T&& val) const {
        return dump(val.val());
    }
};


struct dump_iterator {
    template<std::input_or_output_iterator I>
    std::string operator()(I&& itr) const {
        return COLOR_TYPE + "<iterator> " + COLOR_INIT+ dump(*itr);
    }
};


struct dump_wrapper {
    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::map>
    std::string operator()(T&& val) const {
        return dump_range_impl(val, Brackets("{", "}"));
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::multimap>
    std::string operator()(T&& val) const {
        return dump_range_impl(val, Brackets("{", "}"));
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::unordered_map>
    std::string operator()(T&& val) const {
        return dump_range_impl(val, Brackets("{", "}"));
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::unordered_multimap>
    std::string operator()(T&& val) const {
        return dump_range_impl(val, Brackets("{", "}"));
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::set>
    std::string operator()(T&& val) const {
        return dump_range_impl(val, Brackets("{", "}"));
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::multiset>
    std::string operator()(T&& val) const {
        return dump_range_impl(val, Brackets("{", "}"));
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::unordered_set>
    std::string operator()(T&& val) const {
        return dump_range_impl(val, Brackets("{", "}"));
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::unordered_multiset>
    std::string operator()(T&& val) const {
        return dump_range_impl(val, Brackets("{", "}"));
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::valarray>
    std::string operator()(T&& val) const {
        return dump_range_impl(val, Brackets("[", "]"));
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::vector>
    std::string operator()(T&& val) const {
        return dump_range_impl(val, Brackets("[", "]"));
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::deque>
    std::string operator()(T&& val) const {
        return dump_range_impl(val, Brackets("[", "]"));
    }


    template<uni::internal::derived_from_template<std::queue> T>
    std::string operator()(T val) const {
        std::vector<typename T::value_type> vec;

        while(!val.empty()) vec.emplace_back(val.front()), val.pop();

        return dump_range_impl(vec, Brackets("<", ">"));
    }

    template<uni::internal::derived_from_template<std::stack> T>
    std::string operator()(T val) const {
        std::vector<typename T::value_type> vec;

        while(!val.empty()) vec.emplace_back(val.top()), val.pop();
        std::ranges::reverse(vec);

        return dump_range_impl(vec, Brackets("<", ">"));
    }

    template<uni::internal::derived_from_template<std::priority_queue> T>
    std::string operator()(T val) const {
        std::vector<typename T::value_type> vec;

        while(!val.empty()) vec.emplace_back(val.top()), val.pop();

        return dump_range_impl(vec, Brackets("<", ">"));
    }


    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::pair>
    std::string operator()(T&& val) const {
        std::stringstream res;
        res << "( " << dump(val.first) << ", " << dump(val.second) << " )";
        return res.str();
    }

    template<class T>
        requires uni::internal::derived_from_template<std::remove_cvref_t<T>, std::tuple>
    std::string operator()(T&& val) const {
        std::stringstream res;
        res << "( ";
        dump_tuple_impl<0>(val, res);
        res << " )";
        return res.str();
    }
};


struct dump_range {
    template<std::ranges::input_range T>
    std::string operator()(T&& val) const {
        return dump_range_impl(val);
    }
};


struct dump_loggable {
    template<uni::internal::loggable T>
    std::string operator()(T&& val) const {
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
    if constexpr(std::same_as<std::remove_cvref_t<T>, debug_t>) {
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
        // return "range";
        return dump_range{}(std::forward<T>(val));
    }

    return "== dump error ==";
}


template<class T> void debug(T&& val, const std::string& endl) {
    *cdebug << dump(val) << endl << std::flush;
}


constexpr std::string_view WHITESPACES = " \n\r\t\f\v";

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACES);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACES);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

std::vector<std::string> split(const std::string& str) {
    static constexpr char SEPARATOR = ',';
    static constexpr char ESCAPE = '\\';
    static constexpr std::string_view QUOTATIONS = "\"\'";
    static constexpr std::string_view PARENTHESES = "()[]{}<>";
    static constexpr auto PARENTHESES_KINDS = std::ranges::size(PARENTHESES);
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

    for(auto&& v : res) v = trim(v);

    return res;
}

template<class Arg> void raw(std::nullptr_t, Arg&& arg) { *cdebug << std::forward<Arg>(arg) << std::flush; }
template<class Arg> void raw(Arg&& arg) { *cdebug << dump(std::forward<Arg>(arg)) << std::flush; }

void debug(const std::vector<std::string>, const size_t, const int, const std::string) { debug(nullptr, COLOR_INIT + "\n"); }


std::map<std::pair<std::string, int>, int> count;

template<class Head, class... Tail>
void debug(
    const std::vector<std::string> args, const size_t idx,
    const int line, const std::string path,
    Head&& H, Tail&&... T
) {
    if(idx == 0) {
        std::string file = path.substr(path.find_last_of("/") + 1);
        debug(nullptr, COLOR_LINE + file + " #" + std::to_string(line) + " (" + std::to_string(count[{ file, line }]++) + ")" + COLOR_INIT);
    }
    debug(nullptr, "\n - ");


    const std::string content = dump(H);
    const std::string type_name = get_type_name(std::forward<Head>(H));


    debug(nullptr, COLOR_IDENTIFIER + args[idx]  + COLOR_INIT + " : ");
    debug(nullptr, content);

    if(type_name.size() + content.size() >= 300) debug(nullptr, "\n   ");

    debug(nullptr, " " + type_name);

    debug(args, idx + 1, 0, path, std::forward<Tail>(T)...);
}


} // namespace debugger
