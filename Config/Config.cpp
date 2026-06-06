#include "Config.h"

Config::Config(int flights, int dataSize, int antennas, int threads) {
    number_of_flights = flights;
    size_data = dataSize;
    num_antennas = antennas;
    num_solver_threads = threads;
}

int Config::getNumberOfFlights() const { return number_of_flights; }
int Config::getSizeData() const { return size_data; }
int Config::getNumAntennas() const { return num_antennas; }
int Config::getNumSolverThreads() const { return num_solver_threads; }