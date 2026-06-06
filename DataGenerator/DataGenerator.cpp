#include "DataGenerator.h"
#include "Error.h"
#include <cmath>

DataGenerator::DataGenerator(int sizeData, int numAntennas) {
    if (sizeData <= 0 || numAntennas <= 0) {
        throw SimulationError("Invalid generator parameters (sizeData or numAntennas <= 0)", GENERATION_FAILED);
    }
    this->sizeData = sizeData;
    this->numAntennas = numAntennas;
    std::random_device rd;
    rng = std::mt19937(rd());
    dist = std::uniform_real_distribution<double>(-0.5, 0.5);
    globalRowCounter = 0;
}

void DataGenerator::resetCounter() {
    globalRowCounter = 0;
}

void DataGenerator::setSeed(unsigned int seed) {
    rng.seed(seed);
}

std::vector<RowData> DataGenerator::generateFlight() {
    std::vector<RowData> flight;
    flight.reserve(numAntennas);
    for (int i = 0; i < numAntennas; i++) {
        std::vector<double> values;
        values.reserve(sizeData);
        double sum_off_diag = 0.0;
        for (int j = 0; j < sizeData; j++) {
            double val = dist(rng);
            values.push_back(val);
            if (j != globalRowCounter % sizeData) {  
                sum_off_diag += std::abs(val);
            }
        }
        int diagPos = globalRowCounter % sizeData;
        values[diagPos] = sum_off_diag + 150.0;   

        flight.emplace_back(values);
        globalRowCounter++;
    }
    return flight;
}