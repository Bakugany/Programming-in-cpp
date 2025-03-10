export module FibonacciCounter;

export import Counter;

// Needed here for UINT64_MAX. For some reason, it's not enough to import it in
// the Manager module.
export import <cstdint>;

export class FibonacciCounter : public Counter {
  public:
    FibonacciCounter(std::uint64_t id, std::uint64_t p) noexcept
        : Counter(id, p), prev_fib(1), current_fib(1) {}

  protected:
    virtual std::uint64_t
    ValueIncrementationsBeforeNextEvent() const noexcept override {
        return current_fib - value - 1;
    }

    virtual Event
    ReportEvent(std::uint64_t impulsesSinceLastAdd) noexcept override {
        Event event(id, impulsesSinceLastAdd);
        value = current_fib;
        cooldown = 0;
        updateFib();
        return event;
    }

  private:
    void
    updateFib() noexcept {
        if (current_fib > UINT64_MAX - prev_fib) {
            reportedFinalEvent = true;
            return;
        }
        std::uint64_t next_fib = prev_fib + current_fib;
        prev_fib = current_fib;
        current_fib = next_fib;
    }

    std::uint64_t prev_fib;    // Previous Fibonacci number
    std::uint64_t current_fib; // Current Fibonacci number
};
