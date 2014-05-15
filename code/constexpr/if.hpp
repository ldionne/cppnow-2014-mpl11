/*!
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef CEXPR_IF_HPP
#define CEXPR_IF_HPP

#include <constexpr/integral.hpp>


namespace cexpr {
    template <typename Then, typename Else>
    constexpr auto if_(true_, Then t, Else)
    { return t(0); }

    template <typename Then, typename Else>
    constexpr auto if_(false_, Then, Else e)
    { return e(0); }
} // end namespace cexpr

#endif // !CEXPR_IF_HPP
