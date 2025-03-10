export module EventModule;

import <cstdint>;
import <iostream>;
import <vector>;
import <algorithm>;

export class Event {
  public:
    Event(std::uint64_t id, std::uint64_t t) noexcept : id(id), t(t) {}

    bool
    operator<(const Event& other) const noexcept {
        if (t != other.t) {
            return t < other.t; // Sort by t in increasing order
        }
        return id < other.id; // Sort by id in increasing order
    }

    void
    print() const noexcept {
        std::cout << "E " << id << " " << t << "\n";
    }

  private:
    std::uint64_t id;
    std::uint64_t t;
};

export class EventContainer {
  public:
    void
    addEvent(const Event& event) {
        events.push_back(event);
    }

    void
    sortEvents() {
        std::sort(events.begin(), events.end());
    }

    void
    printEvents() const noexcept {
        for (const auto& event : events) {
            event.print();
        }
    }

    void
    clear() noexcept {
        events.clear();
    }

  private:
    std::vector<Event> events;
};