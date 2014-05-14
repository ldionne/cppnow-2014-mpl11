<%
    xs = (1..n).map { |i| "x<#{i}>" }
    list = MPL::List.new(xs)
%>

#include <boost/mpl/fold.hpp>
<%= list.includes %>


struct f {
    template <typename, typename>
    struct apply { struct type; };
};


template <int> struct x;

struct state;

using xs = <%= list %>;

using go = boost::mpl::fold<xs, state, f>::type;