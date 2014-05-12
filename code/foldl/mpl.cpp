#include <boost/mpl/fold.hpp>
#include <boost/mpl/list.hpp>


struct f {
    template <typename, typename>
    struct apply { struct type; };
};


template <int> struct x;
<% xs = (1..n).map { |i| "x<#{i}>" }.join(', ') %>

struct state;

using xs = boost::mpl::list<
    <%= xs %>
>;

using go = boost::mpl::fold<xs, state, f>::type;