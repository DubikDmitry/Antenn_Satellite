#include "DataGenerator.h"
#include "Error.h"
#include <cmath>

DataGenerator::DataGenerator(int sizeData, int numAntennas) {
    if (sizeData <= 0 || numAntennas <= 0) {
        throw SimulationError("Неверные параметры генератора (sizeData или numAntennas <= 0)", GENERATION_FAILED);
    }
    this->sizeData = sizeData;
    this->numAntennas = numAntennas;
    std::random_device rd;
    rng = std::mt19937(rd());
    dist = std::uniform_real_distribution<double>(-1.0, 1.0);
}

void DataGenerator::setSeed(unsigned int seed) {
    rng.seed(seed);
}

std::vector<RowData> DataGenerator::generateFlight() {
    std::vector<RowData> flight;
    for (int i = 0; i < numAntennas; i++) {
        std::vector<double> values;
        double sum_off_diag = 0.0;
        
        // Генерируем недиагональные элементы
        for (int j = 0; j < sizeData; j++) {
            if (i == j && i < sizeData) {
                values.push_back(0.0);  // временно
            } else {
                double val = dist(rng);
                values.push_back(val);
                sum_off_diag += std::abs(val);
            }
        }
        
        // Диагональный элемент делаем больше суммы остальных
        if (i < sizeData) {
            double diag = sum_off_diag + 1.0 + std::abs(dist(rng));
            values[i] = diag;
        }
        
        flight.push_back(RowData(values));
    }
    return flight;
}