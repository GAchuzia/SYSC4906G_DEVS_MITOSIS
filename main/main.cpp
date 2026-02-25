#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <set>
#include <filesystem>
#include <regex>
#include "cadmium/simulation/root_coordinator.hpp"
#include "cadmium/simulation/logger/stdout.hpp"
#include "../top_model/top.hpp"

using namespace cadmium;

namespace {
    const char* RESULTS_DIR = "simulation_results";
    const char* CSV_HEADER = "time;model_id;model_name;port_name;data";

    // Remove ANSI color/escape sequences so output files are plain text.
    std::string strip_ansi(const std::string& s) {
        static const std::regex ansi("\033\\[[0-9;]*m");
        return std::regex_replace(s, ansi, "");
    }

    void write_results_by_model(const std::string& captured_log) {
        std::string log = strip_ansi(captured_log);
        std::filesystem::create_directories(RESULTS_DIR);
        std::set<std::string> seen;
        std::istringstream in(log);
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            line = strip_ansi(line);
            std::istringstream line_in(line);
            std::string time_s, model_id, model_name, port_name, data;
            if (!std::getline(line_in, time_s, ';')) continue;
            std::getline(line_in, model_id, ';');
            std::getline(line_in, model_name, ';');
            std::getline(line_in, port_name, ';');
            std::getline(line_in, data, ';');
            if (model_name.empty() || model_name == "model_name") continue;
            std::string path = std::string(RESULTS_DIR) + "/" + model_name + ".txt";
            bool is_new = (seen.count(model_name) == 0);
            seen.insert(model_name);
            std::ofstream out(path, is_new ? std::ios::trunc : std::ios::app);
            if (is_new) out << CSV_HEADER << "\n";
            out << line << "\n";
        }
    }
}

int main() {
    auto model = std::make_shared<top_model>("top");
    auto rootCoordinator = RootCoordinator(model);
    rootCoordinator.setLogger<STDOUTLogger>(";");

    std::ostringstream capture;
    std::streambuf* prev = std::cout.rdbuf(capture.rdbuf());

    rootCoordinator.start();
    rootCoordinator.simulate(100.0);
    rootCoordinator.stop();

    std::cout.rdbuf(prev);
    std::string log = capture.str();
    std::cout << log;

    write_results_by_model(log);

    return 0;
}
