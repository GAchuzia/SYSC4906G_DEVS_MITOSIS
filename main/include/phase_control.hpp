#ifndef PHASE_CONTROL_HPP
#define PHASE_CONTROL_HPP

#include <cadmium/modeling/devs/coupled.hpp>
#include "phase_controller.hpp"
#include "status_checker.hpp"

#include <string>

using namespace cadmium;

struct PhaseControl : public Coupled {

    // Input from the top-level trigger file
    Port<std::string> start_in;

    // Inputs from cell structures
    Port<std::string> chrom_status;
    Port<std::string> ne_status;
    Port<std::string> sp_status;
    Port<std::string> cen_status;
    Port<std::string> nucleo_status;

    // Output to cell structures 
    Port<std::string> phase_event;

    PhaseControl(const std::string& id) : Coupled(id) {
        start_in      = addInPort<std::string>("start_in");
        chrom_status  = addInPort<std::string>("chrom_status");
        ne_status     = addInPort<std::string>("ne_status");
        sp_status     = addInPort<std::string>("sp_status");
        cen_status    = addInPort<std::string>("cen_status");
        nucleo_status = addInPort<std::string>("nucleo_status");

        phase_event   = addOutPort<std::string>("phase_event");

        auto statusChecker = addComponent<StatusChecker>("statusChecker");
        auto phaseController = addComponent<PhaseController>("phaseController");

        // Coupling the trigger
        addCoupling(start_in, statusChecker->start_in);

        // Feedback couplings
        addCoupling(chrom_status,  statusChecker->chrom_status);
        addCoupling(ne_status,     statusChecker->ne_status);
        addCoupling(sp_status,     statusChecker->sp_status);
        addCoupling(cen_status,    statusChecker->cen_status);
        addCoupling(nucleo_status, statusChecker->nucleo_status);

        addCoupling(statusChecker->transition, phaseController->permission);
        addCoupling(phaseController->phase_event, phase_event);
    }
};
#endif