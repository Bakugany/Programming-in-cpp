#ifndef POLY_H_
#define POLY_H_

#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>

template<typename T, size_t N = 0>
class poly;

namespace details {
    template<typename T>
    struct is_poly : std::false_type {
    };

    template<typename T, std::size_t N>
    struct is_poly<poly<T, N> > : std::true_type {
        using type = T;
    };

    template<typename T>
    constexpr bool is_poly_v = is_poly<T>::value;

    template<typename T1, typename T2>
    constexpr auto shift(const T2 &q) {
        if constexpr (is_poly_v<typename is_poly<T1>::type>) {
            return shift<typename is_poly<T1>::type>(const_poly(q));
        } else {
            return const_poly(q);
        }
    }

    template<typename T1, typename T2>
    struct common_multiply {
        using type = decltype(std::declval<T1>() * std::declval<T2>());
    };

    template<typename T1, std::size_t N1, typename T2>
    struct common_multiply<poly<T1, N1>, T2> {
        using type = poly<typename common_multiply<T1, T2>::type, N1>;
    };

    template<typename T1, typename T2, std::size_t N2>
    struct common_multiply<T1, poly<T2, N2>> {
        using type = poly<typename common_multiply<T1, T2>::type, N2>;
    };

    template<typename T1, std::size_t N1, typename T2, std::size_t N2>
    struct common_multiply<poly<T1, N1>, poly<T2, N2>> {
        static constexpr size_t result_size = (N1 && N2) ?  N1 + N2 - 1 : 0;
        using type = poly<typename common_multiply<T1, T2>::type, result_size>;
    };

    template<typename T, typename U>
    using common_multiply_t = typename common_multiply<T, U>::type;
}

template<typename... Args>
poly(Args...) -> poly<std::common_type_t<Args...>, sizeof...(Args)>;

template<typename T, size_t N>
class poly {
private:
    std::array<T, N> coefficients{};

    template<typename U, std::size_t index, typename... Rest>
        requires (index < N - 1 && details::is_poly_v<T>)
    constexpr auto at_recursive(const U &polynom, [[maybe_unused]] const Rest &... rest) const {
        return coefficients[index].at(rest...) + at_recursive<U, index + 1>(polynom, rest...) * polynom;
    }

    template<typename U, std::size_t index, typename... Rest>
        requires (index == N - 1 && details::is_poly_v<T>)
    constexpr auto at_recursive([[maybe_unused]] const U &, [[maybe_unused]] const Rest &... rest) const {
        return coefficients[index].at(rest...);
    }

    template<typename U, std::size_t index, typename... Rest>
        requires (index == N - 1 && !details::is_poly_v<T>)
    constexpr auto at_recursive([[maybe_unused]] const U &value, [[maybe_unused]] const Rest &... rest) const {
        return coefficients[index];
    }

    template<typename U, std::size_t index, typename... Rest>
        requires (index < N - 1 && !details::is_poly_v<T>)
    constexpr auto at_recursive(const U &polynom, const Rest &... rest) const {
        return coefficients[index] + at_recursive<U, index + 1>(polynom, rest...) * polynom;
    }

    template<typename U, std::size_t K, std::size_t... Is>
    constexpr auto at_array(const std::array<U, K> &arr, std::index_sequence<Is...>) const {
        return at(arr[Is]...);
    }

public:
    // Constructors
    constexpr poly() {
    }

    template<typename U, size_t M>
        requires ((M <= N) && std::is_convertible_v<U, T>)
    constexpr poly(const poly<U, M> &other) {
        for (size_t i = 0; i < M; ++i) {
            coefficients[i] = other[i];
        }
    }

    template<typename U, size_t M>
        requires ((M <= N) && std::is_convertible_v<U, T>)
    constexpr poly(poly<U, M> &&other) {
        for (size_t i = 0; i < M; ++i) {
            coefficients[i] = std::move(other[i]);
        }
    }

    template<typename U>
        requires (std::is_convertible_v<U, T>)
    constexpr poly(U &&value) {
        coefficients[0] = std::forward<U>(value);
    }

    template<typename... Args>
        requires ((2 <= sizeof...(Args)) && (sizeof...(Args) <= N) && (std::conjunction_v<std::is_convertible<Args, T>...>))
    constexpr poly(Args &&... args) {
        std::size_t iterator = 0;
        ((coefficients[iterator++] = std::forward<Args>(args)), ...);
    }

    // Assignment operators
    template<typename U, size_t M>
        requires ((M <= N) && std::is_convertible_v<U, T>)
    constexpr poly &operator=(const poly<U, M> &other) {
        for (size_t i = 0; i < M; ++i) {
            coefficients[i] = other[i];
        }
        for (size_t i = M; i < N; ++i) {
            coefficients[i] = T{};
        }
        return *this;
    }

    template<typename U, size_t M>
        requires ((M <= N) && std::is_convertible_v<U, T>)
    constexpr poly &operator=(poly<U, M> &&other) {
        for (size_t i = 0; i < M; ++i) {
            coefficients[i] = std::move(other[i]);
        }
        for (size_t i = M; i < N; ++i) {
            coefficients[i] = T{};
        }
        return *this;
    }

    // Compound assignment operators
    template<typename U, size_t M>
        requires ((M <= N) && std::is_convertible_v<U, T>)
    constexpr poly &operator+=(const poly<U, M> &other) {
        for (size_t i = 0; i < M; ++i) {
            coefficients[i] += other[i];
        }
        return *this;
    }

    template<typename U>
        requires (std::is_convertible_v<U, T>)
    constexpr poly &operator+=(const U &value) {
        coefficients[0] += value;
        return *this;
    }

    template<typename U, size_t M>
        requires ((M <= N) && std::is_convertible_v<U, T>)
    constexpr poly &operator-=(const poly<U, M> &other) {
        for (size_t i = 0; i < M; ++i) {
            coefficients[i] -= other[i];
        }
        return *this;
    }

    template<typename U>
        requires (std::is_convertible_v<U, T>)
    constexpr poly &operator-=(const U &value) {
        coefficients[0] -= value;
        return *this;
    }

    template<typename U>
        requires (std::is_convertible_v<U, T>)
    constexpr poly &operator*=(const U &value) {
        for (auto &coeff: coefficients) {
            coeff *= value;
        }
        return *this;
    }

    template<typename U>
        requires ((1 <= N) && std::is_convertible_v<U, T>)
    constexpr poly &operator*=(const poly<U, 1> &other) {
        *this = *this * other;
        return *this;
    }

    // Indexing operator
    constexpr T &operator[](size_t index) {
        return coefficients[index];
    }

    constexpr const T &operator[](size_t index) const {
        return coefficients[index];
    }

    // At method
    template<typename U, typename... Args>
    constexpr auto at(const U &value, const Args &... args) const {
      if constexpr (N == 0) {
        return T{};
      } else {
        return at_recursive<U, 0, Args...>(value, args...);
      }
    }

    constexpr auto at() const {
        return *this;
    }

    template<typename U, std::size_t K>
    constexpr auto at(const std::array<U, K> &arr) const {
        if constexpr (K == 0) {
            return *this;
        } else {
            return at_array(arr, std::make_index_sequence<K>{});
        }
    }

    // Size method
    constexpr size_t size() const {
        return N;
    }
};

// Common type
namespace std {
    template<typename T1, size_t N1, typename T2>
    struct common_type<T1, poly<T2, N1> > {
        using type = poly<std::common_type_t<T1, T2>, N1>;
    };

    template<typename T1, typename T2, size_t N2>
    struct common_type<poly<T1, N2>, T2> {
        using type = poly<std::common_type_t<T1, T2>, N2>;
    };

    template<typename T1, size_t N1, typename T2, size_t N2>
    struct common_type<poly<T1, N1>, poly<T2, N2> > {
        using type = poly<std::common_type_t<T1, T2>, (N1 > N2 ? N1 : N2)>;
    };
}

template<typename T, size_t N>
constexpr auto const_poly(const poly<T, N> &p) {
    poly<poly<T, N>, 1> result;
    result[0] = p;
    return result;
}

// Cross function
template<typename T1, typename T2>
    requires (details::is_poly_v<T1> && details::is_poly_v<T2>)
constexpr auto cross(const T1 &p, const T2 &q) {
    return p * details::shift<T1>(q);
}

// Binary operators
template<typename T, size_t N, typename U>
constexpr auto operator+(const poly<T, N> &lhs, const U &rhs) {
    using ResultType = std::common_type_t<T, U>;
    poly<ResultType, N> result = lhs;
    result += rhs;
    return result;
}

template<typename T, typename U, size_t M>
constexpr auto operator+(const T &lhs, const poly<U, M> &rhs) {
    using ResultType = std::common_type_t<T, U>;
    poly<ResultType, M> result = rhs;
    result += lhs;
    return result;
}

template<typename T, size_t N, typename U, size_t M>
constexpr auto operator+(const poly<T, N> &lhs, const poly<U, M> &rhs) {
    using ResultType = std::common_type_t<T, U>;
    constexpr size_t ResultSize = (N > M) ? N : M;
    poly<ResultType, ResultSize> result;

    for (size_t i = 0; i < N; ++i) {
        result[i] += lhs[i];
    }
    for (size_t i = 0; i < M; ++i) {
        result[i] += rhs[i];
    }

    return result;
}

template<typename T, size_t N, typename U>
constexpr auto operator-(const poly<T, N> &lhs, const U &rhs) {
    using ResultType = std::common_type_t<T, U>;
    poly<ResultType, N> result = lhs;
    result -= rhs;
    return result;
}

template<typename T, typename U, size_t M>
constexpr auto operator-(const T &lhs, const poly<U, M> &rhs) {
    using ResultType = std::common_type_t<T, U>;
    poly<ResultType, M> result = -rhs;
    result += lhs;
    return result;
}

template<typename T, size_t N, typename U, size_t M>
constexpr auto operator-(const poly<T, N> &lhs, const poly<U, M> &rhs) {
    using ResultType = std::common_type_t<T, U>;
    constexpr size_t ResultSize = (N > M) ? N : M;
    poly<ResultType, ResultSize> result;

    for (size_t i = 0; i < N; ++i) {
        result[i] += lhs[i];
    }
    for (size_t i = 0; i < M; ++i) {
        result[i] -= rhs[i];
    }

    return result;
}

template<typename T, size_t N, typename U>
constexpr auto operator*(const poly<T, N> &lhs, const U &rhs) {
    using ResultType = details::common_multiply_t<T, U>;
    poly<ResultType, N> result = lhs;
    result *= rhs;
    return result;
}

template<typename T, typename U, size_t M>
constexpr auto operator*(const T &lhs, const poly<U, M> &rhs) {
    using ResultType = details::common_multiply_t<T, U>;
    poly<ResultType, M> result = rhs;
    result *= lhs;
    return result;
}

template<typename T, size_t N, typename U, size_t M>
constexpr auto operator*(const poly<T, N> &lhs, const poly<U, M> &rhs) {
    using ResultType = details::common_multiply_t<T, U>;
    if constexpr (N == 0 || M == 0) {
        return poly<ResultType, 0>{};
    } else {
        constexpr size_t ResultSize = N + M - 1;
        poly<ResultType, ResultSize> result;

        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                result[i + j] += lhs[i] * rhs[j];
            }
        }

        return result;
    }
}

// Unary operator
template<typename T, size_t N>
constexpr  poly<T, N> operator-(const poly<T, N> &p) {
    poly<T, N> result;
    for (size_t i = 0; i < N; ++i) {
        result[i] = -p[i];
    }
    return result;
}

#endif // POLY_H_
