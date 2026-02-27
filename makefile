# Cadmium Mitosis — Makefile
# CADMIUM_PATH: Cadmium include path (default: devssim)
# Targets: simulation (default), run_tests, clean
CADMIUM_PATH ?= /home/cadmium/rt_cadmium/include
CXX         = g++
CXXFLAGS    = -g -O3 -std=c++17 -I $(CADMIUM_PATH)

.PHONY: all clean simulation run_tests

all: simulation

simulation: main/main.cpp top_model/top.hpp main/include/*.hpp
	$(CXX) $(CXXFLAGS) main/main.cpp -o simulation
	@echo "Build successful. Run: ./simulation"

run_tests: tests/main.cpp tests/test.hpp main/include/*.hpp
	$(CXX) $(CXXFLAGS) tests/main.cpp -o run_tests
	@echo "Tests built. Run: ./run_tests"

clean:
	rm -f simulation run_tests
