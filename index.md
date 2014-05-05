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
Note:
example + benchmarks for each

====================

## Logical operations

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
