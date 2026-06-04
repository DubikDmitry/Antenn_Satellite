#include "MainController.h"

MainController::MainController(Config cfg)
    : config(cfg),
      generator(cfg.getSizeData(), cfg.getNumAntennas()),
      antenna(cfg.getNumSolverThreads(), "output.txt"),
      antennaWorker(&queue, &config, &generator),
      satelliteWorker(&queue, &antenna) {
}

void MainController::runSimulation() {
    std::thread antennaThread(&AntennaWorker::run, &antennaWorker);
    std::thread satelliteThread(&SatelliteWorker::run, &satelliteWorker);

    antennaThread.join();
    satelliteThread.join();
}