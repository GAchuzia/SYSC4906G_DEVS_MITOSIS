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

    void internalTransition(NuclearEnvelopeState& state) const override {
        if (state.state == "disassembling") {
            state.state = "absent";
        } else if (state.state == "reforming") {
            state.state = "restored";
        }
        state.active = false;
    }

    void externalTransition(NuclearEnvelopeState& state, double e) const override {
        for (const auto& msg : phase_in->getBag()) {
            if (msg == "Interphase") {
                state.state = "intact";
                state.active = false;
            } else if (msg == "Prophase") {
                state.state = "disassembling";
                state.active = true;
            } else if (msg == "Metaphase" || msg == "Anaphase") {
                state.state = "absent";
                state.active = false;
            } else if (msg == "Telophase") {
                state.state = "reforming";
                state.active = true;
            } else if (msg == "Cytokinesis") {
                state.state = "restored";
                state.active = false;
            }
        }
    }

    void output(const NuclearEnvelopeState& state) const override {
        if (state.state == "restored") {
            status_out->addMessage("ready");
        } else {
            status_out->addMessage("not_ready");
        }
    }

    double timeAdvance(const NuclearEnvelopeState& state) const override {
        if (state.active) {
            if (state.state == "disassembling") return 1.0; // t1
            if (state.state == "reforming")     return 1.0; // t2
        }
        return std::numeric_limits<double>::infinity();
    }
};

#endif
