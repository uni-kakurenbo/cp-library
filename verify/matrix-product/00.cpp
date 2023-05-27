#include <atcoder/modint>

#include "snippet/fast_io.hpp"
#include "adapter/input.hpp"
#include "adapter/output.hpp"
#include "numeric/matrix.hpp"

input_adapter input;
output_adapter print;

signed main() {
    int n, m, k; std::cin >> n >> m >> k;
    lib::matrix<atcoder::modint998244353> a(n, m), b(m, k);
    input >> a >> b;
    print(a*b);
}
