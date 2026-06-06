#include "MainController.h"
#include "Config.h"
#include "Error.h"
#include <iostream>

int main() {
    int m = 2;
    int n = 3;
    int N = 2;
    int numThreads = 4;

    try {
        Config cfg(m, n, N, numThreads);
        MainController controller(cfg);
        controller.runSimulation();
    } catch (SimulationError& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (std::exception& e) {
        std::cerr << "Unknown exception: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}