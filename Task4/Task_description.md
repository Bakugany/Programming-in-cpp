# Polynomials

## Introduction

In this assignment, students are expected to learn and practice:

- Templates and their specialization as well as partial specialization;
- Techniques for manipulating types and data at compile time;
- The use of `constexpr` and the increasing prevalence of `constexpr` in the standard library (e.g., `std::max` since C++14);
- Techniques for handling templates with a variable number of arguments;
- Standard library facilities that assist with the above, such as `<type_traits>`;
- The idea of perfect forwarding (using `&&` and `std::forward`);
- Certain types and functions used in metaprogramming, e.g., `std::conditional`, `std::enable_if`;
- The mechanism of class template argument deduction (since C++17), along with the definition of custom deduction guides;
- Basic concepts on C++ concepts;
- Overloading of functions, operators, and constructors.

## Task

The aim of this assignment is to implement support for the ring of multivariate polynomials. You are required to implement the following class template:

```cpp
template <typename T, std::size_t N = 0> class poly;
```

An object of this class represents a univariate polynomial of the form:
  
  a₀ + a₁x + a₂x² + … + aₙ₋₁xⁿ⁻¹

where the coefficients aᵢ are of type T. The number N is called the *size* of the polynomial (i.e., degree plus one). The type T should represent a ring, meaning that the binary operators `+`, `-`, `*` and the unary operator `-` must be defined for it, and its default constructor should yield the element corresponding to zero.

To represent multivariate polynomials, T itself should be a polynomial type. In general, to represent a polynomial in *m* variables, the `poly` class is used recursively up to a depth of *m*, where the final type T is not a `poly`. For example:

```cpp
poly< poly< poly<int, 3>, 2>, 4>
```

represents a polynomial in three variables over the type `int`. A polynomial in *m* variables is thought of as a function in the variables x₁, …, xₘ in the following way: the outermost definition is a polynomial in x₁ with coefficients that are polynomials in the variables x₂ to xₘ; the coefficient type is a polynomial in x₂ with coefficients that are polynomials in x₃ to xₘ, and so on.

## Constructors

You must implement the following constructors for the `poly` class:

1. **Default Constructor:**  
   Creates a polynomial that is identically zero.

2. **Copy/Move Constructor (Single-Argument):**  
   Implement copy and move constructors taking an argument of type `const poly<U, M>&` or `poly<U, M>&&`, respectively, where M ≤ N and type U is convertible to type T.

3. **Converting Constructor (Single-Argument):**  
   A constructor taking an argument of a type convertible to T should create a polynomial of size 1.

4. **Multi-Argument Constructor (Two or More Arguments):**  
   This constructor creates a polynomial whose coefficients are initialized to the values of the successive arguments. The number of arguments should not exceed the polynomial's size N, and each argument should be an rvalue reference of a type convertible to T, utilizing perfect forwarding (see `std::forward`).

*Note:* You should make use of `std::is_convertible` and the concept `std::convertible_to` where appropriate.

Since the above constructors do not allow the creation of a polynomial of size one (i.e., a constant) whose only coefficient is itself a polynomial, you must also implement a function `const_poly`, whose argument is a polynomial `p` (an object of type `poly`) and which returns a polynomial of size one whose solitary coefficient is `p`.

Furthermore, provide appropriate deduction guides to allow creation of `poly` objects without specifying the template parameters explicitly.

## Assignment Operators

Implement copy and move assignment operators that take arguments of type `const poly<U, M>&` and `poly<U, M>&&`, respectively, where M ≤ N and U is convertible to T.

## Arithmetic Operators

Implement the arithmetic operators `+=`, `-=`, and `*=` as member functions. For operators `+=` and `-=`, the argument may be a polynomial (of type `const poly<U, M>&`, M ≤ N and U convertible to T) or a constant reference to a value of some type convertible to T.

Also implement the binary operators `+`, `-`, and `*`, as well as the unary minus operator. For the binary operators, three versions should be possible by using appropriate concepts:

- Only the left operand is a polynomial.
- Only the right operand is a polynomial.
- Both operands are polynomials.

The type of the result should be the smallest type that can contain the resulting polynomial and should be deducible at compile time. In the case of addition or subtraction, for a given variable, the result's size should be the maximum of the sizes of the operands. In the case of multiplication of polynomials with sizes N and M (with respect to a given variable), the result's size should be N + M - 1 if both N and M are nonzero; if either size is zero, then the result is zero.

Implement a specialization of the template `std::common_type` and a helper type alias `std::common_type_t` so that, if the coefficient types of the polynomials are U and V, the coefficient type of the resulting polynomial is `std::common_type_t<U, V>`.

## Indexing Operator

Implement the indexing operator `operator[](std::size_t i)` in both non-const and const versions, returning a reference to the coefficient aᵢ of the polynomial.

## Method `at`

Implement two versions of the method `at` for evaluating the polynomial:

1. **Variadic Version:**  
   Applies the arguments to the polynomial as if it were a function of several variables. The number of arguments *k* may differ from the number of variables *n*. This is because a polynomial in *n* variables can be treated as a polynomial in *k* variables, where the coefficients are polynomials of size `max(n − k, 0)`. Thus, *k* may be 0 or greater than *n*. In the first case, the result is the original polynomial. In the second case, the extra arguments are ignored, since the variables xᵢ for i > n do not appear in the polynomial.

2. **`std::array` Version:**  
   A version of `at` that accepts an argument of type `const std::array<U, K>&`. If the array contains `{v₁, …, vₖ}`, then the call is equivalent to invoking `at(v₁, …, vₖ)`.

Arguments to `at` can be of any types for which it is possible to evaluate the polynomial. In particular, they may also be polynomials.

## Method `size`

Implement a method `size` that returns the size of the polynomial.

## Function `cross`

Implement a two-argument function `cross` that performs the multiplication of polynomials according to the formula:

  cross(p, q)(x₁, …, xₙ, y₁, …, yₘ) = p(x₁, …, xₙ) * q(y₁, …, yₘ),

where *p* and *q* are polynomials in *n* and *m* variables respectively, and the resulting polynomial has *n + m* variables.

## Additional Requirements

- All constructors, methods, and functions you define must be marked with `constexpr`, so that they can be used at compile time.
- You may assume that the coefficient type T, if it is not itself a `poly`, provides:
  - A default constructor that creates the zero element,
  - Copy and move constructors,
  - An assignment operator,
  - The operators `+=`, `-=`, `*=`,
  - The binary operators `+`, `-`, `*`, and
  - The unary operator `-`.
- Any helper definitions should be hidden from the global scope (for example, inside a namespace named `detail`).

This concludes the specification for the Polynomials module.
