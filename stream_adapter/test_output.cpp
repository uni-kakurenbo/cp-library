#include <bits/stdc++.h>
#include <atcoder/modint>

#include <debug>

#define debug(...) Debug::debug(Debug::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
#define DEBUG(...) do { Debug::DEBUG(nullptr, "\033[3;35m#" + to_string(__LINE__) + "\033[m  "); Debug::DEBUG(__VA_ARGS__); Debug::DEBUG(nullptr, "\033[m\n"); } while(0);

using namespace std;
using namespace atcoder;

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

#include <output>
using namespace std;

Output print(&cout, Output<>::sendl);

signed main() {

    print << "aa";

    print(0);

    print(modint(10));

    print(1, 2, 3, 4);
    print({1, 2, 3, 4});

    print('a', "abcdefg", string("abcdefg"));

    print(1, 1U, 1L, 1UL, 1LL, 1ULL, 1.5, 1.0L);

    vector<int> vec_1d = { 1, 2, 3, 4 };
    print(vec_1d);
    print(vec_1d.begin(), vec_1d.end());

    deque<int> deq_1d = { 1, 2, 3, 4 };
    print(deq_1d);

    set<int> set_1d = { 1, 2, 3, 4 };
    print(set_1d);

    unordered_set<int> unord_set_1d = { 1, 2, 3, 4 };
    print(unord_set_1d);


    vector vector_string = { "string", "abcdefg" };
    print(vector_string);

    print(pair<int,int>{10,24});

    Primes primes(100);
    print(primes);

    return 0;
}
