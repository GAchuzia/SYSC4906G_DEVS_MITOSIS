#ifndef CELL_STRUCTURES_HPP
#define CELL_STRUCTURES_HPP

#include <cadmium/modeling/devs/coupled.hpp>

#include "chromosome.hpp"       
#include "nuclear_envelope.hpp"
#include "spindle.hpp"
#include "centrosome_pair.hpp"
#include "nucleolus.hpp"

using namespace cadmium;

class CellStructures : public Coupled {
public:
    // Input
    Port<std::string> phase_in;

    // Outputs
    Port<std::string> out_chrom, out_ne, out_sp, out_cen, out_nucleo;

    CellStructures(const std::string& id) : Coupled(id) {

        // Input
        phase_in = addInPort<std::string>("phase_in");

        // Outputs
        out_chrom  = addOutPort<std::string>("out_chrom");
        out_ne = addOutPort<std::string>("out_ne");
        out_sp = addOutPort<std::string>("out_sp");
        out_cen = addOutPort<std::string>("out_cen");
        out_nucleo = addOutPort<std::string>("out_nucleo");

        // Components
        auto chrom  = addComponent<Chromosome>("chrom");
        auto ne = addComponent<NuclearEnvelope>("ne");
        auto sp = addComponent<Spindle>("sp");
        auto cen = addComponent<CentrosomePair>("cen");
        auto nucleo = addComponent<Nucleolus>("nucleo");

        // EIC phase_event into all substructures
        addCoupling(phase_in, chrom->phase_in);
        addCoupling(phase_in, ne->phase_in);
        addCoupling(phase_in, sp->phase_in);
        addCoupling(phase_in, cen->phase_in);
        addCoupling(phase_in, nucleo->phase_in);

        // EOC route internal outputs outward
        addCoupling(chrom->status_out, out_chrom);
        addCoupling(ne->status_out, out_ne);
        addCoupling(sp->status_out, out_sp);
        addCoupling(cen->status_out, out_cen);
        addCoupling(nucleo->status_out, out_nucleo);
    }
};
#endif