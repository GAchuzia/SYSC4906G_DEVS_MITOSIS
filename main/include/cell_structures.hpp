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
    // Bundled Outputs
    Port<std::string> out_chrom, out_ne, out_sp, out_cen, out_nucleo;

    CellStructures(const std::string& id) : Coupled(id) {
        out_chrom  = addOutPort<std::string>("out_chrom");
        out_ne     = addOutPort<std::string>("out_ne");
        out_sp     = addOutPort<std::string>("out_sp");
        out_cen    = addOutPort<std::string>("out_cen");
        out_nucleo = addOutPort<std::string>("out_nucleo");

        // Instantiate internal atomic models
        auto chrom  = addComponent<Chromosome>("chrom");
        auto ne     = addComponent<NuclearEnvelope>("ne");
        auto sp     = addComponent<Spindle>("sp");
        auto cen    = addComponent<CentrosomePair>("cen");
        auto nucleo = addComponent<Nucleolus>("nucleo");

        // Route internal atomic outputs to  (EOC)
        addCoupling(chrom->status_out,  out_chrom);
        addCoupling(ne->status_out,     out_ne);
        addCoupling(sp->status_out,     out_sp);
        addCoupling(cen->status_out,    out_cen);
        addCoupling(nucleo->status_out, out_nucleo);
    }
};
#endif