#include <constexpr/type.hpp>


using namespace cexpr;

static_assert(type<int>{} == type<int>{}, "");
static_assert(type<int>{} != type<void>{}, "");


int main() { }
