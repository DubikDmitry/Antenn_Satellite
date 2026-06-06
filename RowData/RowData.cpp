#include "RowData.h"

RowData::RowData() { }

RowData::RowData(const std::vector<double>& values) : data(values) { }

std::vector<double> RowData::getValues() const { return data; }

bool RowData::isEmpty() const { return data.empty(); }

double RowData::getCoeff(size_t idx) const { return data[idx]; }

double RowData::getRHS() const { return data.back(); }

size_t RowData::size() const { return data.size(); }