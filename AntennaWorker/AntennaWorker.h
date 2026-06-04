#ifndef ANTENNA_WORKER_H
#define ANTENNA_WORKER_H

#include "ConcurrentQueue.h"
#include "RowData.h"
#include "Config.h"
#include "DataGenerator.h"
#include "Error.h"

class AntennaWorker {
private:
    ConcurrentQueue* queue;
    Config* cfg;
    DataGenerator* generator;

public:
    AntennaWorker(ConcurrentQueue* queue, Config* cfg, DataGenerator* gen);
    void run();
};

#endif