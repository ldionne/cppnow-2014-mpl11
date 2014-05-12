#include <constexpr/list.hpp>

#include <constexpr/integral.hpp>
#include <constexpr/type.hpp>


using namespace cexpr;

static_assert(is_empty(list<>{}), "");
static_assert(!is_empty(list<int>{}), "");

static_assert(head(list<int_<1>>{}) == int_<1>{}, "");
static_assert(head(list<int_<1>, int_<2>>{}) == int_<1>{}, "");
static_assert(head(list<type<void>, int_<0>>{}) == type<void>{}, "");

static_assert(tail(list<void>{}) == list<>{}, "");
static_assert(tail(list<void, int_<1>>{}) == list<int_<1>>{}, "");
static_assert(tail(list<void, int_<1>, int_<2>>{}) == list<int_<1>, int_<2>>{}, "");

static_assert(list<>{} == list<>{}, "");
static_assert(list<void>{} != list<>{}, "");
static_assert(list<>{} != list<void>{}, "");
static_assert(list<type<void>>{} == list<type<void>>{}, "");
static_assert(list<type<void>, int>{} != list<type<void>>{}, "");
static_assert(list<int_<1>, int_<2>>{} == list<int_<1>, int_<2>>{}, "");

static_assert(drop(int_<0>{}, list<>{}) == list<>{}, "");
static_assert(drop(int_<1>{}, list<>{}) == list<>{}, "");
static_assert(drop(int_<2>{}, list<>{}) == list<>{}, "");
static_assert(drop(int_<0>{}, list<int_<1>>{}) == list<int_<1>>{}, "");
static_assert(drop(int_<1>{}, list<int_<1>>{}) == list<>{}, "");
static_assert(drop(int_<2>{}, list<int_<1>>{}) == list<>{}, "");
static_assert(drop(int_<0>{}, list<int_<1>, int_<2>>{}) == list<int_<1>, int_<2>>{}, "");
static_assert(drop(int_<1>{}, list<int_<1>, int_<2>>{}) == list<int_<2>>{}, "");
static_assert(drop(int_<2>{}, list<int_<1>, int_<2>>{}) == list<>{}, "");
static_assert(drop(int_<4>{}, list<int_<1>, int_<2>>{}) == list<>{}, "");


constexpr struct {
    template <typename X>
    constexpr auto operator()(X x) const
    { return x + int_<1>{}; }
} inc{};
static_assert(fmap(inc, list<>{}) == list<>{}, "");
static_assert(fmap(inc, list<int_<1>>{}) == list<int_<2>>{}, "");
static_assert(fmap(inc, list<int_<1>, int_<2>>{}) == list<int_<2>, int_<3>>{}, "");


constexpr struct {
    template <typename X, typename Y>
    constexpr auto operator()(X x, Y y) const
    { return x + y; }
} plus{};
template <typename ...Xs>
constexpr auto suml(Xs ...)
{ return foldl(plus, int_<0>{}, list<Xs...>{}); }
static_assert(suml() == int_<0>{}, "");
static_assert(suml(int_<1>{}) == int_<1>{}, "");
static_assert(suml(int_<1>{}, int_<2>{}) == int_<3>{}, "");
static_assert(suml(int_<1>{}, int_<2>{}, int_<3>{}) == int_<6>{}, "");


int main() { }
