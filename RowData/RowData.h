#ifndef ROWDATA_H
#define ROWDATA_H
#include <vector>

class RowData {
private:
    std::vector<double> data;

public:
    RowData();
    RowData(std::vector<double> values);
    std::vector<double> getValues();
    bool isEmpty();
    double getCoeff(int idx);
    double getRHS();
    int size();
};

#endif