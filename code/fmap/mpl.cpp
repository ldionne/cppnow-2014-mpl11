<%
    xs = (1..n).map { |i| "x<#{i}>" }
    list = MPL::List.new(xs)
%>

#include <boost/mpl/transform.hpp>
<%= list.includes %>


struct f {
    template <typename>
    struct apply { struct type; };
};

template <int> struct x;


using xs = <%= list %>;

using ys = boost::mpl::transform<xs, f>::type;