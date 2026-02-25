#ifndef NUCLEOLUS_HPP 
#define NUCLEOLUS_HPP


#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>
#include <limits>

using namespace cadmium;
using namespace std;

struct NucleolusState {
    string state;
    bool active;
};

inline ostream& operator<<(ostream& os, const NucleolusState& s) {
    os << s.state;
    return os;
}

class Nucleolus : public Atomic<NucleolusState> {
public:
    
    Port<std::string> phase_in;
    Port<std::string> status_out;


    Nucleolus(const std::string& id) : Atomic<NucleolusState>(id, {"stable", false}) {
        phase_in = addInPort<std::string>("phase_in");
        status_out = addOutPort<std::string>("status_out");
    }


    // Internal Transition
    void internalTransition(NucleolusState& state) const override {
        state.active = false;
    }

    // External Transition
    void externalTransition(NucleolusState& state, double e) const override {
        for (const auto &msg : phase_in->getBag()) {
            if (msg == "Interphase" || msg == "Cytokinesis") {
                state.state = "visible";
                state.active = false;
            }
            else if (msg == "Prophase") {
                state.state = "disappeared";
                state.active = true;   // transition process
            }
            else if (msg == "Telophase") {
                state.state = "reappearing";
                state.active = true;   // transition process
            }
        }
    }

    // Output
    void output(const NucleolusState& state) const override {
        if (state.state == "visible") {
            status_out->addMessage("ready");
        } else {
            status_out->addMessage("not_ready");
        }
    }


    // Time Advance
    double timeAdvance(const NucleolusState& state) const override {
        if (state.active){
            if (state.state == "disappeared")
                return 2.0;  // t1

            if (state.state == "reappearing")
                return 3.0;  // t2

            // if (state.state == "visible")
            //     return 0.0; // Trigger output immediately
        }
        return std::numeric_limits<double>::infinity();
    }
};

#endif