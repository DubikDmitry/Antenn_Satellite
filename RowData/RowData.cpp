#include "RowData.h"

RowData::RowData() {
}

RowData::RowData(std::vector<double> values) { data = values; }

std::vector<double> RowData::getValues() { return data; }

bool RowData::isEmpty() { return data.empty(); }

double RowData::getCoeff(int idx) { return data[idx]; }

double RowData::getRHS() { return data[data.size() - 1]; }

int RowData::size() { return (int)data.size(); }