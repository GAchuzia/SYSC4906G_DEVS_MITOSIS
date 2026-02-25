#ifndef TEST_HPP
#define TEST_HPP

#include <limits> 
#include <string>
#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include "cadmium/simulation/logger/stdout.hpp"

// Atomic atomic models
#include "../main/include/centrosome.hpp"
#include "../main/include/nucleolus.hpp"
#include "../main/include/spindle.hpp" 

using namespace cadmium;

// A generic Wrapper to connect any Atomic Model to an Input Stream
template<typename ModelType>
struct AtomicTester : public Coupled {
    AtomicTester(const std::string& id, const std::string& input_path) : Coupled(id) {
        auto model = addComponent<ModelType>("model_under_test");
        auto reader = addComponent<lib::IEStream<std::string>>("reader", input_path.c_str());
        
        // Connect the reader to the model's phase_in port
        addCoupling(reader->out, model->phase_in);
    }
};

// Function to handle the simulation boilerplate
template<typename ModelType>
void run_test(std::string test_name, std::string file_path) {
    auto model = std::make_shared<AtomicTester<ModelType>>(test_name, file_path);
    auto rootCoordinator = RootCoordinator(model);
    
    rootCoordinator.setLogger<STDOUTLogger>(";");
    
    rootCoordinator.start();
    rootCoordinator.simulate(100.0);
    rootCoordinator.stop();
}

#endif