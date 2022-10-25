/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region tmplate */
#include <bits/stdc++.h>
using namespace std;

#ifdef LOCAL_JUDGE
    #include<debug>
    #define debug(...) Debug::debug(Debug::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
    #define DEBUG(...) do { Debug::DEBUG(nullptr, "\033[3;35m#" + to_string(__LINE__) + "\033[m  "); Debug::DEBUG(__VA_ARGS__); Debug::DEBUG(nullptr, "\033[m\n"); } while(0);
#else
    #define debug(...) { ; }
    #define DEBUG(...) { ; }
#endif

#define INF32 2147483647
#define INF64 9223372036854775807LL

#define until(...) while(!(__VA_ARGS__))

#define REP(i,n) for(int i=0, i##_length=int(n); i<i##_length; ++i)
#define REPD(i,n) for(int i=(n)-1; i>=0; --i)
#define LOOP(n) REP(_$, (n))
#define FOR(i,l,r) for(ll i=(l), i##_last=ll(r); i<=i##_last; ++i)
#define FORD(i,l,r) for(ll i=(l), i##_last=ll(r); i>=i##_last; --i)

#define ITRP(x,v) for(auto x : (v))
#define ITRR(x,v) for(auto &x : (v))
#define ITR(x,v) for(const auto &x : (v))
#define ITRMP(x,y,v) for(auto [x, y] : (v))
#define ITRMR(x,y,v) for(auto &[x, y] : (v))
#define ITRM(x,y,v) for(const auto [x, y] : (v))

#define ALL(x) begin((x)),end((x))
#define RALL(x) rbegin((x)),rend((x))

#define $F first
#define $S second

using ll = long long;
using ull = unsigned long long;
using ld = long double;

constexpr char ln = '\n';
constexpr char spc = ' ';

inline void fast_io() { ios::sync_with_stdio(false), cin.tie(nullptr); }

template<class T1, class T2> inline auto mod(T1 x, T2 r) { return (x%r+r)%r; }

template<class T> inline bool chmax(T &a, T b) { return (a<b ? a=b, true : false); }
template<class T> inline bool chmin(T &a, T b) { return (a>b ? a=b, true : false); }

template<class T> auto operator<<(ostream &out, const T &val) -> decltype(val.val(), out)& {
    return out << val.val();
}
/* #endregion */

template<class T, class Container = vector<T>, class CompA = less<T>, class CompB = greater<T>> struct KthElement {
  private:
    size_t K;
    priority_queue<T,Container,CompA> small;
    priority_queue<T,Container,CompB> large;

  public:
    KthElement(size_t K) : K(K) {}

    inline T get() const { return small.top(); }
    inline bool has() const { return small.size() == K; }

    inline void push(T v) {
        if(small.size() < K) {
            small.push(v);
            return;
        }
        T kth = small.top();
        if(v < kth) {
            small.pop(); small.push(v);
            large.push(kth);
        }
        else {
            large.push(v);
        }
    }

    inline void pop() {
        small.pop();
        if(large.empty()) return;
        T v = large.top(); large.pop();
        small.push(v);
    }
};

//https://yukicoder.me/submissions/808074

signed main() {
    ios::sync_with_stdio(false), cin.tie(nullptr);

    int q, k; cin >> q >> k;
    KthElement<ll> kth(k);
    LOOP(q) {
        int type; cin >> type;
        if(type == 1) {
            ll v; cin >> v;
            kth.push(v);
        }
        if(type == 2) {
            if(!kth.has()) {
                cout << -1 << ln;
                continue;
            }
            cout << kth.get() << ln;
            kth.pop();
        }
    }

    return 0;
}
