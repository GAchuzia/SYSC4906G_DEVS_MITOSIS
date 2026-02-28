#   make all        # build simulation and run_tests (no run)
#   make simulation # build and run simulation
#   make tests      # build and run run_tests
#   make clean      # remove executables
# Override: make CADMIUM_PATH=/path/to/cadmium/include

CADMIUM_PATH ?= /home/cadmium/rt_cadmium/include
CXX          = g++
CXXFLAGS     = -g -O3 -std=c++17 -I$(CADMIUM_PATH)

BUILD_DIR   = build
SIMULATION  = $(BUILD_DIR)/simulation
RUN_TESTS   = $(BUILD_DIR)/run_tests

TOP_HEADERS  = top_model/top.hpp
MAIN_HEADERS = main/include/centrosome.hpp main/include/centrosome_pair.hpp \
	main/include/cell_structures.hpp main/include/chromosome.hpp \
	main/include/nuclear_envelope.hpp main/include/nucleolus.hpp \
	main/include/phase_control.hpp main/include/phase_controller.hpp \
	main/include/spindle.hpp main/include/status_checker.hpp

.PHONY: all simulation tests clean

all: $(SIMULATION) $(RUN_TESTS)
	@echo "Build complete. Run: ./$(SIMULATION) or ./$(RUN_TESTS)"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SIMULATION): main/main.cpp $(TOP_HEADERS) $(MAIN_HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) main/main.cpp -o $(SIMULATION)

$(RUN_TESTS): tests/main.cpp tests/test.hpp $(MAIN_HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) tests/main.cpp -o $(RUN_TESTS)

simulation: $(SIMULATION)
	./$(SIMULATION)

tests: $(RUN_TESTS)
	mkdir -p tests/results
	./$(RUN_TESTS)

clean:
	rm -f $(SIMULATION) $(RUN_TESTS)
