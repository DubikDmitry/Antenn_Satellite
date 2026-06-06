#ifndef CONFIG_H
#define CONFIG_H

class Config {
private:
    int number_of_flights;
    int size_data;
    int num_antennas;
    int num_solver_threads;

public:
    Config(int flights, int dataSize, int antennas, int threads);
    int getNumberOfFlights() const;
    int getSizeData() const;
    int getNumAntennas() const;
    int getNumSolverThreads() const;
};

#endif