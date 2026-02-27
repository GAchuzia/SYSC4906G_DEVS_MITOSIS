#ifndef CENTROSOME_HPP
#define CENTROSOME_HPP

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>

using namespace cadmium;

struct CentrosomeState {
    std::string state;
    bool active;
};

inline std::ostream& operator<<(std::ostream& os, const CentrosomeState& s) {
    os << s.state;
    return os;
}

class Centrosome : public Atomic<CentrosomeState> {
public:
    
    Port<std::string> phase_in;
    Port<std::string> status_out;

    Centrosome(const std::string& id) : Atomic<CentrosomeState>(id, {"stable", false}) {
        phase_in = addInPort<std::string>("phase_in");
        status_out = addOutPort<std::string>("status_out");
    }

    // Internal Transition
    void internalTransition(CentrosomeState& state) const override {
        state.active = false;
    }

    // External Transition 
    void externalTransition(CentrosomeState& state, double e) const override {
        for (const auto &msg : phase_in->getBag()) {
            if (msg == "Interphase") {
                 state.state = "stable";
                  state.active = false; 
            }
            else if (msg == "Prophase") { 
                state.state = "duplicated";
                 state.active = true; 
            }
            else if (msg == "Metaphase") { 
                state.state = "migrating";
                 state.active = true; 
            }
            else if (msg == "Anaphase" || msg == "Telophase" || msg == "Cytokinesis") { 
                state.state = "at_poles"; 
                state.active = true; 
            }
        }
    }

    // Output
    void output(const CentrosomeState& state) const override {
        if (state.state == "at_poles" || state.state == "stable" || state.state == "duplicated" || state.state == "migrating") {
            status_out->addMessage("ready");
        } else {
            status_out->addMessage("not_ready");
        }
    }

    // Time Advance
    double timeAdvance(const CentrosomeState& state) const override {
        if (state.active) {
            if (state.state == "duplicated") return 2.0; 
            if (state.state == "migrating")  return 3.0;
            if (state.state == "at_poles")   return 0.0; // Trigger output immediately
        }
        return std::numeric_limits<double>::infinity();
    }
};

#endif