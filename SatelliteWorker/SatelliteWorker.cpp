#include "SatelliteWorker.h"

SatelliteWorker::SatelliteWorker(ConcurrentQueue* queue, ReceivingAntenna* antenna) {
    this->queue = queue;
    this->antenna = antenna;
}

void SatelliteWorker::run() {
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
}