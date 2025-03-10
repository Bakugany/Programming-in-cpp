export module GeometricCounter;

export import ModuloCounter;

export class GeometricCounter : public ModuloCounter {
  public:
    GeometricCounter(std::uint64_t id, std::uint64_t p) noexcept
        : ModuloCounter(id, p, 9) {}

  protected:
    virtual Event
    ReportEvent(std::uint64_t impulsesSinceLastAdd) noexcept override {
        updateMaxValue();
        return ModuloCounter::ReportEvent(impulsesSinceLastAdd);
    }

  private:
    void
    updateMaxValue() noexcept {
        if (m == MAXLIMIT) {
            m = 9;
        } else {
            m = m * 10 + 9;
        }
    }

    static const std::uint64_t MAXLIMIT = 999999999999;
};
