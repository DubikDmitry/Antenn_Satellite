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

public:
    DataGenerator(int sizeData, int numAntennas);
    void setSeed(unsigned int seed);              // ← добавить эту строку
    std::vector<RowData> generateFlight();
};

#endif