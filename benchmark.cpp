#include "MainController.h"
#include "Config.h"
#include "DataGenerator.h"
#include "ReceivingAntenna.h"
#include <iostream>
#include <chrono>
#include <vector>

int main() {
    int flights = 100;
    int row = 9999;
    int antenn = 100;

    DataGenerator generator(row, antenn);

    std::vector<RowData> allRows;
    allRows.reserve(flights * antenn);
    for (int i = 0; i < flights; i++) {
        std::vector<RowData> flight = generator.generateFlight();
        allRows.insert(allRows.end(), flight.begin(), flight.end());
    }
    std::cout << "flights " << flights << std::endl;
    std::cout << "row " << row << std::endl;
    std::cout << "antenn " << antenn << std::endl;
    std::cout << "Generated " << allRows.size() << " rows." << std::endl;
    std::cout << "Number of unknowns: " << row - 1 << std::endl;

    std::vector<int> threadCounts = {1, 2, 4, 8, 12, 16, 20, 30, 40};

    std::cout << "\n Solver benchmark Jacobi method " << std::endl;
    for (int threads : threadCounts) {
        try {
            ReceivingAntenna antenna(threads, "");
            auto start = std::chrono::steady_clock::now();
            antenna.solveGivenRows(allRows);
            auto end = std::chrono::steady_clock::now();
            double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
            std::cout << "Threads: " << threads << " -> time: " << elapsed_ms << " ms" << std::endl;
        } catch (SimulationError& e) {
            std::cerr << "Error for threads=" << threads << ": " << e.what() << std::endl;
        } catch (std::exception& e) {
            std::cerr << "Unexpected error: " << e.what() << std::endl;
        }
    }

    return 0;
}