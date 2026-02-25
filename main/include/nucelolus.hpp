#ifndef CENTROSOME_HPP
#define CENTROSOME_HPP

#include <cadmium/modeling/devs/port.hpp>

#include <limits>
#include <string>

using namespace cadmium;
using namespace std;

struct Nucleolus {

    //Ports
    struct phase_in : public in_port<string> {};
    struct status_out : public out_port<string> {};

    using input_ports = tuple<phase_in>;
    using output_ports = tuple<status_out>;

    //State 
    struct state_type {
        string state;
        bool active;
    };

    state_type state;

    Nucleolus() {
        state.state = "visble"; 
        state.active = false
    }

    // Internal Transition
    void internal_transition() {
        state.active = false;
    }

    // External Transition
    void external_transition(TIME e,
        typename make_message_bags<input_ports>::type mbs) {

        for (const auto &x : get_messages<Nucleolus_defs::phase_in>(mbs)) {

            if (x == "Interphase") {
                state.state = "visible";
                state.active = false;
            }
            else if (x == "Prophase") {
                state.state = "disappeared";
                state.active = true;   // transition process
            }
            else if (x == "Telophase") {
                state.state = "reappearing";
                state.active = true;   // transition process
            }
            else if (x == "Cytokinesis") {
                state.state = "visible";
                state.active = false;
            }
        }
    }

     // Output
    typename make_message_bags<output_ports>::type output() const {

        typename make_message_bags<output_ports>::type bags;

        std::vector<std::string> bag_port_out;

        if (state.state == "visible") {
            bag_port_out.push_back("ready");
        } else {
            bag_port_out.push_back("not_ready");
        }

        get_messages<typename Centrosome_defs::status_out>(bags) = bag_port_out;

        return bags;
    }


    TIME time_advance() const {
        if (state.active){
            if (state.state == "disappeared")
                return TIME("00:00:02:000");  // t1

            if (state.state == "reappearing")
                return TIME("00:00:03:000");  // t2
        }
        return numeric_limits<TIME>::infinity();

    }
}