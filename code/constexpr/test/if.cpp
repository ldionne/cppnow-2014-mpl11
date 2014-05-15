#include <constexpr/if.hpp>
#include <constexpr/integral.hpp>

#include <cassert>


using namespace cexpr;


int main() {
    assert(if_(true_{},
        [](auto) { return true_{}; },
        [](auto) { return false_{}; }
    ));

    assert(if_(true_{},
        [](auto) { return true_{}; },
        [](auto x) { return (void)x, typename decltype(x)::this_would_fail{}; }
    ));
}
