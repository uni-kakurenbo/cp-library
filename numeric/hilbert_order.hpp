#pragma once

#include <utility>

template<class T> T hilbert_order(const T n, T i, T j) {
    T p, q, d = 0;
    for(T t=n>>1; t>0; t>>=1) {
        p = (i&t) > 0, q = (j&t) > 0;
        d += t * t * ((p * 3) ^ q);
        if(q > 0) continue;
        if(p > 0) {
            i = n - i - 1;
            j = n - j - 1;
        }
        std::swap(i, j);
    }
    return d;
}

template<class T> T hilbert_order(const T n,const std::pair<T,T> p) {
    return hilbert_order(n, p.first, p.second);
}
