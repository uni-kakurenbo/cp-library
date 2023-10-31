#include <bits/stdc++.h>
#include <atcoder/modint>

#include <debug>

#define debug(...) debugger::debug(debugger::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
#define DEBUG(...) do { debugger::DEBUG(nullptr, "\033[3;35m#" + to_string(__LINE__) + "\033[m  "); debugger::DEBUG(__VA_ARGS__); debugger::DEBUG(nullptr, "\033[m\n"); } while(0);

using namespace std;
using namespace atcoder;

#include "adapter/input.hpp"
#include "adapter/output.hpp"
using namespace std;

lib::input_adapter _input;
lib::output_adapter _print(&cout, lib::output_adapter<>::sendl);
lib::output_adapter _info(&cout, "");

#define input _input
#define print _print
#define info _info

signed main() {

    info("S1 S2: ");
    string s1, s2;
    input >> s1;
    input(&s2);

    print(s1, s2);

    info("size_t: ");
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
