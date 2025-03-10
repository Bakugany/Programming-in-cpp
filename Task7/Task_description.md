# Functional Lists

## Introduction

Students should learn:

- The syntax of lambda expressions.
- The type `std::function`.
- The basics of functional programming using C++.

## Task

The aim of this assignment is to implement operations on lists that implicitly store a sequence of elements `[x₀, …, xₙ₋₁]`. We require that the type `L` of an object representing such a list provides only a template method with the following signature:

```cpp
template <typename F, typename A>
A operator()(F f, A a);
```

Here, the type `F` must provide a template method with the signature:

```cpp
template <typename X>
A operator()(X x, A a);
```

The only way to operate on a list `l` is to execute it as a function by calling `l(f, a)` with an appropriate function `f` and what is called an accumulator `a`. For objects `l`, `f`, `a` of classes `L`, `F`, `A` respectively, and for objects `x₀, x₁, …, xₙ₋₁` of classes `X₀, X₁, …, Xₙ₋₁`, the following equality holds:

  l(f, a) = f(x₀, f(x₁, …, f(xₙ₋₁, a)…)),

where the function calls to `f` on the right-hand side are the corresponding specializations of the operator() for type `F`.

## Operations as Objects

All operations mentioned here should be defined as constant objects with an appropriately overloaded template method `operator()`, so that they can be passed as function arguments if needed. The operations are:

- **empty**  
  `auto empty`  
  A constant function representing the empty list.

- **cons**  
  `auto cons(auto x, auto l)`  
  A function that returns the list `l` with the element `x` added at its front.

- **create**  
  `auto create(auto...)`  
  A function that returns a list consisting of the given arguments.

- **of_range**  
  `auto of_range(auto r)`  
  A function that returns a list created from the elements of `r`. It can be assumed that `r` is of a type satisfying the concept `std::ranges::bidirectional_range`, possibly wrapped in a `std::reference_wrapper`.

- **concat**  
  `auto concat(auto l, auto k)`  
  A function that returns a list produced by concatenating the lists `l` and `k`.

- **rev**  
  `auto rev(auto l)`  
  A function that returns the list `l` with its elements in reverse order.

- **map**  
  `auto map(auto m, auto l)`  
  A function that returns a list produced from `l` in such a way that every element `x` is transformed to `m(x)`.

- **filter**  
  `auto filter(auto p, auto l)`  
  A function that returns a list derived from `l` by retaining only those elements `x` that satisfy the predicate `p(x)`.

- **flatten**  
  `auto flatten(auto l)`  
  A function that returns a list obtained by concatenating all the lists stored within the list `l`.

- **as_string**  
  `std::string as_string(const auto& l)`  
  A function that returns a representation of the list `l` as an `std::string`, assuming that for every element `x` in the list the insertion operator (`<<`) works with an output stream; see usage examples.

All of the above objects should be defined within the namespace `flist`.

## Assumptions

- For all operations that create lists (i.e., all except `as_string`), the arguments are passed by value, which means making full copies. The user of the library may use `std::ref` to wrap an argument if they wish to avoid copying.
- When executing the list `l` via the call `l(f, a)`, it is assumed that the function `f` returns an object of the same type as `a`. If it does not, the behavior of the call `l(f, a)` is undefined.
