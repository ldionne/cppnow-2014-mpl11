#include <boost/mpl11/list.hpp>


struct f {
    using type = f;
    template <typename>
    struct apply { struct type; };
};

template <int> struct x { struct type; };
<% xs = (1..n).map { |i| "x<#{i}>" }.join(', ') %>

using xs = boost::mpl11::list<
    <%= xs %>
>;

using ys = boost::mpl11::fmap<f, xs>::type;