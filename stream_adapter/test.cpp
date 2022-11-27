#include <bits/stdc++.h>
#include <atcoder/modint>

#include <debug>

#define debug(...) Debug::debug(Debug::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
#define DEBUG(...) do { Debug::DEBUG(nullptr, "\033[3;35m#" + to_string(__LINE__) + "\033[m  "); Debug::DEBUG(__VA_ARGS__); Debug::DEBUG(nullptr, "\033[m\n"); } while(0);

using namespace std;
using namespace atcoder;

#include <input>
#include <output>
using namespace std;

Input _input;
Output _print(&cout, Output<>::sendl);
Output _info(&cout, "");

#define input _input
#define print _print
#define info _info

signed main() {

    info("S1 S2: ");
    string s1, s2;
    input >> s1;
    input(&s2);

    print(s1, s2);

    info("Size: ");
    int sz = input.one<int>();

    info("Vector 1D: ");
    vector<int> vec_1d(sz); input >> vec_1d;
    print(vec_1d);

    info("Deque 1D: ");
    deque<int> deq_1d(sz); input(deq_1d.begin(), deq_1d.end());
    print(deq_1d.begin(), deq_1d.end());
    print(deq_1d.begin(), prev(deq_1d.end()));
    print();

    info("pair: ");
    pair<int,int> pp; input(&pp);
    print(pp);

    print("--END--");

    return 0;
}
