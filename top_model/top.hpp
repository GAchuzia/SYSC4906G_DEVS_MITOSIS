#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../main/include/phase_control.hpp"

using namespace cadmium;

class top_model : public Coupled {

public:
    top_model(const std::string& id) : Coupled(id) {

        auto chrom_reader  = addComponent<lib::IEStream<std::string>>(
            "chrom_reader", "input_data/chrom_input.txt");

        auto ne_reader     = addComponent<lib::IEStream<std::string>>(
            "ne_reader", "input_data/ne_input.txt");

        auto sp_reader     = addComponent<lib::IEStream<std::string>>(
            "sp_reader", "input_data/sp_input.txt");

        auto cen_reader    = addComponent<lib::IEStream<std::string>>(
            "cen_reader", "input_data/cen_input.txt");

        auto nucleo_reader = addComponent<lib::IEStream<std::string>>(
            "nucleo_reader", "input_data/nucleo_input.txt");

        auto phaseControl = addComponent<PhaseControl>("phaseControl");

        addCoupling(chrom_reader->out,  phaseControl->chrom_status);
        addCoupling(ne_reader->out,     phaseControl->ne_status);
        addCoupling(sp_reader->out,     phaseControl->sp_status);
        addCoupling(cen_reader->out,    phaseControl->cen_status);
        addCoupling(nucleo_reader->out, phaseControl->nucleo_status);
    }
};