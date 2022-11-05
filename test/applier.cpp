#include "template.hpp"
#include "utility/applier.hpp"

signed main() {
    Max<int> maximum(10);
    std::cout << maximum.val() << std::endl;
    maximum <<= 13;
    std::cout << maximum.val() << std::endl;
    maximum <<= 5;
    std::cout << maximum.val() << std::endl;
    maximum <<= -1204;
    std::cout << maximum.val() << std::endl;
    maximum <<= 124092890;
    std::cout << maximum.val() << std::endl;

    return 0;
}
