#pragma once

#include <iterator>
#include <vector>
#include <utility>
#include <functional>
#include <valarray>

#include "snippet/iterations.hpp"
#include "internal/exception.hpp"

namespace lib {

namespace internal {

namespace interval_scanner_lib {

template<class T> using interval = std::pair<T, T>;
template<class T> using intervals = std::vector<std::pair<T, T>>;

template<class T> struct base {
  protected:
    std::function<bool(T)> validate;

  public:
    base(std::function<bool(T)> validate) : validate(validate) {}

    void scan(T,T,T) {
        static_assert(EXCEPTION<T>, "not implemented: scan()");
    }

    void split(const T first, const T last, intervals<T> *intervals) const {
        std::valarray<bool> valid(false, last - first);
        for(auto itr=first,index=0; itr!=last; ++itr, ++index) valid[index] = validate(itr);

        auto can_begin = [&](const T itr) {
            const auto index = itr - first;
            if(itr == first) return valid[index];
            if(itr == last) return false;
            if(not valid[index-1] and valid[index]) return true;
            return false;
        };

        auto is_end = [&](const T itr) {
            const  auto index = itr - first;
            if(itr == first) return false;
            if(itr == last) return valid[index-1];
            if(valid[index-1] and not valid[index]) return true;
            return false;
        };

        {
            intervals->clear();
            T start = first;
            for(auto itr=first; ; ++itr) {
                if(can_begin(itr)) start = itr;
                if(is_end(itr)) intervals->emplace_back(start, itr);
                if(itr == last) break;
            }
        }
    }

    void scan_all(const T first, const T last) const {
        intervals<T> targets;
        this->split(first, last, &targets);
        ITR(start, end, targets) this->scan(first, start, end);
    }
};

} // namespace interval_scanner_lib

} // namespace internal

template<class T>
struct exclusive_interval_scanner : internal::interval_scanner_lib::base<T> {
  protected:
    std::function<void(T)> init;
    std::function<bool(T)> can_expand;
    std::function<void(T)> expand, contract;
    std::function<void(T, T)> apply;

  public:
    using interval = internal::interval_scanner_lib::interval<T>;
    using intervals = internal::interval_scanner_lib::intervals<T>;

    exclusive_interval_scanner(
        std::function<bool(T)> validate,
        std::function<void(T)> init,
        std::function<bool(T)> can_expand,
        std::function<void(T)> expand,
        std::function<void(T)> contract,
        std::function<void(T, T)> apply
    ) : internal::interval_scanner_lib::base<T>(validate), init(init), can_expand(can_expand), expand(expand), contract(contract), apply(apply) {}

    template<const bool FOLLOWING = true>
    void scan(const T start, const T end) const {
        T l_itr=start, r_itr=start;
        while(l_itr < end) {
            if (FOLLOWING and r_itr <= l_itr) {
                r_itr = l_itr+1;
                init(l_itr);
            }
            while(r_itr < end && can_expand(r_itr)) {
                expand(r_itr++);
            }
            apply(l_itr, r_itr);
            contract(l_itr++);
        }
    };

    template<const bool FOLLOWING = true>
    void scan_all(const T first, const T last) const {
        intervals targets;
        this->split(first, last, &targets);
        ITR(start, end, targets) this->scan<FOLLOWING>(start, end);
    }
};

template<class T>
struct inclusive_interval_scanner : internal::interval_scanner_lib::base<T> {
  protected:
    std::function<void()> init;
    std::function<bool()> valid;
    std::function<void(T)> expand, contract;
    std::function<void(T, T)> apply;

  public:
    using interval = internal::interval_scanner_lib::interval<T>;
    using intervals = internal::interval_scanner_lib::intervals<T>;

    inclusive_interval_scanner(
        std::function<bool(T)> validate,
        std::function<void()> init,
        std::function<bool()> valid,
        std::function<void(T)> expand,
        std::function<void(T)> contract,
        std::function<void(T, T)> apply
    ) : internal::interval_scanner_lib::base<T>(validate), init(init), valid(valid), expand(expand), contract(contract), apply(apply) {}

    template<const bool INVERSE = false, const bool FOLLOWING = true, const bool CONFIRMATION = true>
    void scan(const T start, const T end) const {
        T l_itr = start, r_itr = start;
        init();
        while(l_itr < end) {
            if(FOLLOWING and r_itr < l_itr) {
                r_itr = l_itr;
                init();
            }
            if(r_itr < end and (INVERSE ^ valid())) {
                expand(r_itr++);
            }
            else {
                contract(l_itr++);
            }
            if(!CONFIRMATION or valid()) apply(l_itr, r_itr);
        }
    }

    template<const bool INVERSE = false, const bool FOLLOWING = true, const bool CONFIRMATION = true>
    void scan_all(const T first, const T last) const {
        intervals targets;
        this->split(first, last, &targets);
        ITR(start, end, targets) this->scan<INVERSE,FOLLOWING,CONFIRMATION>(start, end);
    }
};

} // namespace lib
