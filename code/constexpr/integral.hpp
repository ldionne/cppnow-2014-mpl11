/*!
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef CEXPR_INTEGRAL_HPP
#define CEXPR_INTEGRAL_HPP

namespace cexpr {
    template <typename T, T t>
    struct integral {
        constexpr operator T() const { return t; }

        constexpr auto operator!() const {
            return integral<decltype(!t), !t>{};
        }
    };

#define CEXPR_INTEGRAL_BINOP(op)                                            \
    template <typename U, U u, typename V, V v>                             \
    constexpr auto operator op(integral<U, u>, integral<V, v>) {            \
        return integral<decltype(u op v), u op v>{};                        \
    }                                                                       \
/**/
    CEXPR_INTEGRAL_BINOP(+)
    CEXPR_INTEGRAL_BINOP(-)
    CEXPR_INTEGRAL_BINOP(*)
    CEXPR_INTEGRAL_BINOP(/)
    CEXPR_INTEGRAL_BINOP(==)
    CEXPR_INTEGRAL_BINOP(!=)
    CEXPR_INTEGRAL_BINOP(||)
    CEXPR_INTEGRAL_BINOP(&&)
#undef CEXPR_INTEGRAL_BINOP

    template <int i>
    using int_ = integral<int, i>;

    template <bool b>
    using bool_ = integral<bool, b>;
    using true_ = bool_<true>;
    using false_ = bool_<false>;
} // end namespace cexpr

#endif // !CEXPR_INTEGRAL_HPP
