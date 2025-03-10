export module ModuloCounter;

export import Counter;

export class ModuloCounter : public Counter {
  public:
    ModuloCounter(std::uint64_t id, std::uint64_t p, std::uint64_t m) noexcept
        : Counter(id, p), m(m) {}

  protected:
    virtual std::uint64_t
    ValueIncrementationsBeforeNextEvent() const noexcept override {
        return m - value;
    }

    virtual Event
    ReportEvent(std::uint64_t impulsesSinceLastAdd) noexcept override {
        Event event = {id, impulsesSinceLastAdd};
        value = 0;
        cooldown = 0;
        return event;
    }

  protected:
    std::uint64_t m; // Maximum value
};
