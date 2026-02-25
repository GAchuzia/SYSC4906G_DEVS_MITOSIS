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

    return 0;
}