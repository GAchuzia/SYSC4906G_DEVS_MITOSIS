#include "test.hpp"

int main() {

    // Test 1: Centrosome
    run_test<Centrosome>("Centrosome_Test", "input_data/atomic_models/TC_Centrosome1.txt");

    // Test 2: Nucleolus
    run_test<Nucleolus>("Nucleolus_Test", "input_data/atomic_models/TC_Nucleolus1.txt");

    // Chromosome tests (TC-C1 Prophase trigger, TC-C2 full progression, TC-C3 invalid jump)
    run_test<Chromosome>("Chromosome_TC1_TC2", "input_data/atomic_models/TC_Chromosome1.txt");
    run_test<Chromosome>("Chromosome_TC3_InvalidJump", "input_data/atomic_models/TC_Chromosome2.txt");

    // Nuclear Envelope tests (TC-NE1, TC-NE2)
    run_test<NuclearEnvelope>("NuclearEnvelope_TC1_Prophase", "input_data/atomic_models/TC_Nuclear_Envelope1.txt");
    run_test<NuclearEnvelope>("NuclearEnvelope_TC2_Telophase", "input_data/atomic_models/TC_Nuclear_Envelope2.txt");

    // Test 3: StatusChecker
    // run_test<StatusChecker>("Status_Test", "input_data/atomic_models/TC_StatusChecker1.txt");

    // Test 4: PhaseController 
    run_phase_controller_test<PhaseController>("PhaseController_Test", "input_data/atomic_models/TC_PhaseController.txt");

    // COUPLED TESTS
    run_phase_control_test<PhaseControl>(
        "PhaseControl_Test",
        "input_data/coupled_models/start.txt",
        "input_data/coupled_models/chrom_input.txt",
        "input_data/coupled_models/ne_input.txt",
        "input_data/coupled_models/sp_input.txt",
        "input_data/coupled_models/cen_input.txt",
        "input_data/coupled_models/nucleo_input.txt"
    );

    run_test<CentrosomePair>("CentrosomePair_Test", "input_data/coupled_models/TC_Centrosome_Pair.txt");
    
    run_test<CellStructures>( "CellStructures_Test", "input_data/coupled_models/TC_CellStructures.txt" );

    run_phase_control_test<StatusChecker>(
        "StatusChecker_Test",
        "input_data/coupled_models/start.txt",
        "input_data/coupled_models/chrom_input.txt",
        "input_data/coupled_models/ne_input.txt",
        "input_data/coupled_models/sp_input.txt",
        "input_data/coupled_models/cen_input.txt",
        "input_data/coupled_models/nucleo_input.txt"
    );

    return 0;
}