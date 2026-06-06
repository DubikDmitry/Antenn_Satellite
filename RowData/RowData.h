#ifndef ROWDATA_H
#define ROWDATA_H
#include <vector>

class RowData {
private:
    std::vector<double> data;

public:
    RowData();
    RowData(const std::vector<double>& values);
    std::vector<double> getValues() const;
    bool isEmpty() const;
    double getCoeff(size_t idx) const;
    double getRHS() const;
    size_t size() const;
};

#endif