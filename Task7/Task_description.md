The goal of the assignment is to implement operations on lists that implicitly maintain a sequence of elements [x₀, …, xₙ₋₁].

We require that an object of type L representing such a list has only a function template with the following signature:

  template <typename F, typename A> A operator()(F f, A a);

where type F has a function template with the signature

  template <typename X> A operator()(X x, A a);

For objects l, f, and a, which are respectively of classes L, F, and A, and for objects x₀, x₁, …, xₙ₋₁ of classes X₀, X₁, …, Xₙ₋₁, the following equality holds:

  l(f, a) = f(x₀, f(x₁, …, f(xₙ₋₁, a), …)),

where the function calls f on the right-hand side are appropriate specializations of operator() for type F.

Operations as Objects

All the operations mentioned here should be constant objects with an appropriately overloaded function template operator(), so that they can be passed as arguments to functions if needed.

  • auto empty: a constant function representing the empty list
  • auto cons(auto x, auto l): a function returning the list l with x added at its beginning
  • auto create(auto...): a function returning a list consisting of the given arguments
  • auto of_range(auto r): a function returning a list created from the elements of r; it can be assumed that r is of a type satisfying the std::ranges::bidirectional_range concept, possibly wrapped in a std::reference_wrapper
  • auto concat(auto l, auto k): a function returning a list created by concatenating the lists l and k
  • auto rev(auto l): a function returning a list with the order of elements in l reversed
  • auto map(auto m, auto l): a function returning a list created from list l in such a way that each element x is replaced by m(x)
  • auto filter(auto p, auto l): a function returning a list created from the list l by retaining only those elements x that satisfy the predicate p(x)
  • auto flatten(auto l): a function returning a list created by concatenating the lists stored within the list l
  • std::string as_string(const auto& l): a function returning a string representation of the list l, assuming that for each element x of the list the operator << works, where os is an object derived from basic_ostream; see usage examples

The definitions of all the above objects should be placed in the namespace flist.
