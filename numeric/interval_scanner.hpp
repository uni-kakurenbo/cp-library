#pragma once

#include <iterator>
#include <vector>
#include <utility>
#include <functional>
#include <valarray>

#include "template.hpp"
#include "internal/exception.hpp"

namespace Internal {

namespace IntervalScannerLib {

template<class T> using Interval = std::pair<T, T>;
template<class T> using Intervals = std::vector<std::pair<T, T>>;

template<class T> struct IntervalScannerBase {
  protected:
    std::function<bool(T)> validate;

  public:
    IntervalScannerBase(std::function<bool(T)> validate) : validate(validate) {}

    void scan(T,T,T) {
        static_assert(EXCEPTION<T>, "not implemented: scan()");
    }

    void split(const T first, const T last, Intervals<T> *intervals) const {
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
        Intervals<T> targets;
        this->split(first, last, &targets);
        ITRM(start, end, targets) this->scan(first, start, end);
    }
};

} // namespace IntervalScannerLib

} // namespace Internal

template<class T>
struct ExclusiveIntervalScanner : Internal::IntervalScannerLib::IntervalScannerBase<T> {
  protected:
    std::function<void(T)> init;
    std::function<bool(T)> can_expand;
    std::function<void(T)> expand, contract;
    std::function<void(T, T)> apply;

  public:
    using Interval = Internal::IntervalScannerLib::Interval<T>;
    using Intervals = Internal::IntervalScannerLib::Intervals<T>;

    ExclusiveIntervalScanner(
        std::function<bool(T)> validate,
        std::function<void(T)> init,
        std::function<bool(T)> can_expand,
        std::function<void(T)> expand,
        std::function<void(T)> contract,
        std::function<void(T, T)> apply
    ) : Internal::IntervalScannerLib::IntervalScannerBase<T>(validate), init(init), can_expand(can_expand), expand(expand), contract(contract), apply(apply) {}

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
        Intervals targets;
        this->split(first, last, &targets);
        ITRM(start, end, targets) this->scan<FOLLOWING>(start, end);
    }
};

template<class T>
struct InclusiveIntervalScanner : Internal::IntervalScannerLib::IntervalScannerBase<T> {
  protected:
    std::function<void()> init;
    std::function<bool()> valid;
    std::function<void(T)> expand, contract;
    std::function<void(T, T)> apply;

  public:
    using Interval = Internal::IntervalScannerLib::Interval<T>;
    using Intervals = Internal::IntervalScannerLib::Intervals<T>;

    InclusiveIntervalScanner(
        std::function<bool(T)> validate,
        std::function<void()> init,
        std::function<bool()> valid,
        std::function<void(T)> expand,
        std::function<void(T)> contract,
        std::function<void(T, T)> apply
    ) : Internal::IntervalScannerLib::IntervalScannerBase<T>(validate), init(init), valid(valid), expand(expand), contract(contract), apply(apply) {}

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
        Intervals targets;
        this->split(first, last, &targets);
        ITRM(start, end, targets) this->scan<INVERSE,FOLLOWING,CONFIRMATION>(start, end);
    }
};
