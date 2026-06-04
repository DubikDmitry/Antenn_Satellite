#include "MainController.h"
#include "Config.h"
#include "DataGenerator.h"
#include "ReceivingAntenna.h"
#include <iostream>
#include <chrono>
#include <vector>

int main() {
    int m = 2;          // облётов (строк)
    int n = 3;          // размер строки (неизвестных = n-1)
    int N = 2;          // антенн (строк за облёт)

    // Фиксируем seed для повторяемости
    DataGenerator generator(n, N);
    generator.setSeed(12345);

    // Генерируем данные один раз
    std::vector<RowData> allRows;
    for (int i = 0; i < m; i++) {
        std::vector<RowData> flight = generator.generateFlight();
        allRows.insert(allRows.end(), flight.begin(), flight.end());
    }

    std::cout << "Сгенерировано " << allRows.size() << " строк.\n";
    std::cout << "Число неизвестных: " << n - 1 << "\n";

    // Диапазон потоков для тестирования
    std::vector<int> threadCounts = {1, 2, 4, 8, 12};

    std::cout << "\nБенчмарк решения СЛАУ (метод Якоби) \n";
    for (int threads : threadCounts) {
        // Создаём приёмную антенну с пулом потоков
        ReceivingAntenna antenna(threads, "output_benchmark.txt");
        
        auto start = std::chrono::steady_clock::now();
        antenna.solveGivenRows(allRows);
        auto end = std::chrono::steady_clock::now();
        
        double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Потоков: " << threads << " -> время: " << elapsed_ms << " ms\n";
    }

    return 0;
}