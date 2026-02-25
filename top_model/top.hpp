#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../main/include/phase_controller.hpp"

using namespace cadmium;

class top_model : public Coupled {
public:
    top_model(const std::string& id) : Coupled(id) {
        auto input_reader = addComponent<lib::IEStream<std::string>>("input_reader", "input_data/phase_controller_data.txt");
        auto phase_controller = addComponent<PhaseController>("phase_controller");

        addCoupling(input_reader->out, phase_controller->permission);
    }
};
