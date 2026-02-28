#ifndef MITOSIS_SYSTEM_HPP
#define MITOSIS_SYSTEM_HPP

#include <cadmium/modeling/devs/coupled.hpp>

#include "phase_control.hpp"
#include "cell_structures.hpp"

using namespace cadmium;

struct MitosisSystem : public Coupled {

    // ports
    Port<std::string> mitosisStart;
    Port<std::string> mitosisCompleted;

    MitosisSystem(const std::string& id) : Coupled(id) {

        mitosisStart = addInPort<std::string>("mitosisStart"); // input  
        mitosisCompleted = addOutPort<std::string>("mitosisCompleted"); // output

        auto phaseControl = addComponent<PhaseControl>("phaseControl");
        auto cellStructures = addComponent<CellStructures>("cellStructures");

        addCoupling(mitosisStart, phaseControl->phase_event); //EIC

        // IC cell structure to phasecontrol
        addCoupling(cellStructures->out_chrom, phaseControl->chrom_status); 
        addCoupling(cellStructures->out_ne, phaseControl->ne_status);
        addCoupling(cellStructures->out_sp, phaseControl->sp_status);
        addCoupling(cellStructures->out_cen, phaseControl->cen_status);
        addCoupling(cellStructures->out_nucleo, phaseControl->nucleo_status);

        // phase control to cell structures
        addCoupling(phaseControl->phase_event, cellStructures->phase_in);

        addCoupling(phaseControl->phase_event, mitosisCompleted); // EOC
        // this would output every step of mitosis so maybe renaming mitosisCompleted to something else is good
    }
};

#endif

