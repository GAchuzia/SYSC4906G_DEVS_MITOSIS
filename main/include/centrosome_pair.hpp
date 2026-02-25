#ifndef CENTROSOME_PAIR_HPP
#define CENTROSOME_PAIR_HPP

#include <cadmium/modeling/devs/coupled.hpp>
#include "centrosome.hpp" // Centrosome atomic model

using namespace cadmium;

struct CentrosomePair : public Coupled {

    Port<string> phase_in;
    Port<string> phase_out;

    CentrosomePair(const std::string& id) : Coupled(id) {
        phase_in = addInPort<string>("phase_in");
        status_out = addOutPort<string>("status_out");

        auto centrosome_Top = addComponent<Centrosome>("centrosome_Top");
        auto centrosome_Bottom = addComponent<Centrosome>("centrosome_Bottom");

        addCoupling(phase_in, centrosome_Top->phase_in);
        addCoupling(phase_in, centrosome_Bottom->phase_in);

        addCoupling(centrosome_Top->status_out, status_out);
        addCoupling(centrosome_Bottom->status_out, status_out);
    }

};
#endif