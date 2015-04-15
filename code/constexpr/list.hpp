/*!
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef CEXPR_LIST_HPP
#define CEXPR_LIST_HPP

#include <constexpr/integral.hpp>


namespace cexpr {
    template <typename ...xs>
    struct list { };

    template <typename x, typename ...xs>
    constexpr auto head(list<x, xs...>)
    { return x{}; }

    template <typename x, typename ...xs>
    constexpr auto tail(list<x, xs...>)
    { return list<xs...>{}; }

    template <typename ...xs>
    constexpr auto is_empty(list<xs...>)
    { return bool_<sizeof...(xs) == 0>{}; }



    template <typename N, typename Xs>
    constexpr auto drop_impl(N, Xs xs, true_)
    { return xs; }

    template <typename N, typename Xs>
    constexpr auto drop(N n, Xs xs)
    { return drop_impl(n, xs, n == int_<0>{} || is_empty(xs)); }

    template <typename N, typename Xs>
    constexpr auto drop_impl(N n, Xs xs, false_)
    { return drop(n - int_<1>{}, tail(xs)); }



    template <typename ...xs, typename ...ys>
    constexpr auto operator==(list<xs...> x, list<ys...> y)
    { return head(x) == head(y) && tail(x) == tail(y); }

    template <typename ...xs>
    constexpr auto operator==(list<xs...>, list<>)
    { return false_{}; }

    template <typename ...ys>
    constexpr auto operator==(list<>, list<ys...>)
    { return false_{}; }

    constexpr auto operator==(list<>, list<>)
    { return true_{}; }

    template <typename ...xs, typename ...ys>
    constexpr auto operator!=(list<xs...> x, list<ys...> y)
    { return !(x == y); }



    template <typename F, typename ...xs>
    constexpr auto fmap(F f, list<xs...>)
    { return list<decltype(f(xs{}))...>{}; }



    template <typename F, typename State, typename X, typename ...Xs>
    constexpr auto foldl(F f, State s, list<X, Xs...> xs)
    { return foldl(f, f(s, head(xs)), tail(xs)); }

    template <typename F, typename State>
    constexpr auto foldl(F, State s, list<>)
    { return s; }
} // end namespace cexpr

#endif // !CEXPR_LIST_HPP
