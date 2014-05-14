<%
    xs = (1..n).map { |i| "x<#{i}>" }
    vector = MPL::Vector.new(xs)
%>

#include <boost/mpl/fold.hpp>
<%= vector.includes %>


struct f {
    template <typename, typename>
    struct apply { struct type; };
};


template <int> struct x;

struct state;

using xs = <%= vector %>;

using go = boost::mpl::fold<xs, state, f>::type;