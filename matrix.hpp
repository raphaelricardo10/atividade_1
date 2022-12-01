#ifndef MATRIX_H
#define MATRIX_H

#include <map>

typedef std::pair<int, int> MatrixEntryKey;

template <class T>
class Matrix : public std::map<MatrixEntryKey, T>
{
};
#endif