#ifndef TOP_HPP
#define TOP_HPP

#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../main/include/phase_control.hpp"
#include "../main/include/cell_structures.hpp"

using namespace cadmium;

/**
 * Mitosis System Coupled Model
 */
class mitosis_system : public Coupled {
public:
    Port<std::string> start_trigger;

    mitosis_system(const std::string& id) : Coupled(id) {
        start_trigger = addInPort<std::string>("start_trigger");

        auto phaseControl   = addComponent<PhaseControl>("phaseControl");
        auto cellStructures = addComponent<CellStructures>("cellStructures");

        // EIC
        addCoupling(start_trigger, phaseControl->start_in);

        // IC (PhaseControl -> CellStructures)
        addCoupling(phaseControl->phase_event, cellStructures->phase_in);

        // IC (CellStructures -> PhaseControl)
        addCoupling(cellStructures->out_chrom,  phaseControl->chrom_status);
        addCoupling(cellStructures->out_ne,     phaseControl->ne_status);
        addCoupling(cellStructures->out_sp,     phaseControl->sp_status);
        addCoupling(cellStructures->out_cen,    phaseControl->cen_status);
        addCoupling(cellStructures->out_nucleo, phaseControl->nucleo_status);
    }
};

/**
 * Top Model
 */
class top_model : public Coupled {
public:
    top_model(const std::string& id) : Coupled(id) {
        // Read from input start text file
        auto start_reader = addComponent<lib::IEStream<std::string>>("start_reader", "input_data/start.txt");
        
        // mitosis_system component
        auto system = addComponent<mitosis_system>("mitosis_system");

        // Coupling the reader output to the system's trigger port
        addCoupling(start_reader->out, system->start_trigger);
    }
};

#endif