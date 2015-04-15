/*!
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef CEXPR_TYPE_HPP
#define CEXPR_TYPE_HPP

#include <constexpr/integral.hpp>


namespace cexpr {
    template <typename T>
    struct type { };

    template <typename T, typename U>
    constexpr auto operator==(type<T>, type<U>)
    { return false_{}; }

    template <typename T>
    constexpr auto operator==(type<T>, type<T>)
    { return true_{}; }

    template <typename T, typename U>
    constexpr auto operator!=(type<T> t, type<U> u)
    { return !(t == u); }
} // end namespace cexpr

#endif // !CEXPR_TYPE_HPP
