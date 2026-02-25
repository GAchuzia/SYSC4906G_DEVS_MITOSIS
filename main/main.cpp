#include <iostream>
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "../top_model/top.hpp"

using namespace cadmium;

int main() {
    auto model = std::make_shared<top_model>("top");

    auto rootCoordinator = RootCoordinator(model);

    rootCoordinator.setLogger<STDOUTLogger>(";");

    // Run the simulation for 50 time units
    rootCoordinator.start();
    rootCoordinator.simulate(50.0);
    rootCoordinator.stop();

    return 0;
}