#ifndef NUCLEAR_ENVELOPE_HPP
#define NUCLEAR_ENVELOPE_HPP

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>
#include <limits>

using namespace cadmium;

struct NuclearEnvelopeState {
    std::string state;
    bool active;
};

inline std::ostream& operator<<(std::ostream& os, const NuclearEnvelopeState& s) {
    os << s.state;
    return os;
}

class NuclearEnvelope : public Atomic<NuclearEnvelopeState> {
public:
    Port<std::string> phase_in;
    Port<std::string> status_out;

    NuclearEnvelope(const std::string& id) : Atomic<NuclearEnvelopeState>(id, {"intact", false}) {
        phase_in = addInPort<std::string>("phase_in");
        status_out = addOutPort<std::string>("status_out");
    }

    // Internal Transition
    void internalTransition(NuclearEnvelopeState& state) const override {
        if (state.state == "disassembling") state.state = "absent";
        else if (state.state == "absent_ack") state.state = "absent";
        else if (state.state == "reforming") state.state = "restored";
    }

    // External Transition
    void externalTransition(NuclearEnvelopeState& state, double e) const override {
        for (const auto& msg : phase_in->getBag()) {
            if (msg == "Prophase") {
                state.state = "disassembling";
                state.active = true;
            } else if (msg == "Metaphase" || msg == "Anaphase") {
                // Temporary state to acknowledge the phase and trigger output
                state.state = "absent_ack"; 
                state.active = true;
            } else if (msg == "Telophase" || msg == "Cytokinesis") {
                state.state = "reforming";
                state.active = true;
            }
        }
    }

    // Output
    void output(const NuclearEnvelopeState& state) const override {
    if (state.state == "disassembling" || state.state == "reforming" || state.state == "absent_ack") {
            status_out->addMessage("ready");
        } else {
            status_out->addMessage("not_ready");
        }
    }

    // Time Advance
    double timeAdvance(const NuclearEnvelopeState& state) const override {
        if (state.active) {
            if (state.state == "disassembling") return 1.0; 
            if (state.state == "reforming")     return 1.0; 
            if (state.state == "absent_ack")    return 0.1; 
        }        
        return std::numeric_limits<double>::infinity();
    }
};

#endif
