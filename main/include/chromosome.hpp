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

    void internalTransition(ChromosomeState& state) const override {
        if (state.state == "decondensing") {
            state.state = "completed";
        }
        state.active = false;
    }

    void externalTransition(ChromosomeState& state, double e) const override {
        for (const auto& msg : phase_in->getBag()) {
            if (msg == "Prophase" && state.state == "uncondensed") {
                state.state = "condensed";
                state.active = true;
            } else if (msg == "Metaphase" && state.state == "condensed") {
                state.state = "aligned";
                state.active = true;
            } else if (msg == "Anaphase" && state.state == "aligned") {
                state.state = "separating";
                state.active = true;
            } else if (msg == "Telophase" && state.state == "separating") {
                state.state = "decondensing";
                state.active = true;
            }
        }
    }

    void output(const ChromosomeState& state) const override {
        if (state.state == "completed") {
            status_out->addMessage("ready");
        } else {
            status_out->addMessage("not_ready");
        }
    }

    double timeAdvance(const ChromosomeState& state) const override {
        if (state.active) {
            if (state.state == "condensed")   return 1.0;  // t1
            if (state.state == "aligned")    return 1.0;  // t2
            if (state.state == "separating")  return 1.0;  // t3
            if (state.state == "decondensing") return 1.0; // t4
        }
        return std::numeric_limits<double>::infinity();
    }
};

#endif
