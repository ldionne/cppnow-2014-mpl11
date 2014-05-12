#include <constexpr/list.hpp>


template <typename X> struct f_ { };

constexpr struct {
    template <typename X>
    constexpr auto operator()(X) const
    { return f_<X>{}; }
} f{};

template <int> struct x { };
<% xs = (1..n).map { |i| "x<#{i}>" }.join(', ') %>

constexpr auto xs = cexpr::list<
    <%= xs %>
>{};

constexpr auto ys = cexpr::fmap(f, xs);