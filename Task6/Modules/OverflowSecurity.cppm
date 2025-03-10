export module OverflowSecurity;

export import <cstdint>;

namespace OverflowSecurity {

// After the division, the results will be as follows:
//       result   = (a + b) / (d + 1)
//      remainder = (a + b) % (d + 1)
// No constraints values for a, b, d except d != 0.
export void
extendedDiv(std::uint64_t a, std::uint64_t b, std::uint64_t d,
            std::uint64_t& result, std::uint64_t& remainder) noexcept {
    if (d == UINT64_MAX) {
        if (UINT64_MAX - a < b) {
            result = 1;
        }
        remainder = a + b;
    } else {
        // Make sure both a and b are less than d + 1.
        result = a / (d + 1);
        a %= (d + 1);

        // Now we don't even need to use modulo.
        if (d + 1 - a <= b) {
            result += 1;
            if (UINT64_MAX - a < b) {
                // Default behaviour for overflow is modulo 2^64.
                // UINT64_MAX + r + 1 - s - 1 = UINT64_MAX - (r - s)
                std::uint64_t overflow = a + b;
                std::uint64_t underflow = d - overflow;
                remainder = UINT64_MAX - underflow;
            } else {
                remainder = a + b - d - 1;
            }
        } else {
            remainder = a + b;
        }
    }
}

// Returns floor((a + b + c) / d).
// Assumes that:
//      (a + b + c) is a number in range [0, 2^65 - 1]
//           d      is a number in range [1, 2^64 - 1]
// c is boolean, since it's 0 or 1 - this is so the function works for all
// possible values of (a + b + c).
std::uint64_t
extendedFloorDiv(std::uint64_t a, std::uint64_t b, bool c, std::uint64_t d) noexcept {
    std::uint64_t result = 0;

    result += a / d;
    a %= d;
    result += b / d;
    b %= d;

    if (UINT64_MAX - a < b || a + b >= d - static_cast<std::uint64_t>(c)) {
        return result + 1;
    } else {
        return result;
    }
}

// Returns (a + 1) * (b + 1) - (c + 1) or UINT64_MAX if it wouldn't fit in
// std::uint64_t.
export std::uint64_t
extendedMulSub(std::uint64_t a, std::uint64_t b, std::uint64_t c) noexcept {
    if (b == UINT64_MAX) {
        if (a >= 1 || (a == 0 && c == 0)) {
            return UINT64_MAX;
        } else {
            return UINT64_MAX - c;
        }
    } else if (a == UINT64_MAX) {
        if (b >= 1 || (b == 0 && c == 0)) {
            return UINT64_MAX;
        } else {
            return UINT64_MAX - c;
        }
    } else {
        if (b > 0 && a + 1 >= extendedFloorDiv(UINT64_MAX, c, 1, b + 1)) {
            // (a + 1) * (b + 1) - c - 1 >= UINT64_MAX
            return UINT64_MAX;
        } else if (b > 0 &&
                   a + 1 >= extendedFloorDiv(UINT64_MAX, 0, 1, b + 1)) {
            // UINT64_MAX <= (a + 1) * (b + 1) <= UINT64_MAX + c + 1

            // Default behaviour for overflow is modulo 2^64.
            std::uint64_t overflow = (a + 1) * (b + 1);
            if (c <= overflow) {
                return UINT64_MAX;
            } else {
                return UINT64_MAX - overflow + c;
            }
        } else {
            return (a + 1) * (b + 1) - c - 1;
        }
    }
}

} // namespace OverflowSecurity