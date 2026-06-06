#include "ReceivingAntenna.h"
#include "Error.h"
#include <iostream>
#include <cmath>

ReceivingAntenna::ReceivingAntenna(int numThreads, const std::string& filename) : pool(numThreads) {
    if (!filename.empty()) {
        file.open(filename.c_str());
        if (!file.is_open()) {
            throw SimulationError(std::string("Failed to open file ") + filename, FILE_ERROR);
        }
    }
}

ReceivingAntenna::~ReceivingAntenna() {
    if (file.is_open()) {
        file.close();
    }
}

void ReceivingAntenna::receiveRow(const RowData& row) {
    allRows.push_back(row);
    if (file.is_open()) {
        std::vector<double> values = row.getValues();
        for (size_t i = 0; i < values.size(); ++i) {
            file << values[i];
            if (i + 1 < values.size()) file << " ";
        }
        file << std::endl;
    }
}

void ReceivingAntenna::finishAndSolve() {
    std::cout << "Final SLAE:" << std::endl;
    printMatrix();
    solveInternal();
}

void ReceivingAntenna::printMatrix() {
    for (size_t i = 0; i < allRows.size(); ++i) {
        std::vector<double> values = allRows[i].getValues();
        for (size_t j = 0; j < values.size(); ++j) {
            std::cout << values[j];
            if (j + 1 < values.size()) std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void ReceivingAntenna::solveInternal() {
    if (allRows.empty()) {
        std::cout << "No data to solve." << std::endl;
        return;
    }
    size_t n = allRows[0].size();
    size_t unknowns = n - 1;
    size_t totalRows = allRows.size();

    if (totalRows < unknowns) {
        std::cout << "Not enough equations. Need at least " << unknowns << ", got " << totalRows << std::endl;
        return;
    }

    std::vector<std::vector<double> > A(unknowns, std::vector<double>(unknowns));
    std::vector<double> b(unknowns);
    for (size_t i = 0; i < unknowns; ++i) {
        std::vector<double> values = allRows[i].getValues();
        for (size_t j = 0; j < unknowns; ++j) {
            A[i][j] = values[j];
        }
        b[i] = values[unknowns];
    }
    
    allRows.clear();
    allRows.shrink_to_fit();
    // Проверка диагональных элементов
    for (size_t i = 0; i < unknowns; ++i) {
        if (std::abs(A[i][i]) < 1e-10) {
            std::cout << "Zero diagonal element at " << i << std::endl;
            throw SimulationError("Zero diagonal", SOLVER_FAILED);
        }
    }

    int maxIter = 1000;
    double eps = 1e-6;
    std::vector<double> x(unknowns, 0.0);
    std::vector<double> x_new(unknowns, 0.0);
    bool converged = false;

    for (int iter = 0; iter < maxIter && !converged; ++iter) {
        for (size_t i = 0; i < unknowns; ++i) {
            try {
                std::unique_ptr<SolverMethod> method(new JacobiMethod(A, b, x, x_new, i));
                pool.add_task(std::move(method));
            } catch (SimulationError& e) {
                std::cerr << "Error adding task: " << e.what() << std::endl;
                throw;
            }
        }
        pool.wait();

        double maxDiff = 0.0;
        for (size_t i = 0; i < unknowns; ++i) {
            if (std::isnan(x_new[i]) || std::isinf(x_new[i])) {
                throw SimulationError("NaN in solution", SOLVER_FAILED);
            }
            double diff = x_new[i] - x[i];
            if (diff < 0) diff = -diff;
            if (diff > maxDiff) maxDiff = diff;
            x[i] = x_new[i];
        }
        if (maxDiff < eps) {
            std::cout << "Converged in " << iter + 1 << " iterations." << std::endl;
            converged = true;
        }
    }

    // std::cout << "SOLUTION:" << std::endl;
    // for (size_t i = 0; i < unknowns; ++i) {
    //     std::cout << "x" << i << " = " << x[i] << std::endl;
    // }
}

void ReceivingAntenna::solveGivenRows(const std::vector<RowData>& rows) {
    allRows = rows;
    if (file.is_open()) {
        for (size_t i = 0; i < allRows.size(); ++i) {
            std::vector<double> values = allRows[i].getValues();
            for (size_t j = 0; j < values.size(); ++j) {
                file << values[j];
                if (j + 1 < values.size()) file << " ";
            }
            file << std::endl;
        }
    }
    solveInternal();
}