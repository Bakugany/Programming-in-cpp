export module Manager;

import Counter;
import FibonacciCounter;
import GeometricCounter;
import ModuloCounter;
import EventModule;

import <iostream>;
import <cstdint>;
import <map>;
import <memory>;
import <string>;
import <regex>;
import <cctype>;
import <algorithm>;

export class Manager {
  public:
    Manager() noexcept : counters() {}

    void processCommands(std::istream& input) noexcept;
    void addImpulseToAll(std::uint64_t t);
    void printCounter(std::uint64_t c);
    void deleteCounter(std::uint64_t c);

  private:
    std::map<std::uint64_t, std::unique_ptr<Counter>> counters;
    EventContainer eventContainer;

    static std::uint64_t
    stouint64(const std::string& str) {
        if constexpr (sizeof(std::uint64_t) == sizeof(unsigned long)) {
            return std::stoul(str);
        } else {
            return std::stoull(str);
        }
    }
};

void
Manager::processCommands(std::istream& input) noexcept {
    static const std::regex command_regex_M(R"(^M (\d+) (\d+) (\d+)$)");
    static const std::regex command_regex_F(R"(^F (\d+) (\d+)$)");
    static const std::regex command_regex_G(R"(^G (\d+) (\d+)$)");
    static const std::regex command_regex_D(R"(^D (\d+)$)");
    static const std::regex command_regex_P(R"(^P (\d+)$)");
    static const std::regex command_regex_A(R"(^A (\d+)$)");

    std::string line;
    std::smatch match;
    std::uint64_t c = 0, p = 0, m = 0, t = 0;
    std::uint64_t lineNumber = 0;

    while (std::getline(input, line)) {
        lineNumber++;
        try {
            if (std::regex_match(line, match, command_regex_M)) {
                c = stouint64(match[1].str());
                p = stouint64(match[2].str());
                m = stouint64(match[3].str());
                if (counters.contains(c)) {
                    throw std::runtime_error("Counter already exists.");
                }
                counters[c] = std::make_unique<ModuloCounter>(c, p, m);
            } else if (std::regex_match(line, match, command_regex_F)) {
                c = stouint64(match[1].str());
                p = stouint64(match[2].str());
                if (counters.contains(c)) {
                    throw std::runtime_error("Counter already exists.");
                }
                counters[c] = std::make_unique<FibonacciCounter>(c, p);
            } else if (std::regex_match(line, match, command_regex_G)) {
                c = stouint64(match[1].str());
                p = stouint64(match[2].str());
                if (counters.contains(c)) {
                    throw std::runtime_error("Counter already exists.");
                }
                counters[c] = std::make_unique<GeometricCounter>(c, p);
            } else if (std::regex_match(line, match, command_regex_D)) {
                c = stouint64(match[1].str());
                deleteCounter(c);
            } else if (std::regex_match(line, match, command_regex_P)) {
                c = stouint64(match[1].str());
                printCounter(c);
            } else if (std::regex_match(line, match, command_regex_A)) {
                t = stouint64(match[1].str());
                addImpulseToAll(t);
            } else {
                throw std::runtime_error("Unknown command.");
            }
        } catch (const std::exception& e) {
            std::cerr << "ERROR " << lineNumber << "\n";
        }
    }
}

void
Manager::addImpulseToAll(std::uint64_t t) {
    eventContainer.clear();

    for (auto& [id, counter] : counters) {
        counter->addImpulse(t, std::bind(&EventContainer::addEvent,
                                      &eventContainer, std::placeholders::_1));
    }

    eventContainer.sortEvents();
    eventContainer.printEvents();
}

void
Manager::printCounter(std::uint64_t c) {
    if (counters.contains(c)) {
        std::cout << "C " << c << " " << counters[c]->getValue() << "\n";
    } else {
        throw std::runtime_error("Counter does not exist");
    }
}

void
Manager::deleteCounter(std::uint64_t c) {
    if (counters.contains(c)) {
        counters.erase(c);
    } else {
        throw std::runtime_error("Counter does not exist");
    }
}

int
main() {
    Manager manager;
    manager.processCommands(std::cin);
    return 0;
}
