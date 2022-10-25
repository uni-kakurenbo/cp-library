#include <bits/stdc++.h>
#include <atcoder/modint>

#include <debug>

#define debug(...) Debug::debug(Debug::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
#define DEBUG(...) do { Debug::DEBUG(nullptr, "\033[3;35m#" + to_string(__LINE__) + "\033[m  "); Debug::DEBUG(__VA_ARGS__); Debug::DEBUG(nullptr, "\033[m\n"); } while(0);

using namespace std;
using namespace atcoder;

template <class T = int> struct Matrix : vector<vector<T>> {
    Matrix(size_t h, size_t w, T init = T()) : vector<vector<T>> (h, vector<T> (w, init)) {}
    inline T& operator()(size_t i, size_t j) { return (*this)[i][j]; }
};

template<class T = int, class C = vector<T>> struct Primes : C {
    Primes(size_t max) : is_prime(max+1, true) {
        is_prime[0] = is_prime[1] = false;
        for(size_t p=2; p<=max; p++) if(is_prime[p]) {
            for(size_t i=p*p; i<=max; i+=p) is_prime[i] = false;
            this->emplace_back(p);
        }
    }
    bool operator()(size_t index) {
        return is_prime[index];
    }
  private:
    vector<bool> is_prime;
};

template<class T> struct Iterator {
  const T* p;
  const T& operator*() { return *p; }
  Iterator& operator++() { ++p; return *this; }
  bool operator!=(const Iterator& v) { return p != v.p; }
  ptrdiff_t operator-(const Iterator& v) { return p - v.p; }
};
template<class T> ptrdiff_t distance(Iterator<T>& a, Iterator<T>& b) { return b - a; };

struct MyString {
    const char* s;
    Iterator<char> begin() { return { s }; }
    Iterator<char> end()   { return { s + strlen(s) }; }
};

struct MyStructure0 {
    MyStructure0(int a, int b, int c, int d) : a(a), b(b), c(c), d(d) {}
    vector<int> _debug() {
        return { 0, a + b + c + d, a * b * c * d };
    }
  private:
    int a, b, c, d;
};

struct MyStructure1 {
    MyStructure1(int a, int b, int c, int d) : a(a), b(b), c(c), d(d) {}
    friend vector<int> _debug(MyStructure1 &s);
  private:
    int a, b, c, d;
};

vector<int> _debug(MyStructure1 &s) {
    return { 1, s.a + s.b + s.c + s.d, s.a * s.b * s.c * s.d };
}

// template<unsigned long N> Debug::debug_t _debug(bitset<N> &bist) {
//     return bist.to_string();
// }

string _debug(bitset<10UL> bist)  {
    return bist.to_string();
}

signed main() {

    debug('a', "abcdefg", string("abcdefg"));

    debug(1, 1U, 1L, 1UL, 1LL, 1ULL, 1.0, 1.0L);

    vector<int> vec_1d = { 1, 2, 3, 4 };
    debug(vec_1d);

    vector<vector<int>> vec_2d = { { 1, 2, 3, 4 }, { 1, 3, 5 }, { 1, 4, 10 } };
    debug(vec_2d);

    Matrix<int> matrix1(10, 5, 0);
    debug(matrix1);

    Matrix matrix2(2, 3, 0.0L);
    debug(matrix2);

    vector vector_string = { "string", "abcdefg" };
    debug(vector_string);

    debug(Primes(100));

    debug(MyString{"abcdefg"});
    debug(string{string{"abcdefg"}});

    static_assert(!Debug::Internal::is_loggable_v<int>);
    static_assert(!Debug::Internal::is_loggable_v<vector<int>>);
    static_assert(Debug::Internal::is_loggable_v<MyStructure0>);
    static_assert(Debug::Internal::is_loggable_v<MyStructure1>);

    MyStructure0 my_structure0 = { 1, 2, 3, 4 };
    debug(my_structure0);

    MyStructure1 my_structure1 = { 1, 2, 3, 4 };
    debug(my_structure1);

    pair<int,MyStructure0> pair__my_structure0 = { 0, { 1, 2, 3, 4 } };
    debug(pair__my_structure0);

    pair<int,MyStructure1> pair__my_structure1 = { 0, { 1, 2, 3, 4 } };
    debug(pair__my_structure1);

    vector<pair<int,MyStructure0>> vector_pair__my_structure0 = { { 0, { 1, 2, 3, 4 } }, { 1, { 1, 2, 3, 4 } } };
    debug(vector_pair__my_structure0);

    vector<pair<int,MyStructure1>> vector_pair__my_structure1 = { { 0, { 1, 2, 3, 4 } }, { 1, { 1, 2, 3, 4 } } };
    debug(vector_pair__my_structure1);

    map<vector<string>,set<pair<int,tuple<string,char,unsigned>>>> comlexed = {
        { { "str0", "str1" }, { { 1, { "str2", 'c', 1124 } } } }
    };
    debug(comlexed);

    return 0;
}
