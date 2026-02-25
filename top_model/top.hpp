#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../main/include/centrosome.hpp"

using namespace cadmium;

class top_model : public Coupled {
public:
    top_model(const std::string& id) : Coupled(id) {
        auto input_reader = addComponent<lib::IEStream<std::string>>("input_reader", "input.txt");
        auto centrosome = addComponent<Centrosome>("centrosome");

        addCoupling(input_reader->out, centrosome->phase_in);
    }
};
