The goal of this task is to implement a ring of multivariate polynomials. You are required to implement the following class template:

  template <typename T, std::size_t N = 0> class poly;

An object of this class represents a univariate polynomial   a₀ + a₁x + a₂x² + … + aₙ₋₁xⁿ⁻¹, where the coefficients aᵢ are of type T. The number N is called the size of the polynomial (to avoid saying degree plus one). The type T should be a ring, meaning that binary operations +, -, * and the unary - are defined on it, and the default constructor should yield the value corresponding to zero.

To represent multivariate polynomials, the type T should again be a polynomial, meaning that it is of type poly. In general, to represent a polynomial in m variables we use the poly class recursively down to a depth of m, and the final type T is no longer of type poly, for example:

  poly<poly<poly<int, 3>, 2>, 4>

represents a polynomial in three variables over the type int. A polynomial of m variables is thought of as a function of the variables x₁, …, xₘ in the following way. The outermost definition is a polynomial in the variable x₁, whose coefficients are polynomials in the variables from x₂ to xₘ. The coefficient type is a polynomial in the variable x₂ with coefficients that are polynomials in the variables from x₃ to xₘ, and so on.

Constructors

You need to implement the following constructors for the poly class.
1. The default constructor, which creates a polynomial identically equal to zero.
2. The copy and move constructors (single-argument versions), whose argument is respectively of type const poly<U, M>& or poly<U, M>&&, where M ≤ N, and the type U is convertible to type T.
3. The converting constructor (single-argument), with an argument of a type convertible to T, that creates a polynomial of size 1.
4. The variadic constructor (two or more arguments), which creates a polynomial with coefficients equal to the subsequent argument values. The number of arguments should be no greater than the size of the polynomial N, and the type of each argument should be an r-reference to a type convertible to T. The use of “perfect forwarding” is required, see std::forward.
You should refer to the std::is_convertible template and the std::convertible_to concept.

The above constructors do not allow the creation of a polynomial of size one (i.e. a constant polynomial) whose single coefficient is itself a polynomial. Therefore, you must implement a function const_poly, whose argument is a polynomial p (an object of type poly) and that returns a polynomial of size one, whose single coefficient is p.

Additionally, you should write the appropriate deduction guides so that it is possible to create poly objects without specifying template arguments (see the example).

Assignment Operators

You must implement copy and move assignment operators, whose argument is respectively of type const poly<U, M>& or poly<U, M>&&, where M ≤ N, and the type U is convertible to type T.

Arithmetic Operators

You need to implement the operators +=, -=, *=. The argument for the += and -= operators may be a polynomial; in that case, its type is const poly<U, M>&, where M ≤ N, and the type U is convertible to type T. The argument for all three operators may also be a constant reference to a value of a type convertible to T.

You must implement the operators +, -, * in their binary versions, and the unary operator -. In the binary version, three usage variants should be possible by enforcing the appropriate concept:

  1. Only the left argument is a polynomial.   2. Only the right argument is a polynomial.   3. Both arguments are polynomials.

The resulting type should be the smallest type that always accommodates the resulting polynomial and can be deduced at compile time. In the case of addition or subtraction, the size of the result with respect to a given variable should be the maximum of the sizes of the arguments. In the case of multiplication of polynomials of sizes N and M with respect to a given variable, the size of the result should be N + M - 1 when N and M are nonzero, and zero when either of the sizes is zero.

You must implement a specialization of the std::common_type template and the helper type std::common_type_t. If we denote the coefficient types of the polynomials by U and V, the coefficient type of the resulting polynomial should be std::common_type_t<U, V>.

Indexing Operator

You need to implement the operator[](std::size_t i) in both non-const and const versions, returning a reference to the coefficient value aᵢ of the polynomial.

Method at

You need to implement two versions of the at method, which evaluates the polynomial.

  1. The first version of the at method applies arguments to the polynomial as a function of several variables. The number of arguments k may be different from the number of variables n. This is because a polynomial of n variables can be treated as a polynomial of k variables, where the coefficients are polynomials of size max(n − k, 0). Thus, k = 0 or k > n is allowed. In the first case, the result is the original polynomial. In the second case, the extra arguments are ignored, since the variables xᵢ for i > n do not occur in the polynomial.

  2. The second version of the at method has an argument of type const std::array<U, K>&. Then, if the contents of the argument are {v₁, …, vₖ}, the call is equivalent to at(v₁, …, vₖ).

The arguments of at can be of any types for which it is possible to evaluate the polynomial. In particular, they can also be polynomials.

Method size

You must implement the size method, which returns the size of the polynomial.

Function cross

You must implement a binary function cross that performs the multiplication of polynomials according to the formula:

  cross(p, q)(x₁, …, xₙ, y₁, …, yₘ) = p(x₁, …, xₙ) · q(y₁, …, yₘ),

where p and q are polynomials in n and m variables respectively, and the resulting polynomial has n + m variables.

