export module Counter;

export import EventModule;
export import <cstdint>;
export import <functional>;
export import <iostream>;

import OverflowSecurity;

export class Counter {
  public:
    Counter(std::uint64_t id, std::uint64_t p) noexcept
        : id(id), p(p), value(0), cooldown(p), reportedFinalEvent(false) {}
    virtual ~Counter() = default;

    void
    addImpulse(std::uint64_t t,
               const std::function<void(const Event&)>& addEvent) noexcept {
        std::uint64_t impulsesSinceLastInvokation = 0;

        std::uint64_t beforeNextEvent = ImpulsesBeforeNextEvent();
        while (!reportedFinalEvent && t > beforeNextEvent) {
            impulsesSinceLastInvokation += beforeNextEvent + 1;
            t -= beforeNextEvent + 1;
            addEvent(ReportEvent(impulsesSinceLastInvokation));
            beforeNextEvent = ImpulsesBeforeNextEvent();
        }

        updateValueCooldown(t);
    }

    std::uint64_t
    getValue() const noexcept {
        return value;
    }

  protected:
    virtual Event ReportEvent(std::uint64_t impulsesSinceLastAdd) noexcept = 0;

    // The amount of value increases before adding just one causes an event.
    virtual std::uint64_t ValueIncrementationsBeforeNextEvent() const noexcept = 0;

    std::uint64_t id;
    std::uint64_t p;
    std::uint64_t value;
    std::uint64_t cooldown;

    bool reportedFinalEvent;

  private:
    void
    updateValueCooldown(std::uint64_t t) noexcept {
        std::uint64_t value_increase = 0;

        // value_increase = (t + cooldown) / (d + 1)
        //    cooldown    = (t + cooldown) % (d + 1)
        OverflowSecurity::extendedDiv(t, cooldown, p, value_increase, cooldown);

        // Make sure we don't overflow the value when increasing.
        if (UINT64_MAX - value_increase > value) {
            value += value_increase;
        } else {
            value = UINT64_MAX;
        }
    }

    // The amount of impulses needed before just one more causes an event.
    // Returns UINT64_MAX if more than UINT64_MAX impulses are needed.
    std::uint64_t
    ImpulsesBeforeNextEvent() const noexcept {
        // formula: (value_increase + 1) * (p + 1) - cooldown - 1
        return OverflowSecurity::extendedMulSub(ValueIncrementationsBeforeNextEvent(),
                                                p, cooldown);
    }
};
