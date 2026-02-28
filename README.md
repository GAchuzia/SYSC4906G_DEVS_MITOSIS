# Cadmium Mitosis Model

This folder contains the **Mitosis** DEVS model implemented in Cadmium.

---

# FILES ORGANIZATION

**Makefile**  
Build and run targets: `make all` (build both executables), `make simulation`, `make tests`, `make clean`.

**build_sim.sh**  
Alternate script to compile the simulation only.

**build/**  
*Created automatically when you compile. Contains the executables `simulation` and `run_tests`.*

---

**main/**
*Entry point and source for the full mitosis simulation.*

- main.cpp
- include/
  - centrosome.hpp
  - centrosome_pair.hpp
  - cell_structures.hpp
  - chromosome.hpp
  - nuclear_envelope.hpp
  - nucleolus.hpp
  - phase_control.hpp
  - phase_controller.hpp
  - spindle.hpp
  - status_checker.hpp
  - mitosis_system.hpp

---

**top_model/**
*Top coupled model: wires the mitosis system and reads the start trigger from `input_data/coupled_models/start.txt`.*

- top.hpp
- mitosis_simulation.hpp
- mitosis_sim.cpp

---

**tests/**  
*Unit tests for atomic and coupled models.*

- main.cpp
- test.hpp

---

**input_data/**  
*Input data to run the model and the tests.*

- atomic_models/
  - TC_Centrosome1.txt
  - TC_Chromosome1.txt, TC_Chromosome2.txt
  - TC_Nuclear_Envelope1.txt, TC_Nuclear_Envelope2.txt
  - TC_Nucleolus1.txt
  - TC_PhaseController.txt, TC_PhaseController1.txt
  - TC_Spindle1.txt
  - TC_StatusChecker1.txt
  - mitosis_input.txt
- coupled_models/
  - start.txt
  - chrom_input.txt, ne_input.txt, sp_input.txt, cen_input.txt, nucleo_input.txt
  - phase_controller.txt
  - TC_CellStructures.txt
  - TC_Centrosome_Pair.txt

---

**simulation_results/**  
*Created automatically the first time you run the simulation. It stores the outputs from your simulations and tests.*

---

**atomics/**  
*DEVS / JSON specs for the atomic models.*

- CentrosomeModel.json
- ChromosomeModel.json
- NuclearEnvelopeModel.json
- NucleolusModel.json
- PhaseController.json
- SpindleAppartusModel.json
- StatusChecker.json

---

**docs/**  
*Assignment and model form documentation.*

**.github/workflows/**  
*CI configuration (e.g. release.yml).*

---

# STEPS

**0 – Model documentation**  
The **docs/** folder contains the explanation of this model (e.g. assignment and model form docs).

**1 – Update include path**  
Update the Cadmium include path in the Makefile (and in build_sim.sh if you use it).

- In the **Makefile**, set or override:
  - `CADMIUM_PATH ?= /home/cadmium/rt_cadmium/include`
  - Example: `make CADMIUM_PATH=/path/to/cadmium/include`
- In **build_sim.sh**, edit:
  - `CADMIUM_PATH="/home/cadmium/rt_cadmium/include"`
  - Use the path to your `cadmium/include` (or the folder that contains the Cadmium headers).

**2 – Compile and run**  
Open a terminal (e.g. Ubuntu terminal on Linux, Cygwin on Windows) in the project root (Cadmium-Mitosis folder).

- To **build both** the simulation and test executables (no run):  
  `make all`  
  Executables are created in the **build/** folder: `build/simulation` and `build/run_tests`.
- To **build and run the simulation**:  
  `make simulation`  
  Builds (if needed) and runs `./build/simulation`.
- To **build and run the tests**:  
  `make tests`  
  Builds (if needed) and runs `./build/run_tests`.
- To **remove** the built executables:  
  `make clean`

**3 – Run the tests**  
From the project root you can run the tests with:

```bash
make tests
```

Or, if you already ran `make all`, run the test executable directly:

```bash
./build/run_tests
```

To inspect test output, go to the **simulation_results** folder and open the generated files (e.g. per-model `*_results.txt` and the full log).

**4 – Run the top model (simulation)**  
From the project root you can run the simulation with:

```bash
make simulation
```

Or, if you already ran `make all`, run the simulation executable directly:

```bash
./build/simulation
```

The simulation reads the start trigger from **input_data/coupled_models/start.txt**. Output is written to **simulation_results/** (e.g. simulation_output.txt and per-model `*_results.txt`).
