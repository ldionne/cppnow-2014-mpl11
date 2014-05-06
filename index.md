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

Note: Explain that in the examples, we're not using lifted metafunctions for
simplicity, but their utility will be explained. They are useful beyond the
lack of universal template template parameters in c++03: we can't parse
lambda expressions if they don't contain type template parameters only
(IS THAT TRUE?).

TODO: Mark what's available in C++03, C++11 and C++1y

TODO: Explain that for C++03 we're still using variadic templates for the
non-core part of what's demonstrated. Otherwise, the slides would sometimes
contain several pages long of preprocessor mess.

====================

## Logical operations
(without short-circuiting)

<!-- TEST CODE
using true_ = std::true_type;
using false_ = std::false_type;
static_assert(and_<>::value, "");
static_assert(!and_<false_>::value, "");
static_assert(and_<true_>::value, "");
static_assert(!and_<true_, false_>::value, "");
static_assert(and_<true_, true_>::value, "");
static_assert(!and_<true_, true_, true_, true_, true_, false_>::value, "");
-->

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

TODO: Show the benchmarks

====================

## Metafunction mapping

<!-- TEST CODE
template <typename x> struct f { struct type; };
static_assert(std::is_same<map<f, std::tuple<>>::type, std::tuple<>>::value, "");
static_assert(std::is_same<map<f, std::tuple<int>>::type, std::tuple<f<int>::type>>::value, "");
static_assert(std::is_same<map<f, std::tuple<int, void>>::type, std::tuple<f<int>::type, f<void>::type>>::value, "");
-->

----

Naive
```cpp

```
TODO

----

"Vectorized" metafunction application
```cpp
template <template <typename ...> class f, typename xs>
struct map;

template <template <typename ...> class f, typename ...xs>
struct map<f, std::tuple<xs...>> {
    using type = std::tuple<typename f<xs>::type...>;
};
```

----

TODO: Show the benchmarks

====================

## Key-based lookup

<!-- TEST CODE
template <int> struct k;
template <int> struct v;
static_assert(std::is_same<at_key<k<0>, pair<k<0>, v<0>>>::type, v<0>>::value, "");
static_assert(std::is_same<at_key<k<0>, pair<k<0>, v<0>>, pair<k<1>, v<1>>>::type, v<0>>::value, "");
static_assert(std::is_same<at_key<k<1>, pair<k<0>, v<0>>, pair<k<1>, v<1>>>::type, v<1>>::value, "");
-->

----

In the following slides...
```cpp
template <typename x>
struct no_decay { using type = x; };

template <typename ...xs>
struct inherit : xs... { };

template <typename key, typename value>
struct pair { };
```

----

With single inheritance
```cpp
template <typename ...pairs>
struct map {
    template <typename fail = void>
    static typename fail::key_not_found at_key(...);
};

template <typename key, typename value, typename ...pairs>
struct map<pair<key, value>, pairs...> : map<pairs...> {
    using map<pairs...>::at_key;
    static no_decay<value> at_key(no_decay<key>*);
};

template <typename key, typename ...pairs>
using at_key = decltype(
    map<pairs...>::at_key((no_decay<key>*)nullptr)
);
```

----

With multiple inheritance
```cpp
template <typename key, typename value>
static no_decay<value> lookup(pair<key, value>*);

template <typename key, typename ...pairs>
using at_key = decltype(lookup<key>((inherit<pairs...>*)nullptr));
```

----

TODO: Show benchmarks
TODO: There are probably other techniques

====================

## Index-based lookup

<!-- TEST CODE
template <int> struct x;
static_assert(std::is_same<at<0, x<0>>::type, x<0>>::value, "");
static_assert(std::is_same<at<0, x<0>, x<1>>::type, x<0>>::value, "");
static_assert(std::is_same<at<1, x<0>, x<1>>::type, x<1>>::value, "");
static_assert(std::is_same<at<2, x<0>, x<1>, x<2>>::type, x<2>>::value, "");
-->

----

In the following slides...
```cpp
template <typename x>
struct no_decay { using type = x; };

template <std::size_t index, typename value>
struct index_pair { };
```

----

Naive
```cpp
template <std::size_t index, typename x, typename ...xs>
struct at
    : at<index - 1, xs...>
{ };

template <typename x, typename ...xs>
struct at<0, x, xs...> {
    using type = x;
};
```

----

Using multiple inheritance
```cpp
template <std::size_t index, typename value>
no_decay<value> lookup(index_pair<index, value>*);

template <typename indices, typename ...xs>
struct index_map;

template <std::size_t ...indices, typename ...xs>
struct index_map<std::index_sequence<indices...>, xs...>
    : index_pair<indices, xs>...
{ };

template <std::size_t index, typename ...xs>
using at = decltype(lookup<index>(
    (index_map<std::index_sequence_for<xs...>, xs...>*)nullptr
));
```

----

Using overload resolution
```cpp
template <typename ignore>
struct lookup;

template <std::size_t ...ignore>
struct lookup<std::index_sequence<ignore...>> {
    template <typename nth>
    static no_decay<nth>
    apply(decltype(ignore, (void*)nullptr)..., no_decay<nth>*, ...);
};

template <std::size_t index, typename ...xs>
using at = decltype(
    lookup<std::make_index_sequence<index>>::apply(
        (no_decay<xs>*)nullptr...
    )
);
```

----

TODO: Show the benchmarks
TODO: There's another trick by Richard Smith that ought to be tried


====================

## Folding

TODO: We study left folds here, but are right folds similar w.r.t.
C++1y improvement?

Note: Once you win the folds, you win (almost) everything. Explain this.

----

Naive
```cpp
template <template <typename ...> class f, typename state, typename xs,
          bool = is_empty<xs>::value>
struct foldl {
    using type = state;
};

template <template <typename ...> class f, typename state, typename xs>
struct foldl<f, state, xs, false>
    : foldl<
        f,
        typename f<state, typename head<xs>::type>::type,
        typename tail<xs>::type
    >
{ };
```

----

Using variadic templates
```cpp
template <template <typename ...> class f, typename state, typename ...xs>
struct foldl {
    TODO: FINISH THIS
};
```

----

Bonus for homogeneous data: `constexpr`
```cpp
template <typename F, typename State, typename T, std::size_t N>
constexpr State foldl_impl(F f, State s, std::array<T, N> const& xs) {
    for (std::size_t i = 0; i < xs.size(); ++i)
        s = f(s, xs[i]);
    return s;
}

template <template <typename ...> class f, typename state, typename ...xs>
struct foldl_homogeneous {
    TODO: FINISH THIS
    TODO: will it work on arbitrary homogeneous data, or only integrals?
};
```

Note: We can't use a range-based for loop here because `std::array` does not
provide `constexpr` iterators.

----

TODO: Expose benchmarks

====================

## Universal template template params
(or farewell `quoteN`)

----

C++03
```cpp
template <template <typename> class f>
struct quote1 { ... };

template <template <typename, typename> class f>
struct quote2 { ... };

...

template <template <typename, typename, ..., typename> class f>
struct quoteN { ... };
```

----

C++11
```cpp
template <template <typename ...> class f>
struct quote { ... };
```

----

TODO: Show usage example and how the code is made simpler.

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
