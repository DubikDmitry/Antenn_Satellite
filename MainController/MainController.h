#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "Config.h"
#include "ConcurrentQueue.h"
#include "DataGenerator.h"
#include "AntennaWorker.h"
#include "ReceivingAntenna.h"
#include "SatelliteWorker.h"
#include <thread>

class MainController {
private:
    Config config;
    ConcurrentQueue queue;
    DataGenerator generator;
    ReceivingAntenna antenna;
    AntennaWorker antennaWorker;
    SatelliteWorker satelliteWorker;

public:
    MainController(const Config& cfg);
    void runSimulation();
};

#endif