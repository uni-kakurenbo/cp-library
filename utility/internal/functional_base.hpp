#pragma once


namespace uni {


template<class P>
    requires
        requires(P p) {
            p.first;
            p.second;
        }
inline P swapped(P& pair) {
    return P{ pair.second, pair.first };
}


} // namespace uni
