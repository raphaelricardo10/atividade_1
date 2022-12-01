#ifndef FACILITIES_PROBLEM_H
#define FACILITIES_PROBLEM_H

#include <string>

#include "matrix.hpp"

typedef Matrix<double> TransferCostsMatrix;
typedef Matrix<int> CapacityUsagesMatrix;
typedef MatrixEntryKey Edge;

class FacilitiesProblem
{
public:
    int capacity;
    int opening_cost;
    int number_of_clients;
    int number_of_facilities;

    TransferCostsMatrix transfer_costs;
    CapacityUsagesMatrix capacity_usages;

    FacilitiesProblem(int number_of_facilities, int number_of_clients, int opening_cost, int capacity, TransferCostsMatrix transfer_costs, CapacityUsagesMatrix capacity_usages)
    {
        this->capacity = capacity;
        this->opening_cost = opening_cost;
        this->number_of_clients = number_of_clients;
        this->number_of_facilities = number_of_facilities;

        this->transfer_costs = transfer_costs;
        this->capacity_usages = capacity_usages;
    }

    FacilitiesProblem(std::string filename);
};

#endif