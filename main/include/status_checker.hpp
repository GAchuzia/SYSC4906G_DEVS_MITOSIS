#ifndef STATUSCHECKER_HPP
#define STATUSCHECKER_HPP

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>
#include <limits>
#include <algorithm>

using namespace cadmium;

struct StatusCheckerState {
    std::string phase;

    bool chrom_ready = false;
    bool ne_ready = false;
    bool sp_ready = false;
    bool cen_ready = false;
    bool nucleo_ready = false;

    double sigma = std::numeric_limits<double>::infinity();
};

inline std::ostream& operator<<(std::ostream& os, const StatusCheckerState& s) {
    os << s.phase;
    return os;
}

class StatusChecker : public Atomic<StatusCheckerState> {
public:
    // input ports
    Port<std::string> chrom_status;
    Port<std::string> ne_status;
    Port<std::string> sp_status;
    Port<std::string> cen_status;
    Port<std::string> nucleo_status;

    // output ports
    Port<std::string> transition;

    StatusChecker(const std::string& id)
        : Atomic<StatusCheckerState>(id, StatusCheckerState{})   // default state
    {
        // initialize starting phase explicitly
        state.phase = "Interphase";
        state.sigma = std::numeric_limits<double>::infinity();

        chrom_status  = addInPort<std::string>("chrom_status");
        ne_status     = addInPort<std::string>("ne_status");
        sp_status     = addInPort<std::string>("sp_status");
        cen_status    = addInPort<std::string>("cen_status");
        nucleo_status = addInPort<std::string>("nucleo_status");

        transition = addOutPort<std::string>("transition");
    }

    void internalTransition(StatusCheckerState& s) const override {
        // reset readiness after decision
        s.chrom_ready = false;
        s.ne_ready = false;
        s.sp_ready = false;
        s.cen_ready = false;
        s.nucleo_ready = false;

        s.sigma = std::numeric_limits<double>::infinity();
    }

    void externalTransition(StatusCheckerState& s, double e) const override {
        // account for elapsed time (optional, but good practice)
        if (s.sigma != std::numeric_limits<double>::infinity()) {
            s.sigma = std::max(0.0, s.sigma - e);
        }

        for (const auto& msg : chrom_status->getBag())  s.chrom_ready  = (msg == "ready");
        for (const auto& msg : ne_status->getBag())     s.ne_ready     = (msg == "ready");
        for (const auto& msg : sp_status->getBag())     s.sp_ready     = (msg == "ready");
        for (const auto& msg : cen_status->getBag())    s.cen_ready    = (msg == "ready");
        for (const auto& msg : nucleo_status->getBag()) s.nucleo_ready = (msg == "ready");

        bool all_ready = s.chrom_ready && s.ne_ready && s.sp_ready && s.cen_ready && s.nucleo_ready;

        if (all_ready) {
            if (s.phase == "Interphase")      s.phase = "Prophase";
            else if (s.phase == "Prophase")   s.phase = "Metaphase";
            else if (s.phase == "Metaphase")  s.phase = "Anaphase";
            else if (s.phase == "Anaphase")   s.phase = "Telophase";
            else if (s.phase == "Telophase")  s.phase = "Cytokinesis";
            else if (s.phase == "Cytokinesis")s.phase = "Interphase";

            // schedule immediate output
            s.sigma = 0.0;
        }
    }

    void output(const StatusCheckerState& s) const override {
        bool all_ready = s.chrom_ready && s.ne_ready && s.sp_ready && s.cen_ready && s.nucleo_ready;

        if (all_ready) {
            transition->addMessage("allow_transition");
        } else {
            transition->addMessage("deny_transition");
        }
    }

    double timeAdvance(const StatusCheckerState& s) const override {
        return s.sigma;
    }
};

#endif