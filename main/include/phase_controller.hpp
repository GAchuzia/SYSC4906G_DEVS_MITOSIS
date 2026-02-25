#ifndef PHASECONTROLLER_HPP
#define PHASECONTROLLER_HPP

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>
#include <limits>

using namespace cadmium;

struct PhaseControllerState {
    std::string state;
    double sigma;  // time until next internal transition
};

inline std::ostream& operator<<(std::ostream& os, const PhaseControllerState& s) {
    os << s.state;
    return os;
}

class PhaseController : public Atomic<PhaseControllerState> {
public:
    
    Port<std::string> permission;
    Port<std::string> phase_event;

    PhaseController(const std::string& id)
        : Atomic<PhaseControllerState>(id, {"Interphase", std::numeric_limits<double>::infinity()}) 
    {
        permission = addInPort<std::string>("permission");
        phase_event = addOutPort<std::string>("phase_event");
    }

    // Internal Transition
    void internalTransition(PhaseControllerState& state) const override {
        // After output, return to passive waiting
        state.sigma = std::numeric_limits<double>::infinity();
    }

    // External Transition 
    void externalTransition(PhaseControllerState& state, double e) const override {
        for (const auto &msg : permission->getBag()) {

            if (msg == "transition") {

                if (state.state == "Interphase")
                    state.state = "Prophase";
                else if (state.state == "Prophase")
                    state.state = "Metaphase";
                else if (state.state == "Metaphase")
                    state.state = "Anaphase";
                else if (state.state == "Anaphase")
                    state.state = "Telophase";
                else if (state.state == "Telophase")
                    state.state = "Cytokinesis";
                else if (state.state == "Cytokinesis")
                    state.state = "Interphase";

                // schedule immediate output
                state.sigma = 0.0;
            }
        }
    }

    // Output
    void output(const PhaseControllerState& state) const override {
        phase_event->addMessage(state.state);
    }

    // Time Advance
    double timeAdvance(const PhaseControllerState& state) const override {
        return state.sigma;
    }
};

#endif