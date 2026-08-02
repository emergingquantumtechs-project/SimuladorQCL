#include "../include/CSVReader.hpp"

#include <fstream>
#include <sstream>

using namespace std;

vector<vector<string>> CSVReader::read(const string& filename)
{
    vector<vector<string>> data;

    ifstream file(filename);

    if (!file.is_open())
    {
        return data;
    }

    string line;

    while (getline(file, line))
    {
        vector<string> row;

        stringstream ss(line);

        string cell;

        while (getline(ss, cell, ','))
        {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    return data;
}