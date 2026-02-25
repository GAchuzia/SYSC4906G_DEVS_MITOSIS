#ifndef PHASE_CONTROL_HPP
#define PHASE_CONTROL_HPP

#include <cadmium/modeling/devs/coupled.hpp>
#include "phase_controller.hpp" // phase controller atomic model
#include "status_checker.hpp" // status checker atomic model

#include <string>

using namespace cadmium;

struct PhaseControl : public Coupled {

    // inputs from cell structs
    Port<std::string> chrom_status;
    Port<std::string> ne_status;
    Port<std::string> sp_status;
    Port<std::string> cen_status;
    Port<std::string> nucleo_status;

    // output to cell structs 
    Port<std::string> phase_event;

    PhaseControl(const std::string& id) : Coupled(id) {

        // input ports
        chrom_status  = addInPort<std::string>("chrom_status");
        ne_status     = addInPort<std::string>("ne_status");
        sp_status     = addInPort<std::string>("sp_status");
        cen_status    = addInPort<std::string>("cen_status");
        nucleo_status = addInPort<std::string>("nucleo_status");

        // output ports
        phase_event = addOutPort<std::string>("phase_event");

        auto statusChecker = addComponent<StatusChecker>("statusChecker");
        auto phaseController = addComponent<PhaseController>("phaseController");

        // external couplings
        addCoupling(chrom_status,  statusChecker->chrom_status);
        addCoupling(ne_status,     statusChecker->ne_status);
        addCoupling(sp_status,     statusChecker->sp_status);
        addCoupling(cen_status,    statusChecker->cen_status);
        addCoupling(nucleo_status, statusChecker->nucleo_status);

        // internal coupling 
        addCoupling(statusChecker->transition,
                    phaseController->permission);

        addCoupling(phaseController->phase_event,
                    phase_event);
    }

};
#endif