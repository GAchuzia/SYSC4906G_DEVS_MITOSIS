#ifndef SPINDLE_HPP
#define SPINDLE_HPP

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>

using namespace cadmium;
using namespace std;

struct SpindleState {
    string state;
    bool active;
};

inline ostream& operator<<(ostream& os, const SpindleState& s) {
    os << s.state;
    return os;
}

class Spindle : public Atomic<SpindleState> {
public:
    
    Port<std::string> phase_in;
    Port<std::string> status_out;
    

    Spindle(const std::string& id) : Atomic<SpindleState>(id, {"stable", false}) {
        phase_in = addInPort<std::string>("phase_in");
        status_out = addOutPort<std::string>("status_out");
    }


    // Internal Transition
     void internalTransition(SpindleState& state) const override {
        state.active = false;
    }

    // External Transition
    void externalTransition(SpindleState& state, double e) const override {
        for (const auto &msg : phase_in->getBag()) {       
            if (msg == "Prophase") {
                state.state = "forming";
                state.active = true;  
            }
            else if (msg == "Metaphase") {
                state.state = "attached";
                state.active = true;
            }
            else if (msg == "Anaphase") {
                state.state = "pulling";
                state.active = true;
            }
            else if (msg == "Telophase") {
                state.state = "disassembling";
                state.active = true; 
            }
            else if (msg == "Cytokinesis") {
                state.state = "inactive";
                state.active = true;
            }
        }
    }

    // Output
    void output(const SpindleState& state) const override {
        if (state.state == "inactive") {
            status_out->addMessage("ready");
        } else {
            status_out->addMessage("not_ready");
        }
    }


    // Time advance
    double timeAdvance(const SpindleState& state) const override {
        if (state.active){
            if (state.state == "forming")
                return 2.0;  // t1

            if (state.state == "attached")
                return 3.0;  // t2
            
            if (state.state == "pulling")
                return 3.0;  // t3
            
            if (state.state == "disassembling")
                return 3.0;  // t4

            if (state.state == "inactive")
                return 3.0;  // t5
        }
        return std::numeric_limits<double>::infinity();
    }
};

#endif