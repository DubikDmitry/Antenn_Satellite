#include "AntennaWorker.h"
#include <thread>
#include <chrono>
#include <iostream>

AntennaWorker::AntennaWorker(ConcurrentQueue* queue, Config* cfg, DataGenerator* gen) {
    this->queue = queue;
    this->cfg = cfg;
    this->generator = gen;
}

void AntennaWorker::run() {
    try {
        int flights = cfg->getNumberOfFlights();
        for (int i = 0; i < flights; i++) {
            std::vector<RowData> flightRows = generator->generateFlight();
            for (int j = 0; j < (int)flightRows.size(); j++) {
                queue->push(flightRows[j]);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        queue->push(RowData());
    } catch (SimulationError& e) {        
        std::cerr << "Ошибка в AntennaWorker: " << e.what() << std::endl;
        throw;
    } catch (std::exception& e) {          
        std::cerr << "Неизвестная ошибка в AntennaWorker: " << e.what() << std::endl;
        throw SimulationError("Неизвестная ошибка", GENERATION_FAILED);
    }
}