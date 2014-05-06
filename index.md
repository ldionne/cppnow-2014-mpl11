## Template metaprogramming in C++1y
### Louis Dionne, C++Now 2014

====================

## Outline
- __What__ is metaprogramming in C++?
- C++1y areas of improvement over C++03
- My proposal

====================

## __What__ is metaprogramming in C++?
Note:
- State the scope of an eventual MPL reimplementation.
- Also, observe lack of the word template.
----
## What is the purpose of the MPL?
----
## of Fusion?

====================

## C++1y areas of improvement over C++03
Note: provide example + benchmarks for each

====================

## Logical operations
(without short-circuiting)

----

Naive
```cpp
template <typename ...xs>
struct and_
    : std::true_type
{ };

template <typename x, typename ...xs>
struct and_<x, xs...>
    : std::conditional_t<x::value, and_<xs...>, x>
{ };
```

----

With `noexcept`
```cpp
void allow_expansion(...) noexcept;

template <bool condition>
struct noexcept_if { noexcept_if() noexcept(condition) { } };

template <typename ...xs>
using and_ = std::integral_constant<bool,
    noexcept(allow_expansion(noexcept_if<xs::value>{}...))
>;
```

----

With `constexpr`
```cpp
template <std::size_t N>
constexpr bool and_impl(const bool (&array)[N]) {
    for (bool elem: array)
        if (!elem)
            return false;
    return true;
}

template <typename ...xs>
using and_ = std::integral_constant<bool,
    and_impl<sizeof...(xs) + 1>({(bool)xs::value..., true})
>;
```

----

With overload resolution
```cpp
template <typename ...T> std::true_type  pointers_only(T*...);
template <typename ...T> std::false_type pointers_only(T...);
                         std::true_type  pointers_only();

template <typename ...xs>
using and_ = decltype(pointers_only(
    std::conditional_t<xs::value, int*, int>{}...
));
```

----

With partial specialization
```cpp
template <typename ...>
struct and_impl
    : std::false_type
{ };

template <typename ...T>
struct and_impl<std::integral_constant<T, true>...>
    : std::true_type
{ };

template <typename ...xs>
using and_ = and_impl<std::integral_constant<bool, xs::value>...>;
```

----

With `std::is_same`
```cpp
template <bool ...> struct bool_seq;

template <typename ...xs>
using and_ = std::is_same<
    bool_seq<xs::value...>,
    bool_seq<(xs::value, true)...>
>;
```

----

TODO: Show the benchmarks (we should also show the naive implementation)

====================

## Metafunction mapping

====================

## Associative sequences

====================

## Sequence indexing

====================

## Constexpr folding of homogeneous data

====================

## Universal template template parameters
(or farewell `quoteN`)

====================

## Variadic sequences

====================

## Error messages
(still room for improvement)
Note: Show the hell of faux variadics

====================

## Obscure implementation
(yes that's a problem)
Note:
- A kitten dies each time a user sees the internals
- People can't contribute
- You're left with an unmaintainable mess

====================

## Preprocessing and parsing time
Note: Here I mean the time penalty just for including the library.

====================

## At this point, it should be clear that we need a new MPL. What would it look like?

==============================================================================

# Considered solutions
Note:
This only sketches the "core" of the solution. With almost any of these
solutions it would be possible to:
- Use a typeclass-based dispatching mechanism
- Have Datatypes or something equivalent
- Interoperate easily with other libraries

====================

## Faithful reimplementation of MPL
Note: (see old commits and talk about small variations e.g. on dispatching)

====================

## Homogeneous constexpr
(just to clarify ideas and see why this is not a solution)

====================

## Heterogeneous constexpr
(sweet, perhaps for Fusion?)

====================

## Lazy metafunctions + no iterators

====================

## Other libraries
(and why I'm not satisfied)

----
lib1
----
...
----
libN

==============================================================================

# Selected design

====================

## Laziness
Note: dismiss cynics about performance

====================

## Typeclasses

====================

## Datatypes

====================

## Methods

<!-- Is there more Haskell stuff? -->

====================

## Rewrite rules (?)

====================

## Interoperability with other libraries
Note: Talk about the relation to typeclasses, easy ad-hoc polymorphism etc..

====================

## what I don't like about my proposal
----
boxed types
----
no parameterized data types (perhaps that's possible though)

====================

## Roadmap
----
GSOC
----
Boost
----
Standard C++ (?)

====================

# Thank you

<span class="fragment fade-in" data-fragment-index="1">
http://ldionne.com <br>
http://github.com/ldionne
</span>
