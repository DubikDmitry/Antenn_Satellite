#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include <vector>
#include <random>
#include "RowData.h"

class DataGenerator {
private:
    int sizeData;
    int numAntennas;
    std::mt19937 rng;
    std::uniform_real_distribution<double> dist;
    int globalRowCounter;          //счётчик строк

public:
    DataGenerator(int sizeData, int numAntennas);
    void setSeed(unsigned int seed);
    void resetCounter();               // сброс счётчика в 0
    std::vector<RowData> generateFlight();
};

#endif