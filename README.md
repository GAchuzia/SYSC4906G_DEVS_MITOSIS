# Cadmium Mitosis Model

This folder contains the MITOSIS DEVS model implemented in Cadmium.

---

## Files organization

**Root**

- `README.md`
- `makefile`
- `build_sim.sh`

**main/**-Entry point and atomic/coupled model headers

- `main.cpp`
- `include/`
  - `chromosome.hpp`
  - `centrosome.hpp`
  - `centrosome_pair.hpp`
  - `nuclear_envelope.hpp`
  - `nucleolus.hpp`
  - `spindle.hpp`
  - `phase_controller.hpp`
  - `status_checker.hpp`

**top_model/**-Top coupled model

- `top.hpp`

**input_data/**-Input data for the model and tests

- `atomic_models/`
  - `TC_Chromosome1.txt`, `TC_Chromosome2.txt`
  - `TC_Nuclear_Envelope1.txt`, `TC_Nuclear_Envelope2.txt`
  - `TC_Centrosome1.txt`, `TC_Nucleolus1.txt`, `TC_Spindle1.txt`
  - `TC_StatusChecker1.txt`, `TC_PhaseController1.txt`, `TC_PhaseController.txt`
- `TC_Centrosome_Pair1.txt`

**simulation_results/**-Created when you run the model. Contains CSV outputs per model.

- `chromosome_1.txt`, `chromosome_2.txt`, `nuclear_envelope_1.txt`, `nuclear_envelope_2.txt`, `centrosome.txt`, `nucleolus.txt`, …

**tests/**-Unit tests for atomic models

- `main.cpp`
- `test.hpp`

**atomics/**-DEVS Graph / JSON specs for atomic models

- `ChromosomeModel.json`, `NuclearEnvelopeModel.json`, `CentrosomeModel.json`, `NucleolusModel.json`, `SpindleAppartusModel.json`, `StatusChecker.json`, `PhaseController.json`

---

## Steps

### 1. Update include path for Cadmium

- **Makefile:** set `CADMIUM_PATH` to your Cadmium include path (default: `/home/cadmium/rt_cadmium/include`), or run:
  ```bash
  make CADMIUM_PATH=/path/to/cadmium/include
  ```
- **build_sim.sh:** edit the line `CADMIUM_PATH="/home/cadmium/rt_cadmium/include"`

### 2. Compile the project

1. Open the terminal in this folder (Cadmium-Mitosis).
2. Run:
   ```bash
   make clean; make all
   ```
   Or use the script: `./build_sim.sh`
3. This creates the executable `simulation` in this folder. Optionally, `make run_tests` creates `run_tests` for atomic tests.

### 3. Run the top model

1. Open the terminal in this folder.
2. Run:
   ```bash
   ./simulation
   ```
3. Output goes to stdout and to the `simulation_results/` folder (one CSV file per model, e.g. `chromosome_1.txt`, `nuclear_envelope_1.txt`, `centrosome.txt`, `nucleolus.txt`).

### 4. Run atomic tests (optional)

1. If you built `run_tests` (`make run_tests`), run:
   ```bash
   ./run_tests
   ```
2. Each test runs one atomic with an input file from `input_data/atomic_models/`. Output is CSV format to stdout.

### 5. Run with different inputs

1. Add or edit `.txt` files in `input_data/atomic_models/` with the same format: one line per event, `<time> <phase>` (e.g. `0.0 Prophase`). Valid phases: Interphase, Prophase, Metaphase, Anaphase, Telophase, Cytokinesis.
2. Update `top_model/top.hpp` to point the reader for each model to the new file if needed.
3. Run `./simulation` again. Existing files in `simulation_results/` will be overwritten; rename them if you want to keep previous runs.
