CXX = /opt/llvm/19.1.4/bin/clang++
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -fprebuilt-module-path=. -Wno-experimental-header-units -Wno-pragma-system-header-outside-header

SUBMODULE_FILES = EventModule.cppm OverflowSecurity.cppm Counter.cppm FibonacciCounter.cppm GeometricCounter.cppm ModuloCounter.cppm
LIB_NAMES_FOR_SUBMODULES = functional cstdint iostream vector algorithm
LIB_NAMES = $(LIB_NAMES_FOR_SUBMODULES) map string memory regex cctype

PCH_FILES_FOR_SUBMODULES = $(foreach lib,$(LIB_NAMES_FOR_SUBMODULES),$(lib).pch)
PCH_FILES = $(foreach lib,$(LIB_NAMES),$(lib).pch)
PCM_FILES = $(foreach file,$(SUBMODULE_FILES),$(basename $(file)).pcm)

O_FILES = $(foreach file,$(SUBMODULE_FILES),$(basename $(file)).o) Manager.o

PCH_MODULE_FILES = $(foreach file,$(PCH_FILES),-fmodule-file=$(file))
PCH_MODULE_FILES_FOR_SUBMODULES = $(foreach file,$(PCH_FILES_FOR_SUBMODULES),-fmodule-file=$(file))

.PHONY: all clean-most clean

all: counters

# -Wno-ambiguous-ellipsis to suppress warning when precompiling <functional>.
# -Wno-deprecated-declarations to suppress warning when precompiling bind in addImpulsesToAll in Manager module.
%.pch:
	$(CXX) $(CXXFLAGS) -Wno-deprecated-declarations -Wno-ambiguous-ellipsis -xc++-system-header --precompile $* -o $@

Counter.pcm: Counter.cppm EventModule.pcm OverflowSecurity.pcm
%Counter.pcm: %Counter.cppm Counter.pcm
GeometricCounter.pcm: GeometricCounter.cppm ModuloCounter.pcm

%.pcm: %.cppm $(PCH_FILES_FOR_SUBMODULES)
	$(CXX) $(CXXFLAGS) --precompile $(PCH_MODULE_FILES_FOR_SUBMODULES) $< -o $@

Manager.pcm: Manager.cppm $(PCH_FILES) $(PCM_FILES)
	$(CXX) $(CXXFLAGS) --precompile $(PCH_MODULE_FILES) $< -o $@

%.o: %.pcm
	$(CXX) $(CXXFLAGS) -c $< -o $@

counters: $(O_FILES)
	$(CXX) $(CXXFLAGS) $(O_FILES) -o $@

clean-most:
	rm -f *.pch *.pcm *.o

clean:
	rm -f *.pch *.pcm *.o counters
