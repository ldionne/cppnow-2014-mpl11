#include <constexpr/integral.hpp>


using namespace cexpr;

static_assert(int_<0>{} == int_<0>{}, "");
static_assert(int_<1>{} != int_<0>{}, "");

static_assert(int_<1>{} + int_<2>{} == int_<3>{}, "");
static_assert(int_<1>{} - int_<2>{} == int_<-1>{}, "");

static_assert(int_<3>{} * int_<2>{} == int_<6>{}, "");
static_assert(int_<6>{} / int_<3>{} == int_<2>{}, "");

static_assert(int_<3>{} || int_<0>{}, "");
static_assert(int_<3>{} && int_<1>{}, "");

static_assert(!int_<0>{}, "");
static_assert(!!int_<3>{}, "");


int main() { }
