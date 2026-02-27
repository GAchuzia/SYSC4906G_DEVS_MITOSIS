#ifndef CHROMOSOME_HPP
#define CHROMOSOME_HPP

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>
#include <limits>

using namespace cadmium;

struct ChromosomeState {
    std::string state;
    bool active;
};

inline std::ostream& operator<<(std::ostream& os, const ChromosomeState& s) {
    os << s.state;
    return os;
}

class Chromosome : public Atomic<ChromosomeState> {
public:
    Port<std::string> phase_in;
    Port<std::string> status_out;

    Chromosome(const std::string& id) : Atomic<ChromosomeState>(id, {"uncondensed", false}) {
        phase_in = addInPort<std::string>("phase_in");
        status_out = addOutPort<std::string>("status_out");
    }

    // Internal Transition
    void internalTransition(ChromosomeState& state) const override {
        if (state.state == "condensing") state.state = "condensed";
        else if (state.state == "aligning") state.state = "aligned";
        else if (state.state == "separating") state.state = "separated";
        else if (state.state == "decondensing") state.state = "uncondensed";
        state.active = false;
    }

    // External Transition
    void externalTransition(ChromosomeState& state, double e) const override {
        for (const auto& msg : phase_in->getBag()) {
            if (msg == "Prophase") {
                state.state = "condensing";
                state.active = true;
            } else if (msg == "Metaphase") {
                state.state = "aligning";
                state.active = true;
            } else if (msg == "Anaphase") {
                state.state = "separating";
                state.active = true;
            } else if (msg == "Telophase" || msg == "Cytokinesis") {
                state.state = "decondensing";
                state.active = true;
            }
        }
    }

    // Output
    void output(const ChromosomeState& state) const override {
        if (state.state == "condensing" || state.state == "aligning" || 
            state.state == "separating" || state.state == "decondensing") {
            status_out->addMessage("ready");
        } else {
            status_out->addMessage("not_ready");
        }
    }

    double timeAdvance(const ChromosomeState& state) const override {
        if (state.active) {
            if (state.state == "condensing")   return 1.0; 
            if (state.state == "aligning")     return 1.0; 
            if (state.state == "separating")   return 1.0; 
            if (state.state == "decondensing") return 1.0;
        }
        return std::numeric_limits<double>::infinity();
    }
};

#endif
