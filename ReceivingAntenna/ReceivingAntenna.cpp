#include "ReceivingAntenna.h"
#include "Error.h"
#include <iostream>
#include <cmath>

ReceivingAntenna::ReceivingAntenna(int numThreads, std::string filename) : pool(numThreads) {
    if (!filename.empty()) {
        file.open(filename.c_str());
        if (!file.is_open()) {
            throw SimulationError("Не удалось открыть файл " + filename, FILE_ERROR);
        }
    }
}

ReceivingAntenna::~ReceivingAntenna() {
    if (file.is_open()) {
        file.close();
    }
}

void ReceivingAntenna::receiveRow(RowData row) {
    allRows.push_back(row);
    std::vector<double> values = row.getValues();
    for (int i = 0; i < (int)values.size(); i++) {
        file << values[i];
        if (i != (int)values.size() - 1) file << " ";
    }
    file << std::endl;
}

void ReceivingAntenna::finishAndSolve() {
    std::cout << "Итоговое СЛАУ:" << std::endl;
    printMatrix();
    solveInternal();
}

void ReceivingAntenna::printMatrix() {
    for (int i = 0; i < (int)allRows.size(); i++) {
        std::vector<double> values = allRows[i].getValues();
        for (int j = 0; j < (int)values.size(); j++) {
            std::cout << values[j];
            if (j != (int)values.size() - 1) std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void ReceivingAntenna::solveInternal() {
    if (allRows.empty()) {
        std::cout << "Нет данных для решения." << std::endl;
        return;
    }
    int n = allRows[0].size();
    int unknowns = n - 1;
    int totalRows = (int)allRows.size();

    if (totalRows < unknowns) {
        std::cout << "Недостаточно уравнений. Нужно хотя бы " << unknowns << ", получено " << totalRows << std::endl;
        return;
    }

    std::vector<std::vector<double> > A(unknowns, std::vector<double>(unknowns));
    std::vector<double> b(unknowns);
    for (int i = 0; i < unknowns; i++) {
        std::vector<double> values = allRows[i].getValues();
        for (int j = 0; j < unknowns; j++) {
            A[i][j] = values[j];
        }
        b[i] = values[unknowns];
    }

    int maxIter = 1000;
    double eps = 1e-6;
    std::vector<double> x(unknowns, 0.0);
    std::vector<double> x_new(unknowns, 0.0);
    bool converged = false;
    for (int iter = 0; iter < maxIter && !converged; iter++) {
        for (int i = 0; i < unknowns; i++) {
            try {
                std::unique_ptr<Task> task(new JacobiTask(A, b, x, x_new, i));
                pool.add_task(std::move(task));
            } catch (SimulationError& e) {
                std::cerr << "Ошибка добавления задачи: " << e.what() << std::endl;
                throw;
            }
        }
        pool.wait();

        double maxDiff = 0.0;
        for (int i = 0; i < unknowns; i++) {
            double diff = x_new[i] - x[i];
            if (diff < 0) diff = -diff;
            if (diff > maxDiff) maxDiff = diff;
            x[i] = x_new[i];
        }
        if (maxDiff < eps) {
            std::cout << "Сошлось за " << iter + 1 << " итераций." << std::endl;
            converged = true;
        }
    }

    std::cout << "РЕШЕНИЕ СЛАУ:" << std::endl;
    for (int i = 0; i < unknowns; i++) {
        std::cout << "x" << i << " = " << x[i] << std::endl;
    }
}

void ReceivingAntenna::solveGivenRows(const std::vector<RowData>& rows) {
    allRows = rows;
    
    // Записываем в файл (если файл открыт)
    if (file.is_open()) {
        for (int i = 0; i < (int)allRows.size(); i++) {
            std::vector<double> values = allRows[i].getValues();
            for (int j = 0; j < (int)values.size(); j++) {
                file << values[j];
                if (j != (int)values.size() - 1) file << " ";
            }
            file << std::endl;
        }
    }
    
    solveInternal();
}