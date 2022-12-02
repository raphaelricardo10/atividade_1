#ifndef FACILITIES_FILE_H
#define FACILITIES_FILE_H

#include <string>
#include <vector>
#include <fstream>

#include "matrix.hpp"
#include "facilities_problem.hpp"

class FacilitiesFile
{
private:
    int number_of_entries;

    std::vector<std::string> split_line(std::string line)
    {
        size_t pos = 0;
        std::string token;
        char delimiter = ';';
        std::vector<std::string> all_tokens;

        while ((pos = line.find(delimiter)) != std::string::npos)
        {
            token = line.substr(0, pos);
            all_tokens.push_back(token);
            line.erase(0, pos + 1);
        }

        all_tokens.push_back(line);

        return all_tokens;
    }

public:
    std::string filename;

    FacilitiesFile(std::string filename)
    {
        this->filename = filename;
        this->number_of_entries = 0;
    }

    FacilitiesProblem read_file()
    {
        std::string fileLine;
        std::ifstream fileStream(this->filename);

        getline(fileStream, fileLine);

        std::vector<std::string> header = this->split_line(fileLine);

        Matrix<double> transfer_costs;
        Matrix<int> capacity_usages;

        while (getline(fileStream, fileLine))
        {
            std::vector<std::string> entry = this->split_line(fileLine);

            int facility = atoi(entry[0].c_str()) - 1;
            int client = atoi(entry[1].c_str()) - 1;
            double transfer_cost = atof(entry[2].c_str());
            int capacity_cost = atoi(entry[3].c_str());

            MatrixEntryKey key = std::make_tuple(facility, client);

            transfer_costs[key] = transfer_cost;
            capacity_usages[key] = capacity_cost;
        }

        return FacilitiesProblem(atoi(header[0].c_str()), atoi(header[1].c_str()), atoi(header[2].c_str()), atoi(header[3].c_str()), transfer_costs, capacity_usages);
    }
};

#endif