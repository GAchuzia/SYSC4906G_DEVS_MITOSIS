#ifndef TEST_HPP
#define TEST_HPP

#include <limits> 
#include <string>
#include <fstream>
#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include "cadmium/simulation/logger/stdout.hpp"

// Atomic models
#include "../main/include/centrosome.hpp"
#include "../main/include/chromosome.hpp"
#include "../main/include/nuclear_envelope.hpp"
#include "../main/include/nucleolus.hpp"
#include "../main/include/spindle.hpp" 
#include "../main/include/phase_controller.hpp"
#include "../main/include/status_checker.hpp"

// Coupled models
#include "../main/include/phase_control.hpp"
#include "../main/include/centrosome_pair.hpp"
#include "../main/include/cell_structures.hpp"

using namespace cadmium;

// Strips ANSI escape sequences (e.g. ^[[33m) and writes to both file and stdout
struct TeeBuf : std::streambuf {
    std::streambuf* a;
    std::streambuf* b;
    int state = 0;  // 0=normal, 1=after ESC, 2=in CSI
    TeeBuf(std::streambuf* a_, std::streambuf* b_) : a(a_), b(b_) {}
    void put(char c) {
        a->sputc(c);
        b->sputc(c);
    }
    int overflow(int c) override {
        if (c == EOF) return !EOF;
        char ch = static_cast<char>(c);
        if (state == 0) {
            if (c == '\033') state = 1;
            else put(ch);
        } else if (state == 1) {
            if (c == '[') state = 2;
            else { put('\033'); put(ch); state = 0; }
        } else {
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) state = 0;
        }
        return c;
    }
    int sync() override {
        return (a->pubsync() == 0 && b->pubsync() == 0) ? 0 : -1;
    }
};

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

template<typename ModelType>
struct PhaseControllerTester : public Coupled {
    PhaseControllerTester(const std::string& id, const std::string& input_path) : Coupled(id) {
        auto model = addComponent<ModelType>("model_under_test");
        auto reader = addComponent<lib::IEStream<std::string>>("reader", input_path.c_str());
        
        // Connect the reader to the model's phase_in port
        addCoupling(reader->out, model->permission);
    }
};

// Function to handle the simulation boilerplate
template<typename ModelType>
void run_test(std::string test_name, std::string file_path) {
    std::streambuf* old_cout = std::cout.rdbuf();
    std::ofstream out("tests/results/" + test_name + ".txt");
    TeeBuf tee(out.rdbuf(), old_cout);
    std::cout.rdbuf(&tee);
    std::cout << "\n========== " << test_name << " ==========\n";
    auto model = std::make_shared<AtomicTester<ModelType>>(test_name, file_path);
    auto rootCoordinator = RootCoordinator(model);
    
    rootCoordinator.setLogger<STDOUTLogger>(";");
    
    rootCoordinator.start();
    rootCoordinator.simulate(100.0);
    rootCoordinator.stop();
    std::cout.rdbuf(old_cout);
}

template<typename ModelType>
void run_phase_controller_test(std::string test_name, std::string file_path) {
    std::streambuf* old_cout = std::cout.rdbuf();
    std::ofstream out("tests/results/" + test_name + ".txt");
    TeeBuf tee(out.rdbuf(), old_cout);
    std::cout.rdbuf(&tee);
    std::cout << "\n========== " << test_name << " ==========\n";
    auto model = std::make_shared<PhaseControllerTester<ModelType>>(test_name, file_path);
    auto rootCoordinator = RootCoordinator(model);

    rootCoordinator.setLogger<STDOUTLogger>(";");
    rootCoordinator.start();
    rootCoordinator.simulate(100.0);
    rootCoordinator.stop();
    std::cout.rdbuf(old_cout);
}

template<typename ModelType>
struct PhaseControlTester : public Coupled {

    PhaseControlTester(const std::string& id,
                       const std::string& start_file,
                       const std::string& chrom_file,
                       const std::string& ne_file,
                       const std::string& sp_file,
                       const std::string& cen_file,
                       const std::string& nucleo_file)
        : Coupled(id)
    {
        auto model = addComponent<ModelType>("phase_control");

        // Input readers
        auto start_reader   = addComponent<lib::IEStream<std::string>>("start_reader", start_file.c_str());
        auto chrom_reader   = addComponent<lib::IEStream<std::string>>("chrom_reader", chrom_file.c_str());
        auto ne_reader      = addComponent<lib::IEStream<std::string>>("ne_reader", ne_file.c_str());
        auto sp_reader      = addComponent<lib::IEStream<std::string>>("sp_reader", sp_file.c_str());
        auto cen_reader     = addComponent<lib::IEStream<std::string>>("cen_reader", cen_file.c_str());
        auto nucleo_reader  = addComponent<lib::IEStream<std::string>>("nucleo_reader", nucleo_file.c_str());

        // Couplings
        addCoupling(start_reader->out,  model->start_in);
        addCoupling(chrom_reader->out,  model->chrom_status);
        addCoupling(ne_reader->out,     model->ne_status);
        addCoupling(sp_reader->out,     model->sp_status);
        addCoupling(cen_reader->out,    model->cen_status);
        addCoupling(nucleo_reader->out, model->nucleo_status);
    }
};

template<typename ModelType>
void run_phase_control_test(std::string test_name,
                            std::string start_file,
                            std::string chrom_file,
                            std::string ne_file,
                            std::string sp_file,
                            std::string cen_file,
                            std::string nucleo_file)
{
    std::streambuf* old_cout = std::cout.rdbuf();
    std::ofstream out("tests/results/" + test_name + ".txt");
    TeeBuf tee(out.rdbuf(), old_cout);
    std::cout.rdbuf(&tee);
    std::cout << "\n========== " << test_name << " ==========\n";

    auto model = std::make_shared<PhaseControlTester<ModelType>>(
        test_name,
        start_file,
        chrom_file,
        ne_file,
        sp_file,
        cen_file,
        nucleo_file
    );

    auto rootCoordinator = RootCoordinator(model);

    rootCoordinator.setLogger<STDOUTLogger>(";");
    rootCoordinator.start();
    rootCoordinator.simulate(100.0);
    rootCoordinator.stop();
    std::cout.rdbuf(old_cout);
}

#endif