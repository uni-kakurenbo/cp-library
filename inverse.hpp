#include <iterator>
#include <vector>
#include <unordered_map>

template<class I, class T = typename std::iterator_traits<I>::value_type, class V = std::vector<int>, class C = std::unordered_map<T,V>>
struct Inverse : C {
    Inverse(I first, I last) {
        for(auto itr=first; itr!=last; ++itr) (*this)[*itr].push_back(std::distance(first,itr));
    }
};
