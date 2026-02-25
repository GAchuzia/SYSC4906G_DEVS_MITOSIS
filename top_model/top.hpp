#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../main/include/centrosome.hpp"
#include "../main/include/chromosome.hpp"
#include "../main/include/nuclear_envelope.hpp"
#include "../main/include/nucleolus.hpp"

using namespace cadmium;

class top_model : public Coupled {
    // Atomic + phase input reader, coupled reader->out -> model->phase_in.
    template<typename AtomicT>
    void add_atomic_reader(const std::string& reader_id, const std::string& input_path, const std::string& model_id) {
        auto reader = addComponent<lib::IEStream<std::string>>(reader_id, input_path.c_str());
        auto model = addComponent<AtomicT>(model_id);
        addCoupling(reader->out, model->phase_in);
    }

public:
    top_model(const std::string& id) : Coupled(id) {
        add_atomic_reader<Centrosome>("input_reader", "input_data/atomic_models/TC_Centrosome1.txt", "centrosome");
        add_atomic_reader<Nucleolus>("input_reader_2", "input_data/atomic_models/TC_Nucleolus1.txt", "nucleolus");
        add_atomic_reader<Chromosome>("input_reader_3", "input_data/atomic_models/TC_Chromosome1.txt", "chromosome_1");
        add_atomic_reader<Chromosome>("input_reader_4", "input_data/atomic_models/TC_Chromosome2.txt", "chromosome_2");
        add_atomic_reader<NuclearEnvelope>("input_reader_5", "input_data/atomic_models/TC_Nuclear_Envelope1.txt", "nuclear_envelope_1");
        add_atomic_reader<NuclearEnvelope>("input_reader_6", "input_data/atomic_models/TC_Nuclear_Envelope2.txt", "nuclear_envelope_2");
    }
};
