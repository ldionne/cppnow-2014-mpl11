#include <constexpr/list.hpp>


constexpr struct {
    template <typename ...>
    struct result { };

    template <typename X, typename Y>
    constexpr auto operator()(X, Y) const
    { return result<X, Y>{}; }
} f{};

template <int> struct x { };
<% xs = (1..n).map { |i| "x<#{i}>" }.join(', ') %>

constexpr struct { } state{};

constexpr auto xs = cexpr::list<
    <%= xs %>
>{};

constexpr auto go = cexpr::foldl(f, state, xs);