#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../main/include/cell_structures.hpp"
#include "../main/include/status_checker.hpp"


class MitosisSimulation : public Coupled {
public:
    MitosisSimulation(const std::string& id) : Coupled(id) {
        auto structures = addComponent<CellStructures>("structures");
        auto checker    = addComponent<StatusChecker>("checker");

        // Direct coupling between the bundled container and the checker
        addCoupling(structures->out_chrom,  checker->chrom_status);
        addCoupling(structures->out_ne,     checker->ne_status);
        addCoupling(structures->out_sp,     checker->sp_status);
        addCoupling(structures->out_cen,    checker->cen_status);
        addCoupling(structures->out_nucleo, checker->nucleo_status);
    }
};