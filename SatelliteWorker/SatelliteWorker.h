#ifndef SATELLITE_WORKER_H
#define SATELLITE_WORKER_H

#include "ConcurrentQueue.h"
#include "RowData.h"
#include "ReceivingAntenna.h"

class SatelliteWorker {
private:
    ConcurrentQueue* queue;
    ReceivingAntenna* antenna;

public:
    SatelliteWorker(ConcurrentQueue* queue, ReceivingAntenna* antenna);
    void run();
};

#endif