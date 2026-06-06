#include "SatelliteWorker.h"
#include "Error.h"
#include <iostream>

SatelliteWorker::SatelliteWorker(ConcurrentQueue* queue, ReceivingAntenna* antenna) {
    this->queue = queue;
    this->antenna = antenna;
}

void SatelliteWorker::run() {
    try {
        bool running = true;
        while (running) {
            RowData row = queue->pop();
            if (row.isEmpty()) {
                running = false;
            } else {
                antenna->receiveRow(row);
            }
        }
        antenna->finishAndSolve();
    } catch (SimulationError& e) {
        std::cerr << "Error in SatelliteWorker: " << e.what() << std::endl;
        throw;
    } catch (std::exception& e) {
        std::cerr << "Unknown error in SatelliteWorker: " << e.what() << std::endl;
        throw SimulationError("Unknown error", QUEUE_ERROR);
    }
}