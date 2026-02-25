#include "test.hpp"

int main() {

    // Test 1: Centrosome
    run_test<Centrosome>("Centrosome_Test", "input_data/atomic_models/TC_Centrosome1.txt");

    // Test 2: Nucleolus
    run_test<Nucleolus>("Nucleolus_Test", "input_data/atomic_models/TC_Nucleolus1.txt");

    // Test 3: StatusChecker 
    // run_test<StatusChecker>("Status_Test", "input_data/atomic_models/TC_StatusChecker1.txt");

    return 0;
}