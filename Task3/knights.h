#ifndef KNIGHTS_H_
#define KNIGHTS_H_

#include <algorithm>
#include <cassert>
#include <compare>
#include <cstddef>
#include <limits>
#include <list>
#include <ostream>

class Knight {
public:
    constexpr static size_t MAX_GOLD = std::numeric_limits<size_t>::max();

    constexpr Knight(const size_t gold, const size_t weapon_class, const size_t armor_class)
        : gold_(gold), weapon_class_(weapon_class), armor_class_(armor_class) {}

    constexpr Knight(const Knight &) = default;
    constexpr Knight(Knight &&) = default;
    constexpr Knight &operator=(const Knight &) = default;
    constexpr Knight &operator=(Knight &&) = default;

    constexpr size_t get_gold() const {
        return gold_;
    }

    constexpr size_t get_armour_class() const {
        return armor_class_;
    }

    constexpr size_t get_weapon_class() const {
        return weapon_class_;
    }

    void take_gold(const size_t gold) {
        if (MAX_GOLD - gold_ > gold)
            gold_ += gold;
        else
            gold_ = MAX_GOLD;
    }

    size_t give_gold() {
        size_t g = gold_;
        gold_ = 0;
        return g;
    }

    void change_weapon(const size_t weapon_class) {
        weapon_class_ = weapon_class;
    }

    size_t give_up_weapon() {
        const size_t wc = weapon_class_;
        weapon_class_ = 0;
        return wc;
    }

    void change_armour(const size_t armor_class) {
        armor_class_ = armor_class;
    }

    size_t take_off_armour() {
        const size_t ac = armor_class_;
        armor_class_ = 0;
        return ac;
    }

    constexpr auto operator<=>(const Knight &other) const {
        if (armor_class_ >= other.weapon_class_) {
            if (weapon_class_ > other.armor_class_)
                return std::weak_ordering::greater;
            return std::weak_ordering::equivalent;
        }

        if (other.armor_class_ >= weapon_class_)
            return std::weak_ordering::less;

        if (armor_class_ > other.armor_class_)
            return std::weak_ordering::greater;

        if (other.armor_class_ > armor_class_)
            return std::weak_ordering::less;

        if (weapon_class_ > other.weapon_class_)
            return std::weak_ordering::greater;

        if (other.weapon_class_ > weapon_class_)
            return std::weak_ordering::less;

        return std::weak_ordering::equivalent;
    }

    constexpr bool operator==(const Knight &other) const {
        if (*this <=> other == nullptr)
            return true;
        return false;
    }

    constexpr Knight operator+(const Knight &other) const {
        Knight result = *this;
        if (MAX_GOLD - result.gold_ > other.gold_)
            result.gold_ += other.gold_;
        else
            result.gold_ = MAX_GOLD;

        result.armor_class_ = std::max(result.armor_class_, other.armor_class_);
        result.weapon_class_ = std::max(result.weapon_class_, other.weapon_class_);
        return result;
    }

    Knight& operator+=(Knight &other) & {
        take_gold(other.give_gold());
        if (other.weapon_class_ > weapon_class_) {
            weapon_class_ = other.weapon_class_;
            other.weapon_class_ = 0;
        }

        if (other.armor_class_ > armor_class_) {
            armor_class_ = other.armor_class_;
            other.armor_class_ = 0;
        }
        return *this;
    }

private:
    size_t gold_, weapon_class_, armor_class_;
};

inline std::ostream &operator<<(std::ostream &os, const Knight &knight) {
    os << "(" << knight.get_gold() << " gold, "
            << knight.get_weapon_class() << " weapon class, "
            << knight.get_armour_class() << " armour class)\n";
    return os;
}

static constexpr auto TRAINEE_KNIGHT = Knight(0, 1, 1);

class Tournament {
public:
    using knight_list = std::list<Knight>;

    template<typename T>
    struct dependent_false : std::false_type {};

    template <typename Container>
    Tournament(const Container& knights) {
        if constexpr (requires { knights.begin(); knights.end(); })
            contestants.assign(knights.begin(), knights.end());
        else if constexpr (requires { knights.size(); knights[0]; })
            for (size_t i = 0; i < knights.size(); i++)
                contestants.push_back(knights[i]);
        else
            static_assert(dependent_false<Container>::value, "Container type not supported by Tournament.");

        if (contestants.empty())
            contestants.push_back(TRAINEE_KNIGHT);
    }

    Tournament(std::initializer_list<Knight> knights) : contestants(knights) {
        if (contestants.empty())
            contestants.push_back(TRAINEE_KNIGHT);
    }

    Tournament(const Tournament &) = default;
    Tournament(Tournament &&) = default;
    Tournament &operator=(const Tournament &) & = default;
    Tournament &operator=(Tournament &&) & = default;

    Tournament& operator+=(const Knight &knight) & {
        eliminated.clear();
        contestants.push_back(knight);
        return *this;
    }

    Tournament& operator-=(const Knight &knight) & {
        eliminated.clear();
        contestants.erase(
        std::remove_if(contestants.begin(), contestants.end(),
                    [&knight](const Knight& k) {
                        if (k.get_armour_class() != knight.get_armour_class())
                            return false;
                        if (k.get_weapon_class() != knight.get_weapon_class())
                            return false;
                        if (k.get_gold() != knight.get_gold())
                            return false;
                        return true;
                    }),
            contestants.end()
        );
        return *this;
    }

    knight_list::const_iterator play() & {
        eliminated.clear();
        while (contestants.size() > 1) {
            Knight knight1 = contestants.front();
            contestants.pop_front();
            Knight knight2 = contestants.front();
            contestants.pop_front();

            if (knight1 > knight2) {
                knight1 += knight2;
                contestants.push_back(knight1);
                eliminated.push_front(knight2);
            } else if (knight2 > knight1) {
                knight2 += knight1;
                contestants.push_back(knight2);
                eliminated.push_front(knight1);
            } else {
                eliminated.push_front(knight2);
                eliminated.push_front(knight1);
            }
        }
        return contestants.empty() ? no_winner() : contestants.begin();
    }

    knight_list::const_iterator no_winner() const & {
        return contestants.end();
    }

    knight_list::const_iterator no_winner() const && = delete;

    size_t size() const {
        return contestants.size() + eliminated.size();
    }

    friend std::ostream &operator<<(std::ostream &os, const Tournament &tournament) {
        for (const auto &knight: tournament.contestants)
            os << "+ " << knight;

        for (const auto &knight: tournament.eliminated)
            os << "- " << knight;

        os << "=\n";
        return os;
    }

private:
    knight_list contestants, eliminated;
};

template <typename Container>
consteval std::pair<size_t, size_t> max_diff_classes(Container knights) {
    std::pair<size_t, size_t> max_diff(0, 0);
    for (const auto& knight : knights) {
        const size_t diff = knight.get_armour_class() > knight.get_weapon_class()
                                ? knight.get_armour_class() - knight.get_weapon_class()
                                : knight.get_weapon_class() - knight.get_armour_class();

        const size_t tmp = max_diff.second > max_diff.first
                                ? max_diff.second - max_diff.first
                                : max_diff.first - max_diff.second;
        if (diff >= tmp)
            max_diff = std::make_pair(knight.get_weapon_class(), knight.get_armour_class());
    }
    return max_diff;
}

consteval std::pair<size_t, size_t> max_diff_classes(std::initializer_list<Knight> knights) {
    return max_diff_classes<std::initializer_list<Knight>>(knights);
}

#endif // KNIGHTS_H_
