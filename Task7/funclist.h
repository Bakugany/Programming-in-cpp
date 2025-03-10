#ifndef FUNCLIST_H
#define FUNCLIST_H

#include <functional>
#include <string>
#include <sstream>
#include <utility>
#include <ranges>
#include <type_traits>
#include <iostream>

namespace flist {

    inline auto rev = [](auto l) {
        return [=](auto f, auto a) {
            using A = decltype(a);
            using FuncType = std::function<A(A)>;
            FuncType identity = [](A val) { return val; };
            FuncType result = l(
                [=](auto x, FuncType acc) {
                    return [=](A finalA) {
                        return acc(f(x, finalA));
                    };
                },
                identity
            );
            return result(a);
        };
    };

    inline auto empty = [](auto, auto a) {
        return a;
    };

    inline auto cons = [](auto x, auto l) {
        return [=](auto f, auto a) {
            return f(x, l(f, a));
        };
    };

    inline auto create = [](auto... args) {
        auto list = [=](auto f, auto a) {
            (..., (a = f(args, a)));
            return a;
        };
        return rev(list);
    };

    namespace detail {

        template <typename Range, typename Iterator, typename Func, typename Acc>
        inline auto of_range_helper(Range &r, Iterator it, Func f, Acc a) -> decltype(a) {
            return (it == r.end()) ? a : f(*it, of_range_helper(r, std::next(it), f, a));
        }

    }

    template <typename Range>
    auto of_range(Range r) {
        typename std::unwrap_reference<Range>::type range = r;

        return [=](auto f, auto a) -> decltype(a) {
            auto it = range.begin();
            return (it == range.end()) ? a : f(*it, detail::of_range_helper(range, std::next(it), f, a));
        };
    }

    inline auto concat = [](auto l, auto k) {
        return [=](auto f, auto a) {
            return l(f, k(f, a));
        };
    };

    inline auto map = [](auto m, auto l) {
        return [=](auto f, auto a) {
            return l([=](auto x, auto acc) {
                return f(m(x), acc);
            }, a);
        };
    };

    inline auto filter = [](auto p, auto l) {
        return [=](auto f, auto a) {
            return l([=](auto x, auto acc) {
                return p(x) ? f(x, acc) : acc;
            }, a);
        };
    };

    inline auto flatten = [](auto l) {
        return [=](auto f, auto a) {
            return l([=](auto sublist, auto acc) {
                return sublist(f, acc);
            }, a);
        };
    };

    inline auto as_string = [](const auto& l) {
        std::ostringstream os;
        os << "[";
        rev(l)([&](auto x, bool a) {
            os << (a ? ";" : "") << x;
            return 1;
        }, 0);
        os << "]";
        return os.str();
    };

}
#endif // FUNCLIST_H