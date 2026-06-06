#ifndef RECEIVING_ANTENNA_H
#define RECEIVING_ANTENNA_H

#include <vector>
#include <fstream>
#include <string>
#include "RowData.h"
#include "ThreadPool.h"

class ReceivingAntenna {
private:
    std::vector<RowData> allRows;
    std::ofstream file;
    ThreadPool pool;

    void solveInternal();
    void printMatrix();

public:
    ReceivingAntenna(int numThreads, const std::string& filename);
    ~ReceivingAntenna();
    void receiveRow(const RowData& row);
    void finishAndSolve();
    void solveGivenRows(const std::vector<RowData>& rows);
};

#endif