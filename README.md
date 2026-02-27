# Cadmium Mitosis Model

This folder contains the MITOSIS DEVS model implemented in Cadmium.

---

## Files organization

**Root**

- `README.md`
- `makefile`
- `build_sim.sh`

**main/**  
Entry point and atomic/coupled model headers

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

**top_model/**  
Top coupled model

- `top.hpp`: Currently runs one Centrosome fed by `input_data/TC_Centrosome_Pair1.txt`

**input_data/**  
Input data for the model and tests

- `TC_Centrosome_Pair1.txt`: Used by the top model
- `atomic_models/`: Used by `run_tests`: `TC_Chromosome1.txt`, `TC_Chromosome2.txt`, `TC_Nuclear_Envelope1.txt`, `TC_Nuclear_Envelope2.txt`, `TC_Centrosome1.txt`, `TC_Nucleolus1.txt`, etc.

**simulation_results/**  
Created when you run `./simulation`. One CSV file per component (e.g. `centrosome.txt`, `input_reader.txt`).

**tests/**  
Unit tests for atomic models

- `main.cpp`
- `test.hpp`

**atomics/**  
DEVS Graph / JSON specs

- `ChromosomeModel.json`, `NuclearEnvelopeModel.json`, `CentrosomeModel.json`, `NucleolusModel.json`, `SpindleAppartusModel.json`, `StatusChecker.json`, `PhaseController.json`

---

## Steps

### 1. Set Cadmium include path

- **Makefile:** set `CADMIUM_PATH` or run `make CADMIUM_PATH=/path/to/cadmium/include`
- **build_sim.sh:** edit `CADMIUM_PATH="/home/cadmium/rt_cadmium/include"`

### 2. Build

From this folder:

```bash
make clean; make all
```

Or: `./build_sim.sh`

This creates `simulation`. For tests: `make run_tests` creates `run_tests`.

### 3. Run the top model

```bash
./simulation
```

Reads phase input from `input_data/TC_Centrosome_Pair1.txt` (see `top_model/top.hpp`). Writes CSV to stdout and to `simulation_results/` (one file per component).

### 4. Run atomic tests

After `make run_tests`:

```bash
./run_tests
```

Runs Centrosome, Nucleolus, Chromosome (two cases), and Nuclear Envelope (two cases). Each uses a file from `input_data/atomic_models/`. Output is CSV to stdout.

