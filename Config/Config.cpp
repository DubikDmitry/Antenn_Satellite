#include "Config.h"

Config::Config(int flights, int dataSize, int antennas, int threads) {
    number_of_flights = flights;
    size_data = dataSize;
    num_antennas = antennas;
    num_solver_threads = threads;
}

int Config::getNumberOfFlights() { return number_of_flights; }

int Config::getSizeData() { return size_data; }

int Config::getNumAntennas() { return num_antennas; }

int Config::getNumSolverThreads() { return num_solver_threads; }